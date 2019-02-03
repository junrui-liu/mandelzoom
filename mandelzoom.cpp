/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <stack>
#include "mandelzoom.h"
#include "Window.h"
#include "Point.h"
#include "Color.hpp"
#include <iostream>
#include <fstream>

using namespace std;

/*----------------------------------------------
 *              Initialization
 *---------------------------------------------*/

const double initialWindowParams[] = INITIAL_WINDOW_PARAMS;
Point<double> initialWindowBase(initialWindowParams[0], initialWindowParams[1]);
double initWindowRatio = initialWindowParams[2];

const double initialColorParams[] = INITIAL_COLOR_PARAMS;
double colorParam1 = initialColorParams[0],
colorParam2 = initialColorParams[1],
colorParam3 = initialColorParams[2];


/*----------------------------------------------
 *                  States
 *---------------------------------------------*/
bool init = true;
stack<Window*> zoomOut;
stack<Window*> zoomIn;
Window* currentWindow;

// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight = INITIAL_WIN_W, windowWidth = INITIAL_WIN_H;


/*----------------------------------------------
 *            Function Definitions
 *---------------------------------------------*/
void computeCumulativeFrequency()
{
    int N = NUM_ITER;
    Point<double> c;
    Point<int> current_pixel;
    Window& current = *zoomOut.top();
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
        Point<double> c;
        Point<int> current_pixel;
        Window& current = *zoomOut.top();
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

/*
 * Clear the screen and redraw the image.
 */
void drawMandelbrot()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPartialMandelbrot(0, 0, windowWidth, windowHeight);
    //    computeCumulativeFrequency();
#ifdef INTENSITY_DISTRIBUTION_OUTPUT
    writeRawToFile(OUTPUT_FILE);
#endif
}

/*
 * Draw a partial image bounded by a rectangle
 * with base point (x0,y0) and diagonal (x1,y1).
 * Assumes:
 *   0 <= x0 <= x1
 *   0 <= y0 <= y1
 */
void drawPartialMandelbrot(int x0, int y0, int x1, int y1)
{
    // n is the # of iterations before a sequence diverges
    unsigned int N = NUM_ITER, n = 0;
    Point<int> current_pixel;
    Point<double> c; // the complex number corresponding to current_pixel
    double r, g, b; // colors
    
    // draw some number of columns before flushing the screen
    int flush_max_stride = FLUSH_EVERY_NTH_COL;
    
    for (int x = x0, stride = flush_max_stride; x <= x1; x += 1, stride += 1)
    {
        if (stride == flush_max_stride)
        {
            stride = 0;
            glEnd();
        }
        if (stride == 0)
            glBegin(GL_POINTS);
        for (int y = y0; y <= y1; y += 1)
        {
            current_pixel.x = x;
            current_pixel.y = y;
            currentWindow->pixelToPoint(current_pixel, c);
            n = Point<double>::computeIterationsInline(c, N);
            
            Color::getColor(1.0*n/N, colorParam1, colorParam2, colorParam3, r, g, b);
            
            glColor3f(r, g, b);
            glVertex2i(x, y);
        }
        
    }
    glEnd();
    glFlush();
}


/*
 * Clear the color buffer.
 * Taken from Prof. Ellman's example code.
 */
void clearPicture()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

/*
 * Draw a line from (x0,y0) to (x1,y1)
 * Taken from Prof. Ellman's example code.
 */
void drawLine(int x0, int y0, int x1, int y1)
{
    glBegin(GL_LINES);
    glVertex2i(x0, y0);
    glVertex2i(x1, y1);
    glEnd();
    glFlush();
}

/*
 * Draw a rectangular rubber band with anchor (xA, yA)
 * and diagonal (xS, yS).
 * Taken from Prof. Ellman's example code.
 */
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


/*
 * Callback for processing mouse motion.
 * Taken from Prof. Ellman's example code.
 */
void rubberBand(int x, int y)
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

/*
 * Callback for processing reshape events.
 * Adopted from Prof. Ellman's example code.
 */
