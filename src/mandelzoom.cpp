//
//  mandelzoom.cpp
//  mandelzoom
//
//  Adapted from rubber-band.cpp written by Professor Ellman
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <stack>
#include <iostream>
#include <fstream>
#include <random>
#include "mandelzoom.h"
#include "Window.h"
#include "Point.h"
#include "Color.hpp"
#include <assert.h>

using namespace std;

/*----------------------------------------------
 *               Global States
 *---------------------------------------------*/
bool init = true;

// Stacks for undoing & redoing previous zooms
stack<Window*> zoomOut;
stack<Window*> zoomIn;

Window* currentWindow;

// Variable for use in rubberbanding.
int xAnchor, yAnchor, xStretch, yStretch;
bool rubberBanding = false;

// Variables for keeping track of the screen window dimensions.
int windowHeight = INITIAL_WIN_W, windowWidth = INITIAL_WIN_H;

// Variables for keeping track of the color paramters triple.
const double initialColorParams[] = INITIAL_COLOR_PARAMS;
double colorParam1, colorParam2, colorParam3;

// Non-deterministic generator.
random_device rd;
mt19937 gen(rd());


/*----------------------------------------------
 *            Function Definitions
 *---------------------------------------------*/

/*
 * Clear the screen and redraw the image.
 */
void drawMandelbrot()
{
	cout << "Window parameters: <internal> " << *currentWindow <<
		" / <standard> ";
	printInternalToConsoleArgs(currentWindow);
	cout << endl;
    cout << "Color parameters: {" << colorParam1 << ","
         << colorParam2 << "," << colorParam3 << "}" << endl;
    drawPartialMandelbrot(0, 0, windowWidth, windowHeight);
}


/*
 * Draw a partial image bounded by a rectangle
 * with base point (x0,y0) and diagonal (x1,y1).
 */
