/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <fstream>
#include <stack>
#include "mandelzoom.h"

using namespace std;

bool init = true;
stack<Window> smaller;
stack<Window> larger;


double init_param[] = INIT_PARAM;
Point<double> windowBotLeft(init_param[0], init_param[1]);
double initRatio = init_param[2];

// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight = INITIAL_WIN_W, windowWidth = INITIAL_WIN_H;

void assignColor(double intensity, double& r, double& g, double& b)
{
    if (intensity == 1)
    {
        r = 0; g = 0; b = 0;
    }
    else
    {
        double I = intensity;
        //                double I = intensity;
        r = sin(I*PI_2*0.9);
        g = sin(I*PI_2*1.2);
        b = sin(I*PI_2*1.3);
        //        r=I;g=I;b=I;
    }
}

void drawPartialMandelbrot(int x0, int y0, int x1, int y1)
{
    unsigned int N = NUM_ITER;
    Point<double> c(0, 0);
    Point<int> current_pixel(0, 0);
    
    int step = 1;
    int flush_step = step * 10;
    unsigned int n;
    Window& current = smaller.top();
    
    for (int u = x0, stride = flush_step; u <= x1; u += step, stride += step)
    {
        if (stride == flush_step)
        {
            stride = 0;
            glEnd();
            //glFlush();
        }
        if (stride == 0)
            glBegin(GL_POINTS);
        for (int v = y0; v <= y1; v += step)
        {
            current_pixel.x = u;
            current_pixel.y = v;
            current.pixelToPoint(current_pixel, c);
            n = Point<double>::computeIterationsInline(c, N);
            
            double r, g, b;
            assignColor(1.0 * n / N, r, g, b);
            glColor3f(r, g, b);
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

void drawLine(int xOld, int yOld, int xNew, int yNew)
// Draw a line from (xOld,yOld) to (xNew,yNew).
{
    glBegin(GL_LINES);
    glVertex2i(xOld, yOld);
    glVertex2i(xNew, yNew);
    glEnd();
    glFlush();
}


void drawRubberBand(int xA, int yA, int xS, int yS)
{
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xA, yA);
    glVertex2i(xA, yS);
    glVertex2i(xS, yS);
    glVertex2i(xS, yA);
    glEnd();
    glDisable(GL_COLOR_LOGIC_OP);
    glFlush();
}

void rubberBand(int x, int y)
// Callback for processing mouse motion.
{
    if (rubberBanding)
    {
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        y = windowHeight - y;
        xStretch = x;
        yStretch = y;
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        glFlush();
    }
}

void reshape(int w, int h)
// Callback for processing reshape events.
{
    if (!init)
    {
        Window& current = smaller.top();
        current.base.y += (windowHeight - h) / current.ratio;
        if (w > windowWidth)
        {
            drawPartialMandelbrot(windowWidth, w, 0, h);
            if (h > windowHeight)
                drawPartialMandelbrot(0, windowWidth, windowHeight, h);
        }
        else if (h > windowHeight)
        {
            drawPartialMandelbrot(0, w, windowHeight, h);
            if (h > windowHeight)
                drawPartialMandelbrot(windowWidth, w, 0, windowHeight);
        }
        windowWidth = w;
        windowHeight = h;
    }
    init = false;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}


void escExit(GLubyte key, int, int)
// Callback for processing keyboard events.
{
    if (key == 27 /* ESC */) std::exit(0);
}


void mainMenu(int item)
// Callback for processing main menu.
{
    switch (item)
    {
        case 1: // smaller
            if (!smaller.empty())
            {
                Window& w = smaller.top();
                smaller.pop();
                larger.push(w);
                drawMandelbrot();
                
            }
            break;
        case 2: // larger
            if (!larger.empty())
            {
                Window& w = larger.top();
                larger.pop();
                smaller.push(w);
                drawMandelbrot();
            }
            break;
        case 3: // exit
            std::exit(0);
    }
}

void setMenus()
// Function for creating menus.
{
    glutCreateMenu(mainMenu);
    glutAddMenuEntry("smaller", 1);
    glutAddMenuEntry("larger", 2);
    glutAddMenuEntry("Exit", 3);
    //    glutAttachMenu( GLUT_MIDDLE_BUTTON );
    glutAttachMenu(GLUT_RIGHT_BUTTON); // TODO: switch to middle button when done
}


void processLeftDown(int x, int y)
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
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        rubberBanding = true;
    }
}

