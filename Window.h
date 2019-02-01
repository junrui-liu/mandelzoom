//
//  Window.h
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#ifndef Window_h
#define Window_h

#include "Point.h"

/*
 * Class: Window
 */
class Window {
public:
    Point<double> base;
    double ratio;
    
    Window(Point<double>& p, double r);
    /* copy constructor */
    Window(const Window& w);
    
    void pixelToPoint(const Point<int>& pixel,
                      Point<double>& point) const;
};

#endif /* Window_h */
