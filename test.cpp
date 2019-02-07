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
//#include "Window.h"
#include "Point.h"
//#include "Color.hpp"
//#include "mandelzoom.h"

using namespace std;

#define USE_TEST_FILE_MAIN
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
    Complex z1 (1,0);
    Complex zi (0,1);
    Complex z0 (0,0);
    cout << z0 << endl;
    cout << z1 * zi << endl;
}

#endif /* USE_TEST_FILE_MAIN */