void reshape(int w, int h)
{
    if (!init)
    {
        // re-draw only the protruding rectangles, if any
        currentWindow->base.y += (windowHeight - h) / currentWindow->ratio;
        if (w > windowWidth)
        {
            drawPartialMandelbrot(windowWidth, w, 0, h);
            if (h > windowHeight)
                drawPartialMandelbrot(0, windowWidth, windowHeight, h);
        }
        else if (h > windowHeight) // symmetric to above
        {
            drawPartialMandelbrot(0, w, windowHeight, h);
            if (h > windowHeight)
                drawPartialMandelbrot(windowWidth, w, 0, windowHeight);
        }
        windowWidth = w;
        windowHeight = h;
    }
    init = false;
    
    // TODO: do not re-render the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}


/*
 * Callback for processing keyboard events.
 * Adopted from Prof. Ellman's example code.
 */
void keyboard(GLubyte key, int, int)
{
    int mod = 1.0 / COLOR_PARAM_GRANULARITY;
    switch (key)
    {
        case 27: // esc
            std::exit(0); break;
        case 32: // space
            colorParam1 = 1.0 * (random() % mod) / mod;
            colorParam2 = 1.0 * (random() % mod) / mod;
            colorParam3 = 1.0 * (random() % mod) / mod;
            break;
        case 67: // char 'C'
            colorParam1 = initialColorParams[0];
            colorParam2 = initialColorParams[1];
            colorParam3 = initialColorParams[2];
            break;
    }
    drawMandelbrot();
}



void mainMenu(int item)
// Callback for processing main menu.
{
    Window* newWindow;
    switch (item)
    {
        case 1: // zoomOut
            if (!zoomOut.empty())
            {
                newWindow = zoomOut.top();
                zoomOut.pop();
            }
            else
            {
                newWindow = new Window(*currentWindow);
                currentWindow->
                scaleWrtCenter(ZOOM_OUT_DEFAULT_SCALING,
                               windowWidth, windowHeight, *newWindow);
            }
            zoomIn.push(currentWindow);
            currentWindow = newWindow;
            drawMandelbrot();
            break;
        case 2: // zoomIn
            if (!zoomIn.empty())
            {
                newWindow = zoomIn.top();
                zoomIn.pop();
            }
            else
            {
                newWindow = new Window(*currentWindow);
                currentWindow->
                scaleWrtCenter(ZOOM_IN_DEFAULT_SCALING,
                               windowWidth, windowHeight, *newWindow);
            }
            zoomOut.push(currentWindow);
            currentWindow = newWindow;
            drawMandelbrot();
            break;
        case 3: // exit
            std::exit(0);
    }
}

void setMenus()
// Function for creating menus.
{
    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Zoom Out", 1);
    glutAddMenuEntry("Zoom In", 2);
    glutAddMenuEntry("Exit", 3);
    glutAttachMenu(MENU_BUTTON);
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
        
        Point<int> pixelBase;
        pixelBase.x = (xMax + xMin - newWidth) / 2;
        pixelBase.y = (yMax + yMin - newHeight) / 2;
        
        Window* newWindow = new Window(*currentWindow);
        currentWindow->pixelToPoint(pixelBase, newWindow->base);
        newWindow->ratio /= ratioFinal;
        zoomOut.push(currentWindow);
        currentWindow = newWindow;
        
        while (!zoomIn.empty())
        {
            free(zoomIn.top());
            zoomIn.pop();
        }
        
        drawMandelbrot();
        cout << *newWindow << endl;
        
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
}

#ifndef USE_TEST_FILE_MAIN
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
    
    Window* initWindow = new Window(initialWindowBase, initWindowRatio);
    currentWindow = initWindow;
    
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
    glutKeyboardFunc(keyboard);
    glutMotionFunc(rubberBand);
    glutPassiveMotionFunc(rubberBand);
    
    glutMainLoop();
    return 0;
}
#endif /* USE_TEST_FILE_MAIN */
