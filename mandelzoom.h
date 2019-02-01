//
//  mandelzoom.h
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#ifndef mandelzoom_h
#define mandelzoom_h

#include "Window.h"
#include "Point.h"

/* control target OS */
// define OS_WINDOWS
#define OS_MACOS

#ifdef OS_WINDOWS
    #include <Windows.h>
    #include <GL/glut.h>
#endif
#ifdef OS_MACOS
    #include <GLUT/glut.h>
#endif

// Defining default values for window size, location, and more.
#define INITIAL_WIN_W 800
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 0
#define INITIAL_WIN_Y 0

#define NUM_ITER 500
#define PI 3.14159265358979323846
#define PI_2 (2*PI)
//#define OUTPUT_FILE "./out.csv"

//#define INIT_PARAM { -2, -1.25, INITIAL_WIN_W / 2.5 };
//#define INIT_PARAM {-1.06236,0.260434,128835};
#define INIT_PARAM {-0.748273,-0.0544996, 7.13855e+06};
//#define INIT_PARAM {-0.749956,-0.0529485,4.92623e+07}
//#define INIT_PARAM {-0.74321,0.113349,1.2128e+06};
//#else
//    double param[] = {-2, 0.5, -1.25, 1.25};
//    double param[] = {0.262, 0.263, 0.002, 0.003};
//    double param[] = {0.26215, 0.26225, 0.00215, 0.00225};
//    double param[] = {-1.4014, -1.4010, -0.0002, 0.0002};
//    double param[] = {0.281, 0.291, -0.019, -0.009};
//Point<double> windowBotLeft(param[0], param[2]);
//Point<double> windowTopRight(param[1], param[3]);
//#endif

/*
 * Function Declarations
 */
void assignColor(double intensity, double& r, double& g, double& b);
void drawPartialMandelbrot(int x0, int y0, int x1, int y1);
void drawMandelbrot();
void clearPicture();
void drawLine(int xOld, int yOld, int xNew, int yNew);
void drawRubberBand(int xA, int yA, int xS, int yS);
void rubberBand(int x, int y);
void reshape(int w, int h);
void escExit(GLubyte key, int, int);
void mainMenu(int item);
void setMenus();void processLeftDown(int x, int y);
void processLeftUp(int x, int y);
void mouse(int button, int state, int x, int y);

#endif /* mandelzoom_h */