void processLeftUp(int x, int y)
// Function for processing mouse left botton up events.
{
    if (rubberBanding)
    {
        int xNew, yNew;
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        rubberBanding = false;
        xNew = x;
        yNew = windowHeight - y;
        
        int xMin = min(xAnchor, xNew);
        int yMin = min(yAnchor, yNew);
        
        int xMax = max(xAnchor, xNew); // or could use the xor trick
        int yMax = max(yAnchor, yNew);
        
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
        
        Point<int> pixelBase(0, 0);
        pixelBase.x = (xMax + xMin - newWidth) / 2;
        pixelBase.y = (yMax + yMin - newHeight) / 2;
        
        Window& current = smaller.top();
        
        Window newWindow(current);
        current.pixelToPoint(pixelBase, newWindow.base);
        newWindow.ratio /= ratioFinal;
        smaller.push(newWindow);
        
        while (!larger.empty()) larger.pop();
        
        drawMandelbrot();
        cout << newWindow.base.x << "," << newWindow.base.y << "," << newWindow.ratio << endl;
        
    }
}

void mouse(int button, int state, int x, int y)
// Function for processing mouse events.
{
    if (button == GLUT_LEFT_BUTTON)
    {
        switch (state)
        {
            case GLUT_DOWN: processLeftDown(x, y); break;
            case GLUT_UP: processLeftUp(x, y); break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        
    }
}

int main(int argc, char * argv[])
{
//     Mask floating point exceptions.
#ifdef OS_WINDOWS
        _control87( MCW_EM, MCW_EM );
#endif
    
    // Initialize glut with command line parameters.
    glutInit(&argc, argv);
    
    // Choose RGB display mode for normal screen window.
    glutInitDisplayMode(GLUT_RGB);
    
    // Set initial window size, position, and title.
    glutInitWindowSize(INITIAL_WIN_W, INITIAL_WIN_H);
    glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
    windowWidth = INITIAL_WIN_W;
    windowHeight = INITIAL_WIN_H;
    glutCreateWindow("Mandelzoom");
    
    Window currentWin(windowBotLeft, initRatio);
    smaller.push(currentWin);
    
    // You don't (yet) want to know what this does.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)INITIAL_WIN_W, 0.0, (double)INITIAL_WIN_H);
    glMatrixMode(GL_MODELVIEW);
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
    glutDisplayFunc(drawMandelbrot);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(escExit);
    glutMotionFunc(rubberBand);
    glutPassiveMotionFunc(rubberBand);
    
    glutMainLoop();
    return 0;
}




void computeCumulativeFrequency()
{
    int N = NUM_ITER;
    Point<double> c(0, 0);
    Point<int> current_pixel(0, 0);
    Window& current = smaller.top();
    unsigned int n_partitions = 100;
    double partition_size = 1.0 / n_partitions;
    unsigned long *nonCumulativeFreqs = (unsigned long *)calloc(n_partitions, sizeof(unsigned long));
    unsigned long nonCumulativeTotal = 0;
    
    int n, p;
    double intensity;
    for (int u = 0; u < windowWidth; u++)
    {
        for (int v = 0; v < windowHeight; v++)
        {
            current_pixel.x = u;
            current_pixel.y = v;
            current.pixelToPoint(current_pixel, c);
            n = Point<double>::computeIterationsInline(c, N);
            intensity = (1.0 * n) / N;
            p = (unsigned int)floor(intensity / partition_size);
            
            nonCumulativeFreqs[p] += 1;
            nonCumulativeTotal += 1;
        }
    }
    
    double* cumulativeFreqs = (double *) calloc (n_partitions, sizeof(double));
    unsigned long cumulativeTotal;
    cumulativeFreqs[0] = nonCumulativeFreqs[0];
    cumulativeTotal = nonCumulativeFreqs[0];
    for (int i = 1; i < n_partitions; i++)
    {
        cumulativeFreqs[i] = nonCumulativeFreqs[i] + cumulativeFreqs[i - 1];
    }
    for (int i = 0; i < n_partitions; i++)
    {
        cumulativeFreqs[i] /= nonCumulativeTotal;
        cout << i << "," << nonCumulativeFreqs[i];
        cout << endl;
        //        cout << "," << cumulativeFreqs[i] << endl;
    }
    free(nonCumulativeFreqs);
    free(cumulativeFreqs);
}

void writeRawToFile(const string& filename)
{
    ofstream myfile(filename, ios::out | ios::trunc);
    if (myfile.is_open())
    {
        int N = NUM_ITER;
        Point<double> c(0, 0);
        Point<int> current_pixel(0, 0);
        Window& current = smaller.top();
        int n;
        double intensity;
        int step = 5;
        for (int u = 0; u < windowWidth; u += step)
        {
            for (int v = 0; v < windowHeight; v += step)
            {
                current_pixel.x = u;
                current_pixel.y = v;
                current.pixelToPoint(current_pixel, c);
                n = Point<double>::computeIterationsInline(c, N);
                intensity = (1.0 * n) / N;
                myfile << u * windowWidth + v << "," << intensity << endl;
            }
        }
        myfile.close();
        cout << "finished writing";
    }
    else cout << "cannot open file.\n";
}
