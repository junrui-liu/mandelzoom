/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#include <cstdlib>
#include <GLUT/glut.h>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

// Defining default values for window size, shape and location.
#define INITIAL_WIN_W 800
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 0
#define INITIAL_WIN_Y 0
#define NUM_ITER 500
#define PI 3.14159265358979323846
#define PI_2 (2*PI)
#define USE_MY_PARAM
//#define OUTPUT_FILE "./out.csv"

template <class T>
class Point {
public:
    T x;
    T y;
    Point (T x1, T y1)
    {
        x = x1;
        y = y1;
    }
    /* copy constructor */
    Point (const Point<T>& p)
    {
        x = p.x;
        y = p.y;
    }
};

class Window {
public:
    Point<double> base;
    double ratio;
    Window () :
        base(Point<double> (0.0, 0.0)),
        ratio(1.0) {}
    Window (Point<double> p, double r) :
        base(p),
        ratio(r) {}
    
    /* copy constructor */
    Window (const Window& w) :
        base(w.base),
        ratio(w.ratio) {}
};


stack<Window> undo;
stack<Window> redo;

// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight = INITIAL_WIN_W, windowWidth = INITIAL_WIN_H;


#ifdef USE_MY_PARAM
    double myParam[] =
{-2, -1.25, INITIAL_WIN_W/2.5};
//{-1.06236,0.260434,128835};
//{-0.748273,-0.0544996, 7.13855e+06};
//{-0.749956,-0.0529485,4.92623e+07};
//{-0.74321,0.113349,1.2128e+06};
    Point<double> windowBotLeft (myParam[0], myParam[1]);
    double initRatio = myParam[2];
#else
//    double param[] = {-2, 0.5, -1.25, 1.25};
//    double param[] = {0.262, 0.263, 0.002, 0.003};
//    double param[] = {0.26215, 0.26225, 0.00215, 0.00225};
//    double param[] = {-1.4014, -1.4010, -0.0002, 0.0002};
//    double param[] = {0.281, 0.291, -0.019, -0.009};
    Point<double> windowBotLeft (param[0], param[2]);
    Point<double> windowTopRight (param[1], param[3]);
#endif


template <class T>
void complexAdd(const Point<T>& p, const Point<T>& q, Point<T>& result )
{
    result.x = p.x + q.x;
    result.y = p.y + q.y;
}

template <class T>
void complexMult(const Point<T>& p, const Point<T>& q, Point<T>& result )
{
    T a = p.x, b = p.y, c = q.x, d = q.y;
    result.x = a*c - b*d;
    result.y = a*d + b*c;
}

template <class T>
void complexMod(const Point<T>& p, T& result )
{
    result = sqrt(p.x * p.x + p.y * p.y);
}

template <class T>
int computeIterations(const Point<T>& c, int bound)
{
    Point<T> z (0.0, 0.0);
    T mod;
    for (int run = 0; run < bound; ++run)
    {
        complexMult(z, z, z);
        complexAdd(z, c, z);
        complexMod(z, mod);
        if (mod > 2) return run;
    }
    return bound;
}

int computeIterationsInline(const Point<double>& c, int bound)
{
    double zx = 0.0, zy = 0.0;
    double cx = c.x, cy = c.y;
    double mod_sq;
    double tmp_x, tmp_y;
    for (int run = 0; run < bound; ++run)
    {
        tmp_x = zx * zx - zy * zy + cx;
        tmp_y = 2 * zx * zy + cy;
        mod_sq = tmp_x * tmp_x + tmp_y * tmp_y;
        if (mod_sq > 4) return run;
        zx = tmp_x;
        zy = tmp_y;
    }
    return bound;
}

void pixelToPoint(const Point<int>& pixel, Point<double>& point)
{
    const Window& current = undo.top();
    point.x = (1.0 * pixel.x) / current.ratio + current.base.x;
    point.y = (1.0 * pixel.y) / current.ratio + current.base.y;
}

void assignColor(double intensity, double& r, double& g, double& b )
{
    if (intensity == 1)
    {
        r = 0; g = 0; b = 0;
    }
    else
    {
        double I = intensity;
        //                double I = intensity;
        r = sin(I*PI_2*0.95);
        g = sin(I*PI_2*1.2);
        b = sin(I*PI_2*1.3);
//        r=I;g=I;b=I;
        //                a = 1-I;
    }
}