void drawPartialMandelbrot(int x0, int y0, int x1, int y1)
{
    int x_tmp = min(x0, x1);
    x0 = x_tmp;
    x1 = x_tmp ^ x0 ^ x1;
    
    int y_tmp = min(y0, y1);
    y0 = y_tmp;
    y1 = y_tmp ^ y0 ^ y1;

    // n is the # of iterations before a sequence diverges
    unsigned int N = NUM_ITER, n = 0;
    Point<int> current_pixel;
    Point<double> c; // the complex number corresponding to current_pixel
    double r, g, b; // colors
    
    // draw some number of columns before flushing the screen
    int flush_max_stride = FLUSH_PERIOD;
    
    for (int y = y1, stride = 0; y >= y0; y -= 1, stride += 1) // row first
    //for (int x = x0, stride = 0; x <= x1; x += 1, stride += 1) // col first
    {
        if (stride == flush_max_stride)
        {
            stride = 0;
            glEnd();
            glFlush();
        }
        if (stride == 0)
            glBegin(GL_POINTS);
        for (int x = x0; x <= x1; x += 1) // row first
        //for (int y = y0; y <= y1; y += 1) // col first
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
 * Callback for processing reshape events.
 * This version preserves the center of the current window.
 * Adapted from Prof. Ellman's example code.
 */
void reshapePreserveCenter(int w, int h)
{
    Window* newWindow = new Window();
    currentWindow->scalePreserveCenter(1.0, w, h, *newWindow);
    free(currentWindow);
    currentWindow = newWindow;
}


/*
 * Callback for processing reshape events.
 * This version preserves the base (bottom left point).
 * Adapted from Prof. Ellman's example code.
 */
void reshapePreserveBase(int w, int h)
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
    
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}


/*
 * Callback for processing keyboard events.
 * Adapted from Prof. Ellman's example code.
 */
void keyboard(GLubyte key, int x, int y)
{
    int mod = 1.0 / COLOR_PARAM_GRANULARITY;
    switch (key)
    {
        case KEY_ZOOM_IN: zoomFunc(MENU_ZOOM_IN); break;
        case KEY_ZOOM_OUT: zoomFunc(MENU_ZOOM_OUT); break;
        case KEY_SHUFFLE_COLOR:
            colorParam1 = 1.0 * (gen() % mod) / mod;
            colorParam2 = 1.0 * (gen() % mod) / mod;
            colorParam3 = 1.0 * (gen() % mod) / mod;
            drawMandelbrot();
            break;
        case KEY_RESET_COLOR:
            colorParam1 = initialColorParams[0];
            colorParam2 = initialColorParams[1];
            colorParam3 = initialColorParams[2];
            drawMandelbrot();
            break;
        case KEY_ESC:
            if (rubberBanding)
            {
                drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
                rubberBanding = false;
            }
            else
            {
                std::exit(0);
            }
            break;
    }
}


void zoomFunc(menu_t choice)
{
    assert(choice == MENU_ZOOM_OUT || choice == MENU_ZOOM_IN);
    
    stack<Window*> *zoomStack; // stack of choice
    stack<Window*> *zoomOther; // the other stack
    double zoomDefaultFactor;
    
    if (choice == MENU_ZOOM_OUT)
    {
        zoomStack = &zoomOut;
        zoomOther = &zoomIn;
        zoomDefaultFactor = ZOOM_OUT_DEFAULT_SCALING;
    }
    else
    {
        zoomStack = &zoomIn;
        zoomOther = &zoomOut;
        zoomDefaultFactor = ZOOM_IN_DEFAULT_SCALING;
    }
    
    Window* newWindow;
    if (!zoomStack->empty()) // undo the previous zoom
    {
        newWindow = zoomStack->top();
        zoomStack->pop();
    }
    else // no previous action; zoom by default value
    {
        newWindow = new Window(*currentWindow);
        currentWindow->
        scalePreserveCenter(zoomDefaultFactor, windowWidth, windowHeight, *newWindow);
    }
    zoomOther->push(currentWindow);
    currentWindow = newWindow;
    drawMandelbrot();
}


/*
 * Callback for processing main menu.
 * Adapted from Prof. Ellman's example code.
 */
void mainMenu(int item)
{
    menu_t choice = (menu_t) item;
    switch (choice)
    {
        case MENU_ZOOM_OUT:
        case MENU_ZOOM_IN: zoomFunc(choice); break;
        case MENU_EXIT: std::exit(0);
    }
}


/*
 * Function for creating menus.
 * Adapted from Prof. Ellman's example code.
 */
void setMenus()
{
    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Zoom Out", MENU_ZOOM_OUT);
    glutAddMenuEntry("Zoom In", MENU_ZOOM_IN);
    glutAddMenuEntry("Exit", MENU_EXIT);
    glutAttachMenu(MENU_BUTTON);
}


/*
 * Draw a rectangular rubber band with anchor (xA, yA)
 * and diagonal (xS, yS).
 * Taken from Prof. Ellman's example code.
 */
void drawRubberBand(int xA, int yA, int xS, int yS)
{
    glColor3f(1.0,1.0,1.0);
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
    {   // undo the previous drawing of the rubber band using XOR
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        y = windowHeight - y;
        xStretch = x;
        yStretch = y;
        // draw the new rubber band
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        glFlush();
    }
}


/*
 * Function for processing mouse left botton down events.
 * Adapted from Prof. Ellman's example code.
 */
void processLeftDown(int x, int y)
{
    if (!rubberBanding)
    {
        y = windowHeight - y;
        xAnchor = x;
        yAnchor = y;
        xStretch = x;
        yStretch = y;
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        rubberBanding = true;
    }
}

/*
 * Function for processing mouse left botton up events.
 * Adapted from Prof. Ellman's example code.
 */
void processLeftUp(int x, int y)
{
    if (rubberBanding)
    {
        drawRubberBand(xAnchor, yAnchor, xStretch, yStretch);
        rubberBanding = false;
        y = windowHeight - y; // convert from GLUT coord sys to OpenGL
        
        // anchor can be anywhere relative to stretch
        int xMin = min(xAnchor, x);
        int yMin = min(yAnchor, y);
        int xMax = max(xAnchor, x);
        int yMax = max(yAnchor, y);
        int rbWidth = xMax - xMin;
        int rbHeight = yMax - yMin;
        
        // ratio between rubber band width (& height) and current window width (& height)
        double ratioWidth = (1.0 * rbWidth) / windowWidth;
        double ratioHeight = (1.0 * rbHeight) / windowHeight;
        // choose the ratio for which the rb width/height
        //   is closest to the current window width/height
        double ratioBetterFit = max(ratioWidth, ratioHeight);
        // adjust rb width/height if it is not chosen above
        if (ratioWidth > ratioHeight)
        {
            rbHeight = windowHeight * ratioWidth;
        }
        else if (ratioHeight > ratioWidth)
        {
            rbWidth = windowWidth * ratioHeight;
        }
        
        // Pixel coord of the new base point (bottom left corner of the rb window).
        // My strategy is to preserve the center of the rb window,
        //   which will become the center of the new display window.
        Point<int> pixelBase;
        pixelBase.x = (xMax + xMin - rbWidth) / 2;
        pixelBase.y = (yMax + yMin - rbHeight) / 2;
        
        Window* newWindow = new Window(*currentWindow); // copy current to new
        currentWindow->pixelToPoint(pixelBase, newWindow->base); // relocate the base point
        newWindow->ratio /= ratioBetterFit; // recalibrate the scaling factor
        zoomOut.push(currentWindow); // the current window will be the next zoom-out window
        currentWindow = newWindow;
        
        // don't forget to empty the zoom-in stack!
        // The reason is that he stuff left on the zoom-in stack
        // might be larger than our rb window, which would
        // violate the stack ordering invariant.
        while (!zoomIn.empty())
        {
            free(zoomIn.top());
            zoomIn.pop();
        }
        drawMandelbrot();
    }
}


/*
 * Function for processing mouse events.
 * Adapted from Prof. Ellman's example code.
 */
void mouse(int button, int state, int x, int y)
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


Window* convertConsoleArgsToInternal(double x0, double x1, double y0, double y1, int w, int h)
{
	Window* converted = new Window();
	assert (x0 < x1);
	assert (y0 < y1);
	assert(w > 0 && h > 0);
	double baseX = min(x0, x1);
	double baseY = min(y0, y1);
	double otherX = max(x0, x1);
	double otherY = max(y0, y1);
	converted->ratio = min(w/(otherX - baseX), h/(otherY - baseY));
	converted->base.x = baseX;
	converted->base.y = baseY;
	return converted;
}

void printInternalToConsoleArgs(Window* win)
{
	double w, h, x0, y0, x1, y1;
	w = windowWidth;
	h = windowHeight;
	x0 = win->base.x;
	y0 = win->base.y;
	x1 = x0 + w / win->ratio;
	y1 = y0 + h / win->ratio;
	cout << x0 << " " << x1 << " " << y0 << " " << y1 << " " << w << " " << h;
}

/*
 * Main function.
 * Adapted from Prof. Ellman's example code.
 */
#ifndef USE_TEST_FILE_MAIN
int main(int argc, char * argv[])
{
//     Mask floating point exceptions in Windows.
#ifdef OS_WINDOWS
        _control87( MCW_EM, MCW_EM );
#endif
// Initialize glut with command line parameters. Not necessary on Windows.
#ifdef OS_MACOS
    glutInit(&argc, argv);
#endif

    // Choose RGB display mode for normal screen window.
    glutInitDisplayMode(GLUT_RGB);
    
    // Set initial window size, position, and title.
#ifdef CONSOLE_VER
	double x0, x1, y0, y1;
	int w, h;
	sscanf_s(argv[1], "%lf", &x0);
	sscanf_s(argv[2], "%lf", &x1);
	sscanf_s(argv[3], "%lf", &y0);
	sscanf_s(argv[4], "%lf", &y1);
	sscanf_s(argv[5], "%d", &w);
	sscanf_s(argv[6], "%d", &h);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
	windowWidth = w;
	windowHeight = h;
	Window* initWindow = convertConsoleArgsToInternal(x0, x1, y0, y1, w, h);
#else
	glutInitWindowSize(INITIAL_WIN_W, INITIAL_WIN_H);
	glutInitWindowPosition(INITIAL_WIN_X, INITIAL_WIN_Y);
	windowWidth = INITIAL_WIN_W;
	windowHeight = INITIAL_WIN_H;
	const double initialWindowParams[] = INITIAL_WINDOW_PARAMS;
	Point<double> initialWindowBase(initialWindowParams[0], initialWindowParams[1]);
	double initWindowRatio = initialWindowParams[2];
	Window* initWindow = new Window(initialWindowBase, initWindowRatio);
#endif
	currentWindow = initWindow;
    glutCreateWindow("Mandelzoom");

    colorParam1 = initialColorParams[0];
    colorParam2 = initialColorParams[1];
    colorParam3 = initialColorParams[2];
    
    // You don't (yet) want to know what this does.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)INITIAL_WIN_W, 0.0, (double)INITIAL_WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* The following code is found in Prof. Ellman's example code,
     * which does not seem to affect the behavior of the program.
     */
    //    glTranslatef( 0.375, 0.375, 0.0 );
    
    // Set up the menus.
    setMenus();
    
    // Set the callbacks for the normal screen window.
    glutDisplayFunc(drawMandelbrot);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshapePreserveBase);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(rubberBand);
    glutPassiveMotionFunc(rubberBand);
    
    glutMainLoop();
    return 0;
}
#endif /* USE_TEST_FILE_MAIN */
