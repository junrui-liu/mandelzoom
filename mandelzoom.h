//
//  mandelzoom.h
//  mandelzoom
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#ifndef mandelzoom_h
#define mandelzoom_h
#include "Window.h"


/*----------------------------------------------------
 *                   Settings
 *---------------------------------------------------*/
#define EXTRA_CREDIT_VER // enable this and disable CONSOLE_VER to see my submission of the extra credit img
//#define CONSOLE_VER    // enable this to run with console arguments

#ifdef EXTRA_CREDIT_VER
#define INITIAL_WINDOW_PARAMS IMG_EXTRA_CREDIT

// display the complete mandelbrot set by default
// You might want to change IMG_COMPLETE to one of the predefined images (see the corresponding section below)
#else 
#define INITIAL_WINDOW_PARAMS IMG_COMPLETE 
#endif

// Enable one of the following depending on your OS
//#define OS_WINDOWS
#define OS_MACOS
//#define OS_LINUX

// Trigger the menu via right/middle button
//#define MENU_BUTTON GLUT_RIGHT_BUTTON
#define MENU_BUTTON GLUT_MIDDLE_BUTTON

// set the default window width/height for non-extra-credit ver.
#define DEFAULT_W 800
#define DEFAULT_H 800


/*----------------------------------------------------
 *                   Constants
 *---------------------------------------------------*/

#ifdef OS_MACOS
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

#define EXTRA_CREDIT_W 1600
#define EXTRA_CREDIT_H 900

#ifdef EXTRA_CREDIT_VER
#define INITIAL_WIN_W EXTRA_CREDIT_W
#define INITIAL_WIN_H EXTRA_CREDIT_H
#else // use default width and height
#define INITIAL_WIN_W DEFAULT_W
#define INITIAL_WIN_H DEFAULT_H
#endif

#define INITIAL_WIN_X 10
#define INITIAL_WIN_Y 10
#define EXTRA_CREDIT_ITER 520  // please don't change this in the extra credit version
#ifdef EXTRA_CREDIT_VER
#define NUM_ITER EXTRA_CREDIT_ITER
#else
#define NUM_ITER 700 // default number of iterations
#endif
#define FLUSH_PERIOD 10

#define ZOOM_OUT_DEFAULT_SCALING 0.5
#define ZOOM_IN_DEFAULT_SCALING 2
#define COLOR_PARAM_GRANULARITY 0.01

enum menu_t {
    MENU_ZOOM_OUT,
    MENU_ZOOM_IN,
    MENU_EXIT,
};

#define KEY_ESC 27
#define KEY_ZOOM_OUT 'o'
#define KEY_ZOOM_IN 'i'
#define KEY_SHUFFLE_COLOR ' '
#define KEY_RESET_COLOR 'c'


/*----------------------------------------------------
 *                 Predefined Images
 *---------------------------------------------------*/
// The following parameters are represented using internal representation (x0, y0, scaling),
// not in the format of console input paramters (x0, x1, y0, y1, w, h).

#define IMG_EXTRA_CREDIT {.36037937, .64080188, 3.82747e+08}
#define IMG_COMPLETE { -2, -1.25, INITIAL_WIN_W / 2.5 }
#define IMG_INTEGRAL_SIGN {-0.777222,0.135332,,954472}
#define IMG_TREES {0.357867,0.647837, 3.52118e+06} // best viewed in 600*800
#define IMG_BEAN_BAG {-0.749956,-0.0529485,4.92623e+07} // bean bag
#define IMG_DARK_SUN {-0.74321,0.113349,1.2128e+06} // dark sun
#define IMG_SPIRAL {-0.748273,-0.0544996, 7.13855e+06}; // spiral
#define IMG_SNOWFLAKE {-1.06236,0.260434,128835} // snowflake
#define IMG_BRAIN {0.360128,0.641216,7.85541e+07} // brain
#define IMG_FLOWER {0.3605162,0.6405592,1.30296e+08} // flower
#define IMG_FLOWER_2 {0.360728,0.640144, 1.98057e+08} // flower'
#define IMG_TREASURE_LIES_HERE {0.352861,0.638862, 73178.8} // interesting region

