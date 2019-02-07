//
//  Point.h
//  mandelzoom
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#ifndef Point_h
#define Point_h

#include <iostream>

/*
 * Template Class: Point
 */
template <class T>
class Point {
public:
    T x;
    T y;
    
    Point();
    Point(T x1, T y1);
    
    /* copy constructor */
    Point(const Point<T>& p);
    
    /* static methods */
    static void scaleWrtPoints(const Point<T>& pivot, const Point<T>& q,
                               T scalar, Point<T>& result);
    
    static void complexAdd(const Point<T>& p,
                           const Point<T>& q,
                           Point<T>& result);
    
    static void complexMult(const Point<T>& p,
                            const Point<T>& q,
                            Point<T>& result);
    
    static void complexScale(const Point<T>& p,
                             T scalar,
                             Point<T>& result);
    
    static void complexMod(const Point<T>& p,
                           T& result);
    
    static unsigned int computeIterations(const Point<T>& c,
                                          unsigned int bound);
    
    static unsigned int computeIterationsInline(const Point<T>& c,
                                                unsigned int bound);
    
    /* overloaded methods */
    template <class T_>
    friend std::ostream& operator<< (std::ostream& out, const Point<T_>& p);
    
};



#include "Point.tpp"

#endif /* Point_h */
