//
// Created by Anton Karpov on 05.04.2021.
//

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <array>

class Mandelbrot
{
public:
    uint windowHeight = 1800, windowWidth = 1800;
    sf::Image image;
    sf::RenderWindow window;
    int depth = 20;
    double center_x = -.5, center_y = 0.;
    double view = 1.5;
    int threadCount = 36;
    std::array<std::thread, 36> threads;

    int colorScheme = 0;  // 1 for rainbow, 0 for greyscale

public:
    Mandelbrot();

    void CalculateMT(int start, int end);  // Calculates points for one strip windowWidth * [some height]
    void TolerantCalculateMT(int start, int end);  // Rainbow
    void CalculateMT_2(int start, int end);
    void CalculateMT_3(int start, int end);

    void RunCalculation();  // Sets up threads for each strip
    void Recalc();  // Joins threads and calls RunCalculation()

    void Move(int right, int up);  // Move across the grid (right and up can be {-1, 0, 1} for directions)
    void Zoom();
    void Unzoom();

    void Draw();
};

#endif //MANDELBROT_MANDELBROT_H
