//
//  Window.cpp
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#include "Window.h"

//
//    Window() :
//    base(Point<double>(0.0, 0.0)),
//    ratio(1.0) {}
Window::Window(Point<double>& p, double r) :
    base(p),
    ratio(r)
    {}
    
/* copy constructor */
Window::Window(const Window& w) :
    base(w.base),
    ratio(w.ratio) {}

void Window::pixelToPoint(const Point<int>& pixel, Point<double>& point) const
{
    point.x = (1.0 * pixel.x) / this->ratio + this->base.x;
    point.y = (1.0 * pixel.y) / this->ratio + this->base.y;
}
