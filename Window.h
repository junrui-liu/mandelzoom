//
//  Window.h
//  mandelzoom
//
//  Created by Junrui Liu on 1/31/19.
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#ifndef Window_h
#define Window_h

#include "Point.h"
#include <iostream>

/*
 * Keep track of the state of the current window.
 *
 * Each `Window` object consists of
 *   - `base`: a point in the real plane, corresponding to
 *     the bottom left pixel of the current window
 *   - `ratio`: the scaling factor from the pixel coord to the real plane.
 *     The unit is pixel/real. Intuitively, we need `ratio` number of pixels
 *     to display a unit of length in the real plane.
 *
 * For example, if we have a window of size 300x400 that
 * displays a rectangle in the real plane with diagonal points
 * (10,10) and (70,90), then:
 *   - `base` = (10, 10), corresponding to the pixel <0,0> of the window.
 *   - `ratio` = 300/(70-10) = 400/(90-10) = 5.0 .
 *
 * The reason that I did not adopt the representation scheme
 * suggested in the handout is that
 *   1) scaling becomes almost trivial, and
 *   2) we never have to worry about the discrepancy between
 *      the aspect ratio of the diagonal points (x0,y0) & (x1,y1),
 *      and that of the window width & height. Thus a pixel will
 *      never be ``stretched'' in this representation scheme.
 */
class Window {

public:
    Point<double> base;
    double ratio;
    
    /* constructors */
    Window();
    Window(Point<double>& p, double r);
    Window(const Window& w); // copy constructor
    
    void center(int w, int h, Point<int>& center) const;
    
    void pixelToPoint(const Point<int>& pixel,
                      Point<double>& point) const;
    
    void scalePreservePivot(double factor, Point<int>& pivotPixel,
                       Window& newW) const;
    
    void scalePreserveCenter(double factor, int w, int h,
                        Window& newWindow) const;
    
    /* overloaded methods */
    friend std::ostream& operator<< (std::ostream& out, const Window& window);
};

#endif /* Window_h */
