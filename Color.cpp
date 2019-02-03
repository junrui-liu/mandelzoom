//
//  Color.cpp
//  mandelzoom
//
//  Created by work on 2/2/19.
//

#include "Color.hpp"

using namespace std;

void Color::getColor (double intensity,
                      double k1, double k2, double k3,
                      double& r, double& g, double& b)
{
    if (intensity == 1)
    {   r = 0; g = 0; b = 0;    }
    else
    {
        double I = atan(1/log(intensity+1))/M_PI*3;
        r = sin(I*PI_2X * (k1/2 +1));
        g = sin(I*PI_2X * (k2/2 +1));
        b = sin(I*PI_2X * (k3/2 +1));
    }
}

void Color::getColor (unsigned int seed, double intensity, double& r, double& g, double& b)
{

    unsigned int i, j, k;
    Color::cantorPairingInv3D(seed, i, j, k);
    getColor(intensity, i, j, k, r, g, b);
}

int Color::triangleNumber(int n)
{   return n * (n + 1) /2;   }

void Color::cantorPairingInv2D(unsigned int z, unsigned int& x, unsigned int& y)
{
    int n = floor( (-1 + sqrt(8.0* z + 1))/2.0 );
    y = z - triangleNumber(n);
    x = n - y;
}

void Color::cantorPairingInv3D(unsigned int z,
                               unsigned int& a, unsigned int& b, unsigned int& c)
{
    unsigned int tmp;
    Color::cantorPairingInv2D(z, a, tmp);
    Color::cantorPairingInv2D(tmp, b, c);
}

void Color::cantorPairingInvNDim(unsigned int n, unsigned int z, unsigned int* arr)
{
    unsigned int x, y;
    for (int i = 0; i < n-1; i++)
    {
        cantorPairingInv2D(z, x, y);
        arr[i] = x;
        z = y;
    }
    arr[n-1] = z;
}