void computeCumulativeFrequency()
{
    int N = NUM_ITER;
    Point<double> c (0,0);
    Point<int> current_pixel (0,0);
    
    unsigned int n_partitions = 100;
    double partition_size = 1.0 / n_partitions;
    unsigned long nonCumulativeFreqs [n_partitions];
    unsigned long nonCumulativeTotal = 0;
    for (auto& x: nonCumulativeFreqs) x = 0;
    
    int n, p;
    double intensity;
    for (int u = 0; u < windowWidth; u++)
    {
        for (int v = 0; v < windowHeight; v++)
        {
            current_pixel.x = u;
            current_pixel.y = v;
            pixelToPoint(current_pixel, c);
            n = computeIterationsInline(c, N);
            intensity = (1.0 * n) / N;
            p = (unsigned int) floor(intensity / partition_size);
            
            nonCumulativeFreqs[p] += 1;
            nonCumulativeTotal += 1;
        }
    }
    
    double cumulativeFreqs [n_partitions];
    for (auto& x: cumulativeFreqs) x = 0;
    unsigned long cumulativeTotal;
    cumulativeFreqs[0] = nonCumulativeFreqs[0];
    cumulativeTotal = nonCumulativeFreqs[0];
    for (int i = 1; i < n_partitions; i++)
    {
        cumulativeFreqs[i] = nonCumulativeFreqs[i] + cumulativeFreqs[i-1];
    }
    for (int i = 0; i < n_partitions; i++)
    {
        cumulativeFreqs[i] /= nonCumulativeTotal;
        cout << i << "," <<  nonCumulativeFreqs[i];
        cout << endl;
//        cout << "," << cumulativeFreqs[i] << endl;
    }
    
}

void writeRawToFile(const string& filename)
{
    ofstream myfile (filename, ios::out | ios::trunc);
    if ( myfile.is_open() )
    {
        int N = NUM_ITER;
        Point<double> c (0,0);
        Point<int> current_pixel (0,0);
        int n;
        double intensity;
        int step = 5;
        for (int u = 0; u < windowWidth; u += step)
        {
            for (int v = 0; v < windowHeight; v += step)
            {
                current_pixel.x = u;
                current_pixel.y = v;
                pixelToPoint(current_pixel, c);
                n = computeIterationsInline(c, N);
                intensity = (1.0 * n) / N;
                myfile << u*windowWidth+v << "," << intensity << endl;
            }
        }
        myfile.close();
        cout << "finished writing";
    }
    else cout << "cannot open file.\n";
}

void drawPartialMandelbrot(int x0, int y0, int x1, int y1)
{
    int N = NUM_ITER;
    Point<double> c (0,0);
    Point<int> current_pixel (0,0);
    int step = 1;
    int n;
    glBegin( GL_POINTS );
    for (int u = x0; u <= x1; u += step)
    {
        for (int v = y0; v <= y1; v += step)
        {
            current_pixel.x = u;
            current_pixel.y = v;
            pixelToPoint(current_pixel, c);
            //            int n = computeIterations(c, N);
            n = computeIterationsInline(c, N);
            
            double r,g,b;
            assignColor(1.0 * n/N, r, g, b);
            glColor3f(r, g, b);
//            for (int u1 = u; u1 < u + step; u1++)
//                for (int v1 = v; v1 < v + step; v1++)
                    glVertex2i(u, v);
        }
    }
    glEnd();
    glFlush();
}


void drawMandelbrot()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPartialMandelbrot(0, 0, windowWidth, windowHeight);
//    computeCumulativeFrequency();
#ifdef OUTPUT_FILE
    writeRawToFile(OUTPUT_FILE);
#endif
}


void clearPicture()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void drawLine(int xOld, int yOld, int xNew, int yNew )
// Draw a line from (xOld,yOld) to (xNew,yNew).
{
    glBegin( GL_LINES );
    glVertex2i( xOld, yOld );
    glVertex2i( xNew, yNew );
    glEnd();
    glFlush();
}


void drawRubberBand(int xA, int yA, int xS, int yS)
{
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    glBegin( GL_LINE_LOOP );
    glVertex2i( xA, yA );
    glVertex2i( xA, yS );
    glVertex2i( xS, yS );
    glVertex2i( xS, yA );
    glEnd();
    glDisable(GL_COLOR_LOGIC_OP);
    glFlush();
}

void rubberBand( int x, int y )
// Callback for processing mouse motion.
{
    if ( rubberBanding )
    {
        drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
        y = windowHeight - y;
        xStretch = x;
        yStretch = y;
        drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
        glFlush();
    }
}

void reshape( int w, int h )
// Callback for processing reshape events.
{
//    cout << w << "," << h << endl;
//
//    if (w > windowWidth)
//    {
//        drawPartialMandelbrot(windowWidth, w, 0, h);
//        if (h > windowHeight)
//            drawPartialMandelbrot(0, windowWidth, windowHeight, h);
//    }
//    else if (h > windowHeight)
//    {
//        drawPartialMandelbrot(0, w, windowHeight, h);
//        if (h > windowHeight)
//            drawPartialMandelbrot(windowWidth, w, 0, windowHeight);
//    }
    windowWidth = w;
    windowHeight = h;
    
    drawMandelbrot();
//    glViewport( 0, 0, ( GLsizei )w, ( GLsizei )h );
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    gluOrtho2D( 0.0, ( GLdouble )w, 0.0, ( GLdouble )h );
}


void escExit( GLubyte key, int, int )
// Callback for processing keyboard events.
{
    if ( key == 27 /* ESC */ ) std::exit( 0 );
}


