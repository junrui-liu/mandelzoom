//
//  Point.tpp
//  mandelzoom
//
//  Created by Junrui Liu on 1/31/19.
//
/*--------------------------------------------------------*/
/*  CS-378           Computer Graphics         Tom Ellman */
/*--------------------------------------------------------*/

#ifndef point_tpp
#define point_tpp

/* default is (0,0) */
template <class T>
Point<T>::Point() : x(0), y(0)
{}

template <class T>
Point<T>::Point(T x1, T y1) : x(x1), y(y1)
{}

/* copy constructor */
template <class T>
Point<T>::Point(const Point<T>& p)
{
    x = p.x;
    y = p.y;
}

/* static methods */
template <class T>
void Point<T>::complexAdd(const Point<T>& p, const Point<T>& q,
                          Point<T>& result)
{
    result.x = p.x + q.x;
    result.y = p.y + q.y;
}

template <class T>
void Point<T>::complexScale(const Point<T>& p, T scalar, Point<T>& result)
{
    result.x = p.x * scalar;
    result.y = p.y * scalar;
}

template <class T>
void Point<T>::scaleWrtPoints(const Point<T>& pivot, const Point<T>& q,
                              T scalar, Point<T>& result)
{
    Point<T> tmp;
    Point<T>::complexScale(pivot, 1-scalar, tmp);
    Point<T>::complexScale(q, scalar, result);
    Point<T>::complexAdd(tmp, result, result);
}

template <class T>
void Point<T>::complexMult(const Point<T>& p, const Point<T>& q, Point<T>& result)
{
    T a = p.x, b = p.y, c = q.x, d = q.y;
    result.x = a * c - b * d;
    result.y = a * d + b * c;
}

template <class T>
void Point<T>::complexMod(const Point<T>& p, T& result)
{
    result = sqrt(p.x * p.x + p.y * p.y);
}

template <class T>
unsigned int Point<T>::computeIterations(const Point<T>& c, unsigned int bound)
{
    Point<T> z(0.0, 0.0);
    T mod;
    for (unsigned int run = 0; run < bound; ++run)
    {
        complexMult(z, z, z);
        complexAdd(z, c, z);
        complexMod(z, mod);
        if (mod > 2) return run;
    }
    return bound;
}

template <class T>
unsigned int Point<T>::computeIterationsInline(const Point<T>& c, unsigned int bound)
{
    T z_r = 0.0, z_i = 0.0;
    T c_r = c.x, c_i = c.y;
    T z_rsq = z_r * z_r, z_isq = z_i * z_i;
    
    for (unsigned int run = 0; run < bound; ++run)
    {
        if (z_rsq + z_isq > 4) return run;
        z_i = z_r * z_i;
        z_i *= 2;
        z_i += c_i;
        z_r = z_rsq - z_isq + c_r;
        z_rsq = z_r * z_r;
        z_isq = z_i * z_i;
    }
    return bound;
}

/* overloaded methods */
template <class T>
std::ostream& operator<< (std::ostream& out, const Point<T>& p)
{
    out << p.x << "," << p.y;
    return out;
}

#endif /* point_tpp */
