//
//  test.cpp
//  mandelzoom
//
//  Created by Junrui Liu on 2/2/19.
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include "Window.h"
#include "Point.h"
#include "Color.hpp"
#include "mandelzoom.h"

using namespace std;

#ifdef USE_TEST_FILE_MAIN



template <class T>
void printArray(int size, T arr[]) {
    cout << "[";
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << ",";
    }
    cout << "]" << endl;
}

int main(int argc, char * argv[])
{
    int dim = 3;
    unsigned int arr[dim];
    for (int i = 0; i < 1000; i++)
    {
        Color::cantorPairingInvNDim(dim, i, arr);
        cout << i << ":";
        printArray<unsigned int>(dim, arr);
    }
}
#endif /* USE_TEST_FILE_MAIN */