/* Below are standard images suggested in the homework handout in internal representation */
#define IMG_STANDARD_1 {0.262, 0.002, 400000};    // 0.262   0.263   0.002   0.003
#define IMG_STANDARD_2 {0.26215, 0.00215, 4e+06}; // 0.26215 0.26225 0.00215 0.00225 
#define IMG_STANDARD_3 {-1.4014, -0.0002, 1e+06}; // -1.4014 -1.4010 -0.0002 0.0002
#define IMG_STANDARD_4 {0.281, 0.291, 40000};     // 0.281   0.291   -0.019  -0.009


/*----------------------------------------------------
 *            Interesting Color Parameters
 *---------------------------------------------------*/

/* The paramters below are used in this set of expressions:
 *    double I = atan(1/log(intensity+1))/M_PI*3;
 *    r = sin(I*PI_2X * (k1/2 +1));
 *    g = sin(I*PI_2X * (k2/2 +1));
 *    b = sin(I*PI_2X * (k3/2 +1));
 */

#ifdef EXTRA_CREDIT_VER
#define INITIAL_COLOR_PARAMS COLOR_EXTRA_CREDIT_OCEAN
#else
#define INITIAL_COLOR_PARAMS COLOR_BRIGHT
#endif
#define COLOR_EXTRA_CREDIT_OCEAN {.63, .37, .28} // ocean
#define COLOR_BRIGHT {0.2,0.12,0.05}
#define COLOR_HIGH_CONTRAST {0.942,0.18,0.344} // high contrast
#define COLOR_PHOSPHORUS {0.294,0.356,0.141} // phosphorus
#define COLOR_IRON {0.51,0.61,0.62} // iron
#define COLOR_HYDROGEN {0.98,0.55,0.48} // hydrogen
#define COLOR_CHROMOSHPRE {0.13,0.35,0.44} // sun
#define COLOR_JADE {0.71,0.32,0.29} // jade
#define COLOR_LAVA {0.14,0.32,0.67} // lava
#define COLOR_LAVA2 {0.29,0.5,0.68} // lava 2
#define COLOR_CALM_RED {0.54,0.8,0.95} // calm red
#define COLOR_MICROBE {0.29,0.14,0.67} // microbe
#define COLOR_TOXIC {0.16,0.22,0.13} // toxic
#define COLOR_YELLOW {0.3,0.34,0.54} // yellow
#define COLOR_GREEN {0.73,0.37,0.94} // green
#define COLOR_SPRING {0.41,0.33,0.81} // spring
#define COLOR_WINTER {0.28,0.26,0.23} // winter
#define COLOR_SNOW {0.114,0.089,0.025}
#define COLOR_FADED_BOOK {0.41,0.44,0.51}
#define COLOR_MILK {0.38,0.18,0.12}
#define COLOR_DRY {0.658,0.547,0.424}
#define COLOR_FADED_SPACE {0.56,0.46,0.4} // faded space
#define COLOR_FADED_SPACE_2 {0.527,0.399,0.276} // faded space 2
#define COLOR_SPACE {0.556,0.471,0.31} // space
#define COLOR_SPACE_2 {0.452,0.319,0.157} // space 2
#define COLOR_SPACE_3 {0.823,0.729,0.678} // space 3
#define COLOR_SPACE_4 {0.775,0.628,0.45} // space 3
#define COLOR_LUCID {0.722,0.807,0.666} // lucid
#define COLOR_LIGHT_BLUE {0.962,0.636,0.55} // light blue
#define COLOR_COLD_FIRE {0.647,0.863,0.863} // cold fire
#define COLOR_GOLDEN_SLUMBER {0.248,0.251,0.421} // golden slumber
#define TEST {0.59, 0.47, 0.21}


/*----------------------------------------------------
 *             Function Declarations
 *---------------------------------------------------*/

/* drawing & zooming functions */
void drawPartialMandelbrot(int x0, int y0, int x1, int y1);
void drawMandelbrot();
void drawRubberBand(int xA, int yA, int xS, int yS);
void zoomFunc(menu_t choice);
Window* convertConsoleArgsToInternal(double x0, double y0, double x1, double y1, int w, int h);
void printInternalToConsoleArgs(Window* win);

/* callbacks */
void rubberBand(int x, int y);
void keyboard(GLubyte key, int, int);
void mainMenu(int item);
void setMenus();
void processLeftDown(int x, int y);
void processLeftUp(int x, int y);
void mouse(int button, int state, int x, int y);

#endif /* mandelzoom_h */
