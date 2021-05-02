//
// Created by Anton Karpov on 02.05.2021.
//

#ifndef MANDELBROT_RANDOMSTRING_H
#define MANDELBROT_RANDOMSTRING_H

#include <string>
#include <random>
#include <ctime>

std::string RandomString(int length)
{
    std::mt19937 gen;
    gen.seed(time(0));
    std::uniform_int_distribution dis('a', 'z');

    std::string res;

    for (int i = 0; i < length; i++)
    {
        res += char(dis(gen));
    }

    return res;
}

#endif //MANDELBROT_RANDOMSTRING_H
