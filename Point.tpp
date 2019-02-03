//
//  Point.tpp
//  mandelzoom
//
//  Created by work on 1/31/19.
//


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
    T zx = 0.0, zy = 0.0;
    T cx = c.x, cy = c.y;
    T mod_sq;
    T tmp_x, tmp_y;
    for (unsigned int run = 0; run < bound; ++run)
    {
        tmp_x = zx * zx - zy * zy + cx;
        tmp_y = 2 * zx * zy + cy;
        mod_sq = tmp_x * tmp_x + tmp_y * tmp_y;
        if (mod_sq > 4) return run;
        zx = tmp_x;
        zy = tmp_y;
    }
    return bound;
}

/* overloaded methods */
template <class T>
std::ostream& operator<< (std::ostream& out, const Point<T>& p)
{
    out << "Point(" << p.x << "," << p.y << ")";
    return out;
}

#endif /* point_tpp */
