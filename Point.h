//
//  Point.h
//  mandelzoom
//
//  Created by work on 1/31/19.
//

#ifndef Point_h
#define Point_h

/*
 * Template Class: Point
 */
template <class T>
class Point {
public:
    T x;
    T y;
    
    Point(T x1, T y1);
    
    /* copy constructor */
    Point(const Point<T>& p);
    
    static void complexAdd(const Point<T>& p,
                           const Point<T>& q,
                           Point<T>& result);
    
    static void complexMult(const Point<T>& p,
                            const Point<T>& q,
                            Point<T>& result);
    
    static void complexMod(const Point<T>& p,
                           T& result);
    
    static unsigned int computeIterations(const Point<T>& c,
                                          unsigned int bound);
    
    static unsigned int computeIterationsInline(const Point<T>& c,
                                                unsigned int bound);
};

#include "Point.tpp"

#endif /* Point_h */