void mainMenu( int item )
// Callback for processing main menu.
{
    Window w;
    switch ( item )
    {
        case 1: // undo
            if (!undo.empty())
            {
                w = undo.top();
                undo.pop();
                redo.push(w);
                drawMandelbrot();
                break;
            }
        case 2: // redo
            if (!redo.empty())
            {
                w = redo.top();
                redo.pop();
                undo.push(w);
                drawMandelbrot();
            }
            break;
        case 3: // exit
            std::exit( 0 );
    }
}

void setMenus()
// Function for creating menus.
{
    glutCreateMenu( mainMenu );
    glutAddMenuEntry( "Undo", 1 );
    glutAddMenuEntry( "Redo", 2 );
    glutAddMenuEntry( "Exit", 3);
//    glutAttachMenu( GLUT_MIDDLE_BUTTON );
    glutAttachMenu( GLUT_RIGHT_BUTTON ); // TODO: switch to middle button when done
}


void processLeftDown( int x, int y )
// Function for processing mouse left botton down events.
{
    if (!rubberBanding)
    {
        int xNew = x;
        int yNew = windowHeight - y;
        xAnchor = xNew;
        yAnchor = yNew;
        xStretch = xNew;
        yStretch = yNew;
        drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
        rubberBanding = true;
    }
}

void processLeftUp( int x, int y )
// Function for processing mouse left botton up events.
{
    if (rubberBanding)
    {
        int xNew, yNew;
        drawRubberBand(xAnchor,yAnchor,xStretch,yStretch);
        rubberBanding = false;
        xNew = x;
        yNew = windowHeight - y;
        
        int xMin = min(xAnchor, xNew);
        int yMin = min(yAnchor, yNew);
        
        int xMax  = max(xAnchor, xNew); // or could use the xor trick
        int yMax  = max(yAnchor, yNew);
        
        int newWidth = xMax - xMin;
        int newHeight = yMax - yMin;
        
        double ratioNewOldWidth = (1.0 * newWidth) / windowWidth;
        double ratioNewOldHeight = (1.0 * newHeight) / windowHeight;
        
        double ratioFinal = max(ratioNewOldWidth, ratioNewOldHeight);
        if (ratioNewOldWidth > ratioNewOldHeight) // width better fits
        {
            newHeight = windowHeight * ratioNewOldWidth;
        }
        else if (ratioNewOldHeight > ratioNewOldWidth)
        {
            newWidth = windowWidth * ratioNewOldHeight;
        }

        Point<int> pixelBase (0,0);
        pixelBase.x = (xMax + xMin - newWidth) / 2;
        pixelBase.y = (yMax + yMin - newHeight) / 2;
        
        Window newWindow (undo.top());
        pixelToPoint(pixelBase, newWindow.base);
        newWindow.ratio /= ratioFinal;
        undo.push(newWindow);
        
        while (!redo.empty()) redo.pop();
        
        drawMandelbrot();
        cout << newWindow.base.x << "," << newWindow.base.y << "," << newWindow.ratio << endl;
        
    }
}

void mouse( int button, int state, int x, int y )
// Function for processing mouse events.
{
    if ( button == GLUT_LEFT_BUTTON )
    {
        switch ( state )
        {
            case GLUT_DOWN: processLeftDown( x, y ); break;
            case GLUT_UP: processLeftUp( x, y ); break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        
    }
}

int main( int argc, char * argv[] )
{
    // Mask floating point exceptions.
    //    _control87( MCW_EM, MCW_EM );
    
    // Initialize glut with command line parameters.
    glutInit( & argc, argv );
    
    // Choose RGB display mode for normal screen window.
    glutInitDisplayMode( GLUT_RGB );
    
    // Set initial window size, position, and title.
    glutInitWindowSize( INITIAL_WIN_W, INITIAL_WIN_H );
    glutInitWindowPosition( INITIAL_WIN_X, INITIAL_WIN_Y );
    windowWidth = INITIAL_WIN_W;
    windowHeight = INITIAL_WIN_H;
    glutCreateWindow( "Mandelzoom" );
    
#ifdef USE_MY_PARAM
    Window currentWindow (windowBotLeft, initRatio);
#else
    Window currentWindow (windowBotLeft,
                          windowWidth/(windowTopRight.x - windowBotLeft.x));
#endif
    undo.push(currentWindow);
    
    // You don't (yet) want to know what this does.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, ( double )INITIAL_WIN_W, 0.0, ( double )INITIAL_WIN_H );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
//    glTranslatef( 0.375, 0.375, 0.0 );
    
//    // Set the drawing color.
//    glColor3f(1.0,1.0,1.0);
//
//    // Set the color for clearing the window.
//    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    
    // Set up the menus.
    setMenus();
    
    // Set the callbacks for the normal screen window.
    glutDisplayFunc( drawMandelbrot );
    glutMouseFunc( mouse );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( escExit );
    glutMotionFunc( rubberBand );
    glutPassiveMotionFunc( rubberBand );
    
    glutMainLoop();
    return 0;
}


