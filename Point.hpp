#ifndef GEOMETRY_LIB_POINT_HPP
#define GEOMETRY_LIB_POINT_HPP

#include <vector>
#include <ctype.h>
#include <algorithm>
#include <cmath>

class Point
{
private:
    double* _coord = nullptr;
    int _dimension;
    int _size;

    bool is_zeros(int ind) const
    {
        if (ind >= _size) return false;
        for (int i = ind; i < _size; i++)
            if ((*this)[i] != 0) return false;
        return true;
    }

    void dimension_normalization()
    {
        _dimension = 0;
        for (int i = 0; i < _size; i++)
            if ((*this)[i] != 0) _dimension++;
    }

    void zero_normalization()
    {
        int s = Size(), i;
        for (i = s - 1; i > 0 && (*this)[i] == 0; i--)
            s--;
        if (s != Size())
            resize(s);
    }

    bool equal(const Point& a, int size) const
    {
        for (int i = 0; i < size; i++)
            if ((*this)[i] != a[i]) return false;
        return true;
    }

public:
    Point()
    {
        _coord = new double[1]{};
        _size = 1;
        _dimension = 0;
    }

    Point(double x, double y);

    Point(int size)
    {
        _coord = new double[size] {};
        _size = size;
        _dimension = 0;
    }

    Point(double* coord, int size)
    {
        _coord = new double[size]{};
        _size = size;
        std::copy_n(coord, size, _coord);
        dimension_normalization();
    }

    ~Point()
    {
        delete[] _coord;
    }

    bool operator==(const Point& a) const
    {
        int min = Size();
        if (Size() < a.Size())
        {
            if (!a.is_zeros(a.Size() - Size() - 1)) return false;
        }
        else if (Size() > a.Size())
        {
            min = a.Size();
            if (!is_zeros(Size() - a.Size() - 1)) return false;
        }
        return equal(a, min);
    }
    
    bool operator!=(const Point& a) const { return !(*this == a); }

    Point& operator += (const Point& a)
    {
        int i = a.Size();
        if (Size() < a.Size())
        {
            int min = Size();
            resize(a.Size());
            for (i = Size() - 1; i >= min; i--)
                (*this)[i] = a[i];
        }
        for (; i >= 0; i--)
            (*this)[i] += a[i];
        dimension_normalization();
        return *this;
    }

    Point& operator -= (const Point& a)
    {
        *this += -a;
        return *this;
    }
    
    Point operator-(const Point& a) const { return (*this) + (-a); }

    Point operator-() const
    {
        Point ans;
        ans = Point(Size());
        for (int i = 0; i < Size(); i++)
            ans[i] = -(*this)[i];
        return ans;
    }

    Point operator+(const Point& a) const
    {
        Point ans = Point();
        ans += *this;
        ans += a;

        /*
        Point ans;
        int i = Size(), min = Size();
        if (Size() < a.Size())
        {
            ans = Point(a.Size());
            for (i = ans.Size() - 1; i >= min; i--)
                ans[i] = a[i];
        }
        else if (Size() > a.Size())
        {
            min = a.Size();
            ans = Point(Size());
            for (i = ans.Size() - 1; i >= min; i--)
                ans[i] = (*this)[i];
        }
        for (; i >= 0; i--)
            ans[i] = (*this)[i] + a[i];
        */

        ans.dimension_normalization();
        return ans;
    }

    double operator*(const Point& a) const
    {
        double ans = 0;
        int min = std::min(a.Size(), Size());
        for (int i = 0; i < min; i++)
            ans += a[i] * (*this)[i];
        return ans;
    }

    Point operator^(const Point& a) const
    {
        Point ans(3);
        Point a1 = copy();
        Point a2 = a.copy();
        a1.resize(3);
        a2.resize(3);
        ans[0] = a1[1] * a2[2] - a1[2] * a2[1];
        ans[1] = a1[2] * a2[0] - a1[0] * a2[2];
        ans[2] = a1[0] * a2[1] - a1[1] * a2[0];
        return ans;
    }
    
    double operator[](int ind) const { return _coord[ind]; }

    double& operator[](int ind) { return _coord[ind]; }
    
    double operator[](char* ch) const
    {
        switch (tolower(ch[0]))
        {
        case 'x':
            return (*this)[0];
        case 'y':
            return (*this)[1];
        case 'z':
            return (*this)[2];
        case 'w':
            return (*this)[3];
        }
    }
    
    double& operator[](char* ch)
    {
        switch (tolower(ch[0]))
        {
        case 'x':
            return (*this)[0];
        case 'y':
            return (*this)[1];
        case 'z':
            return (*this)[2];
        case 'w':
            return (*this)[3];
        }
    }

    const int Size() const { return _size; }

    const int Dimension() const { return _dimension; }

    const int Length() const { return distance(Point(), *this); }

    void resize(int size)
    {
        double* buf = _coord;
        _coord = new double[size] {};
        _size = size;
        std::copy_n(buf, std::min(size, _size), _coord);
        delete[] buf;
        dimension_normalization();
    }

    static double distance(const Point& a, const Point& b)
    {
        double ans = 0;
        int i = a.Size();
        if (a.Size() < b.Size())
            for (i = b.Size() - 1; i >= a.Size(); i--)
                ans += b[i] * b[i];
        else if (a.Size() > b.Size())
            for (i = a.Size() - 1; i >= b.Size(); i--)
                ans += a[i] * a[i];
        for (; i >= 0; i--)
            ans += pow(a[i] - b[i], 2);
        return sqrt(ans);
    }

    void zeroing()
    {
        delete[]_coord;
        _coord = new double[1]{};
        _size = 1;
        _dimension = 0;
    }
    
    static Point middle(const Point& a, const Point& b)
    {
        Point ans;
        int i = a.Size(), min = a.Size();
        if (a.Size() < b.Size())
        {
            ans = Point(b.Size());
            for (i = ans.Size() - 1; i >= min; i--)
                ans[i] = b[i] / 2;
        }
        else if (a.Size() > b.Size())
        {
            min = b.Size();
            ans = Point(a.Size());
            for (i = ans.Size() - 1; i >= min; i--)
                ans[i] = a[i] / 2;
        }
        for (; i >= 0; i--)
            ans[i] = (a[i] + b[i]) / 2;
        ans.dimension_normalization();
        return ans;
    }

    Point copy() const
    {
        double* arr = new double[Size()];
        std::copy_n(_coord, Size(), arr);
        Point ans(arr, Size());
        return ans;
    }

    static double cos(const Point& a, const Point& b)
    {
        return a * b / (a.Length() * b.Length());
    }
    
    /**
     * @brief Does check is point has one or both coordinates at infinity
     *
     * @param point A point that need to check
     */
    static bool isAtInfinity(const Point& point);
};

#endif // GEOMETRY_LIB_POINT_HPP
