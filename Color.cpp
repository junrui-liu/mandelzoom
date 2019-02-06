//
//  Color.cpp
//  mandelzoom
//
//  Created by Junrui Liu on 2/2/19.
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#include "Color.hpp"

using namespace std;


/*
 * Given intensity (interpreted from the escape rate), and
 * control parameters k1, k2 and k3, compute the rgb triple
 * with range [-1,1].
 */
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


/*
 * Given intensity (interpreted from the escape rate), and
 * a color seed, compute the rgb triple with range [-1,1]
 * using the inverse Cantor pairing function.
 */
void Color::getColor (unsigned int seed, double intensity, double& r, double& g, double& b)
{

    unsigned int i, j, k;
    Color::cantorPairingInv3D(seed, i, j, k);
    getColor(intensity, i, j, k, r, g, b);
}



/* !!!
 * Note: The following functions are not currently used.
 * I wrote them as an alternative to deterministically
 * traverse the Z^3 space (i.e. the possible RGB triples)
 */

/*
 * Compute the n-th triangle number, defined as
 *   T(n) = n*(n+1)/2 = sum of the sequence {1, 2, ..., n}.
 */
int Color::triangleNumber(int n)
{   return n * (n + 1) /2;   }


/*
 * The inverse of the famous Cantor pairing function \pi.
 * Given a natural number z, computes a pair (x,y) such that
 *   \pi(x,y) = z.
 * For an intuitive derivation and proof, see the SO answer
 * https://math.stackexchange.com/a/222835
 */
void Color::cantorPairingInv2D(unsigned int z, unsigned int& x, unsigned int& y)
{
    int n = floor( (-1 + sqrt(8.0* z + 1))/2.0 );
    y = z - triangleNumber(n);
    x = n - y;
}


/*
 * The inverse of the Cantor pairing function generalized to Z -> Z^3.
 */
void Color::cantorPairingInv3D(unsigned int z,
                               unsigned int& a, unsigned int& b, unsigned int& c)
{
    unsigned int tmp;
    Color::cantorPairingInv2D(z, a, tmp);
    Color::cantorPairingInv2D(tmp, b, c);
}


/*
 * The generalized inverse Cantor pairing function from Z to Z^n
 * for any natural number n >= 2.
 */
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
