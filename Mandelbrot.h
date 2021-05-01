//
// Created by Anton Karpov on 05.04.2021.
//

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <complex>
#include <thread>
#include <array>

std::complex<double> func(std::complex<double> current, std::complex<double> add)
{
    return pow(current, 2) + add;
//    return pow(std::complex<double>(abs(current.real()), -abs(current.imag())), 2) + add;
}

class Mandelbrot
{
public:
    uint windowHeight = 1800, windowWidth = 1800;
    sf::Image image;
    sf::RenderWindow window;
    int depth = 30;
    double center_x = -.5, center_y = 0.;
    double view = 1.5;
    double left = -2, right = 1, bottom = -1.5, top = 1.5;
    int threadCount = 36;
    std::array<std::thread, 36> threads;

public:
    Mandelbrot()
    {
        window.create(sf::VideoMode(windowWidth, windowHeight), "fractal");
        window.setFramerateLimit(10);
        window.clear(sf::Color::Black);

        sf::Texture texture;
        texture.create(windowWidth, windowHeight);
        texture.update(window);

        texture.copyToImage().saveToFile("../mem.png");

        image.loadFromFile("../mem.png");
    }

    void CalculateMT(int start, int end)
    {
        for (int x = 0; x < windowWidth; x++)
        {
            for (int y = start; y < end; y++)
            {
                auto X = static_cast<double>(x), Y = static_cast<double>(y);

                std::complex<double> currentPt(X / (windowWidth) * (2 * view) + center_x - view
                        , (windowHeight - Y) / (windowHeight) * (2 * view) + center_y - view);


                double color = 0;

                std::complex<double> current(0., 0.);

                for (int i = 0; i < depth; i++)
                {
                    current = func(current, currentPt);

                    if (abs(current) > 2.)
                    {
                        break;
                    }

                    color += 255. / depth;

                    if (i == depth - 1)
                    {
                        color = 0;
                    }
                }



                image.setPixel(x, y, sf::Color(color, color, color));
            }
        }

//        image.saveToFile("../mem.png");
    }

    void OptimizedCalculateMT(int start, int end)
    {
        for (int x = 0; x < windowWidth; x++)
        {
            for (int y = start; y < end; y++)
            {
                auto X = static_cast<double>(x), Y = static_cast<double>(y);

                X = X / (windowWidth) * (2 * view) + center_x - view;
                Y = (windowHeight - Y) / (windowHeight) * (2 * view) + center_y - view;

                double color = 0;

                double x_ = 0, y_ = 0;
                double x2 = 0, y2 = 0;

                for (int i = 0; i < depth; i++)
                {
                    y_ = 2 * x_ * y_ + Y;
                    x_ = x2 - y2 + X;
                    x2 = x_ * x_;
                    y2 = y_ * y_;

                    if (x2 + y2 > 4)
                    {
                        break;
                    }

                    color += 255. / depth;

                    if (i == depth - 1)
                    {
                        color = 0;
                    }
                }



                image.setPixel(x, y, sf::Color(color, color, color));
            }
        }

//        image.saveToFile("../mem.png");
    }

    void RunCalculation()
    {
        int start = 0;
        int end = windowHeight / threadCount;

        for (auto& t : threads)
        {
            t = std::thread(&Mandelbrot::OptimizedCalculateMT, this, start, end);

            start = end;
            end += windowHeight / threadCount;

            std::cout << "created thread\n";
        }

//        for (auto& t : threads)
//        {
//            t.join();
//        }
    }

    void Move(int right, int up)
    {
        center_x += view / 3 * right;
        center_y += view / 3 * up;

        for (auto & t : threads)
        {
            t.join();
        }

        RunCalculation();
    }

    void Zoom()
    {
        view *= .5;

        for (auto & t : threads)
        {
            t.join();
        }

        RunCalculation();
    }

    void Unzoom()
    {
        view *= 2;

        for (auto & t : threads)
        {
            t.join();
        }

        RunCalculation();
    }

    void Recalc()
    {
        for (auto & t : threads)
        {
            t.join();
        }

        RunCalculation();
    }

//    void Calculate()
//    {
//        for (int x = 0; x < windowWidth; x++)
//        {
//            for (int y = 0; y < windowHeight; y++)
//            {
//                auto X = static_cast<double>(x), Y = static_cast<double>(y);
//
////                std::complex<double> currentPt(X / (windowWidth) * (right - left) + left
////                , (windowHeight - Y) / (windowHeight) * (top - bottom) + bottom);
//
//                std::complex<double> currentPt(X / (windowWidth) * (2 * view) + center_x - view
//                        , (windowHeight - Y) / (windowHeight) * (2 * view) + center_y - view);
//
//
//                double color = 255;
//
//                std::complex<double> current(0., 0.);
//
//                for (int i = 0; i < depth; i++)
//                {
//                    current = func(current, currentPt);
//
//                    if (abs(current) > 2.)
//                    {
//                        break;
//                    }
//
//                    color -= 255. / depth;
//                }
//
////                image.setPixel(x, y, sf::Color(color, color, color));
//                image.setPixel(x, y, sf::Color(color, color, color));
//
//                std::cout << (X * windowHeight + Y) / (windowHeight * windowWidth) * 100. << "%\r";
//            }
//        }
//
////        image.saveToFile("../mem.png");
//    }

    void Draw()
    {
        sf::Texture t;
        sf::Sprite s;

        t.loadFromImage(image);
        s.setTexture(t);

        window.draw(s);
    }

    void ShowCoords()
    {
        auto vec = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

        vec.x = vec.x / (windowWidth) * (right - left) + left;
        vec.y = (windowHeight - vec.y) / (windowHeight) * (top - bottom) + bottom;

        std::cout << vec.x << " " << vec.y << std::endl;
    }
};
#endif //MANDELBROT_MANDELBROT_H
