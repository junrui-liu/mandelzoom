//
//  Point.tpp
//  mandelzoom
//
//  Created by work on 1/31/19.
//


#ifndef point_tpp
#define point_tpp

template <class T>
Point<T>::Point(T x1, T y1)
{
    x = x1;
    y = y1;
}

/* copy constructor */
template <class T>
Point<T>::Point(const Point<T>& p)
{
    x = p.x;
    y = p.y;
}

template <class T>
void Point<T>::complexAdd(const Point<T>& p, const Point<T>& q, Point<T>& result)
{
    result.x = p.x + q.x;
    result.y = p.y + q.y;
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

#endif /* point_tpp */