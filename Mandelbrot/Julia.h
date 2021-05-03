//
// Created by Anton Karpov on 02.05.2021.
//

#ifndef MANDELBROT_JULIA_H
#define MANDELBROT_JULIA_H

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <thread>

class Julia
{
public:
    int threadCount = 36;
    std::array<std::thread, 36> threads;
    double center_x = 0, center_y = 0, view = 1.5;
    sf::Image image;
    uint windowHeight = 1800, windowWidth = 1800;
    int depth = 20;
    sf::RenderWindow& window;
    double offset_x = 0, offset_y = 0;
    double zoom = 1;

    sf::Vector2<double> currentPoint;

    int colorScheme = 1;

public:
    explicit Julia (sf::RenderWindow& w);

    void SetCurrentPoint();  // Sets the point for Julia set calculation based on mouse position

    void CalculateMT(int start, int end);
    void CalculateMT_2(int start, int end);

    void RunCalculation();
    void Recalc();

    void Move(int right, int up);
    void Zoom();
    void Unzoom();

    void Draw();
};


#endif //MANDELBROT_JULIA_H
