//
//  mandelzoom.h
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#ifndef mandelzoom_h
#define mandelzoom_h

//#define INTENSITY_DISTRIBUTION_OUTPUT "./out.csv"
//#define USE_TEST_FILE_MAIN

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
#define MENU_BUTTON GLUT_RIGHT_BUTTON
#define INITIAL_WIN_W 600
#define INITIAL_WIN_H 800
#define INITIAL_WIN_X 0
#define INITIAL_WIN_Y 0

#define INITIAL_COLOR_SEED 0
#define COLOR_ITERATIONS 100

#define ZOOM_OUT_DEFAULT_SCALING 0.3
#define ZOOM_IN_DEFAULT_SCALING 2

#define NUM_ITER 500

#define INIT_PARAM \
{0.357867,0.647837, 3.52118e+06} // two trees 600*800
//{ -2, -1.25, INITIAL_WIN_W / 2.5 } // defult
//{-0.749956,-0.0529485,4.92623e+07} // bean bag
//{-0.74321,0.113349,1.2128e+06} // dark sun
//{-0.748273,-0.0544996, 7.13855e+06}; // spiral
//{-1.06236,0.260434,128835} // snowflake
//{0.360128,0.641216,7.85541e+07} // brain
//{0.360516,0.640559,1.33248e+08} // flower
//{0.360728,0.640144, 1.98057e+08} // flower'

// Color backup
//current color: (0.71,0.32,0.29)
//double I = atan(1/log(intensity+1))/M_PI*3;
//r = sin(I*PI_2X * (k1/2 +1));
//g = sin(I*PI_2X * (k2/2 +1));
//b = sin(I*PI_2X * (k3/2 +1));

//    double param[] = {0.262, 0.263, 0.002, 0.003};
//    double param[] = {0.26215, 0.26225, 0.00215, 0.00225};
//    double param[] = {-1.4014, -1.4010, -0.0002, 0.0002};
//    double param[] = {0.281, 0.291, -0.019, -0.009};

/*
 * Function Declarations
 */

/* drawing functions */
void drawPartialMandelbrot(int x0, int y0, int x1, int y1);
void drawMandelbrot();
void clearPicture();
void drawLine(int xOld, int yOld, int xNew, int yNew);
void drawRubberBand(int xA, int yA, int xS, int yS);

/* callbacks */
void rubberBand(int x, int y);
void reshape(int w, int h);
void escExit(GLubyte key, int, int);
void mainMenu(int item);
void setMenus();void processLeftDown(int x, int y);
void processLeftUp(int x, int y);
void mouse(int button, int state, int x, int y);

#endif /* mandelzoom_h */
