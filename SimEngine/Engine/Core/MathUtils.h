#pragma once

class MathUtils
{
public:
    template <class T>
    static T randomNum(T min, T max);
};

template <class T>
T MathUtils::randomNum(T min, T max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution dist(min, max);
    return dist(gen);
}
