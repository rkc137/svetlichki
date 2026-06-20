#pragma once

#include <random>
#include <type_traits>

template <typename T>
requires (std::is_arithmetic_v<T>)
T get_rand(T from, T to)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if(from > to)
        std::swap(from, to);
    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> distr(from, to);
        return distr(gen);
    }
    else
    {
        std::uniform_real_distribution<T> distr(from, to);
        return distr(gen);
    }
}

template <typename T>
requires (std::is_arithmetic_v<T>)
T get_rand(T to)
{
    return get_rand<T>(0, to);
}