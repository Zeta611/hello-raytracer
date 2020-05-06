#ifndef VEC_H
#define VEC_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <type_traits>

template<
    int DIM,
    typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
class vec;

template<int DIM, typename T>
vec<DIM, T> operator+(const vec<DIM, T>&, const vec<DIM, T>&);

template<int DIM, typename T>
vec<DIM, T> operator-(const vec<DIM, T>&, const vec<DIM, T>&);

template<int DIM, typename T>
T operator*(const vec<DIM, T>&, const vec<DIM, T>&);

template<int DIM, typename T>
vec<DIM, T> operator*(const vec<DIM, T>&, T);

template<int DIM, typename T>
vec<DIM, T> operator*(T, const vec<DIM, T>&);

template<int DIM, typename T>
vec<DIM, T> operator/(const vec<DIM, T>&, T);

template<int DIM, typename T, typename>
class vec {
public:
    vec() : data() {}

    vec(std::initializer_list<T> lst)
    {
        std::copy(lst.begin(), lst.end(), data);
        for (size_t i = lst.size(); i < DIM; ++i) {
            data[i] = T();
        }
    }

    const T& operator[](int i) const
    {
        assert(i < DIM);
        return data[i];
    }

    T& operator[](int i)
    {
        assert(i < DIM);
        return data[i];
    }

    T magnitude_sq() const
    {
        T result = 0;
        for (int i = 0; i < DIM; ++i) {
            float e = static_cast<float>(data[i]);
            result += e * e;
        }
        return result;
    }

    float magnitude() const
    {
        return std::sqrtf(static_cast<float>(magnitude_sq()));
    }

    vec normalized() const
    {
        return *this / magnitude();
    }

    static inline vec zero = vec();

    vec operator-() const
    {
        vec<DIM, T> result;
        for (int i = 0; i < DIM; ++i) {
            result.data[i] = -this->data[i];
        }
        return result;
    }

private:
    T data[DIM];

    friend vec operator+<>(const vec&, const vec&);
    friend vec operator-<>(const vec&, const vec&);
    friend T operator*<>(const vec&, const vec&);
    friend vec operator*<>(const vec&, T);
    friend vec operator*<>(T, const vec&);
    friend vec operator/<>(const vec&, T);
};

template<int DIM, typename T>
vec<DIM, T> operator+(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs)
{
    vec<DIM, T> result;
    for (int i = 0; i < DIM; ++i) {
        result.data[i] = lhs.data[i] + rhs.data[i];
    }
    return result;
}

template<int DIM, typename T>
vec<DIM, T> operator-(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs)
{
    vec<DIM, T> result;
    for (int i = 0; i < DIM; ++i) {
        result.data[i] = lhs.data[i] - rhs.data[i];
    }
    return result;
}

template<int DIM, typename T>
T operator*(const vec<DIM, T>& lhs, const vec<DIM, T>& rhs)
{
    T result = 0;
    for (int i = 0; i < DIM; ++i) {
        result += lhs.data[i] * rhs.data[i];
    }
    return result;
}

template<int DIM, typename T>
vec<DIM, T> operator*(const vec<DIM, T>& vector, T scalar)
{
    vec<DIM, T> result;
    for (int i = 0; i < DIM; ++i) {
        result.data[i] = vector.data[i] * scalar;
    }
    return result;
}

template<int DIM, typename T>
vec<DIM, T> operator*(T scalar, const vec<DIM, T>& vector)
{
    vec<DIM, T> result;
    for (int i = 0; i < DIM; ++i) {
        result.data[i] = scalar * vector.data[i];
    }
    return result;
}

template<int DIM, typename T>
vec<DIM, T> operator/(const vec<DIM, T>& vector, T scalar)
{
    vec<DIM, T> result;
    for (int i = 0; i < DIM; ++i) {
        result.data[i] = vector.data[i] / scalar;
    }
    return result;
}

typedef vec<2, float> vec2f;
typedef vec<3, float> vec3f;
typedef vec<4, float> vec4f;
typedef vec<2, int> vec2i;
typedef vec<3, int> vec3i;
typedef vec<4, int> vec4i;

#endif /* ifndef VEC_H */
