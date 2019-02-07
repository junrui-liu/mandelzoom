//
//  Color.hpp
//  mandelzoom
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#ifndef Color_hpp
#define Color_hpp
#define _USE_MATH_DEFINES
#include <math.h>

#define PI_2X (2*M_PI)

#define SINE_PERIOD_STEP_SIZE 0.1
#define DEFAULT_COLOR_SEED 208

class Color {
    static int triangleNumber(int n);
    
    static void cantorPairingInv2D(unsigned int z,
                                   unsigned int& x, unsigned int& y);
    
    static void cantorPairingInv3D(unsigned int z,
                                   unsigned int& a, unsigned int& b, unsigned int& c);
    
    static void cantorPairingInvNDim(unsigned int n, unsigned int z, unsigned int* arr);
    
public:
    static void getColor (unsigned int seed, double intensity,
                          double& r, double& g, double& b);
    
    static void getColor (double intensity,
                          double k1, double k2, double k3,
                          double& r, double& g, double& b);
};

#endif /* Color_hpp */
