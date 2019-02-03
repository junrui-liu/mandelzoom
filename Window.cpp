//
//  Window.cpp
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#include "Window.h"

Window::Window() :
    base(Point<double>()),
    ratio(0) {}

Window::Window(Point<double>& p, double r) :
    base(p),
    ratio(r) {}
    
/* copy constructor */
Window::Window(const Window& w) :
    base(w.base),
    ratio(w.ratio) {}

void Window::pixelToPoint(const Point<int>& pixel,
                          Point<double>& point) const
{
    point.x = (1.0 * pixel.x) / this->ratio + this->base.x;
    point.y = (1.0 * pixel.y) / this->ratio + this->base.y;
}

void Window::center(int w, int h, Point<int>& center) const
{
    center.x = w/2;
    center.y = h/2;
}

/* Scale a window by some factor with respect to
 a pivot point in the current window.
 */
void Window::scaleWrtPivot(double factor, Point<int>& pivotPixel, Window& newWindow) const
{
    Point<double> pivotReal, newBase;
    pixelToPoint(pivotPixel, pivotReal);
    Point<double>::scaleWrtPoints(pivotReal, this->base, 1/factor, newBase);
    newWindow.base = newBase;
    newWindow.ratio = this->ratio * factor;
}

/* Scale a window by some factor with respect to
 * the center of the current window.
 */
void Window::scaleWrtCenter(double factor, int w, int h, Window& newWindow) const

{
    Point<int> c;
    center(w, h, c);
    scaleWrtPivot(factor, c, newWindow);
}


std::ostream& operator<< (std::ostream& out, const Window& window)
{
    out
    << "Window[" << window.base << ","
    << "ratio=" << window.ratio << "]";
    return out;
}
