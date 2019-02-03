//
//  Window.h
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#ifndef Window_h
#define Window_h

#include "Point.h"
#include <iostream>

/*
 * Class: Window
 */
class Window {
public:
    Point<double> base;
    double ratio; // unit is pixel/real
    
    Window();
    Window(Point<double>& p, double r);
    /* copy constructor */
    Window(const Window& w);
    
    void center(int w, int h, Point<int>& center) const;
    
    void pixelToPoint(const Point<int>& pixel,
                      Point<double>& point) const;
    
    void scaleWrtPivot(double factor, Point<int>& pivotPixel,
                       Window& newW) const;
    
    void scaleWrtCenter(double factor, int w, int h,
                        Window& newWindow) const;
    
    /* overloaded methods */
    friend std::ostream& operator<< (std::ostream& out, const Window& window);
};

#endif /* Window_h */
