//
// Created by Anton Karpov on 02.05.2021.
//

#include "Mandelbrot.h"

Mandelbrot::Mandelbrot ()
{
    window.create(sf::VideoMode(windowWidth, windowHeight), "fractal");
    window.setFramerateLimit(60);
    window.clear(sf::Color::Black);

    sf::Texture texture;
    texture.create(windowWidth, windowHeight);
    texture.update(window);

    texture.copyToImage().saveToFile("../mem.png");

    image.loadFromFile("../mem.png");
}

void Mandelbrot::CalculateMT (int start, int end)
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
                y_ = (x_ + x_) * y_ + Y;
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
}

void Mandelbrot::TolerantCalculateMT (int start, int end)
{
    for (int x = 0; x < windowWidth; x++)
    {
        for (int y = start; y < end; y++)
        {
            auto X = static_cast<double>(x), Y = static_cast<double>(y);

            X = X / (windowWidth) * (2 * view) + center_x - view;
            Y = (windowHeight - Y) / (windowHeight) * (2 * view) + center_y - view;

            double r = 255, g = 0, b = 0;

            double x_ = 0, y_ = 0;
            double x2 = 0, y2 = 0;

            for (int i = 0; i < depth; i++)
            {
                y_ = (x_ + x_) * y_ + Y;
                x_ = x2 - y2 + X;
                x2 = x_ * x_;
                y2 = y_ * y_;

                if (x2 + y2 > 4)
                {
                    break;
                }

                r -= 255. * 4 / depth * (i > depth / 2 && i < depth * 3 / 4);
                g += 255. * 2 / depth * (i < depth / 2);
                g -= 255. * 4 / depth * (i > depth * 3 / 4);
                b += 255. * 4 / depth * (i > depth * 3 / 4);


                if (i == depth - 1)
                {
                    r = 0;
                    g = 0;
                    b = 0;
                }
            }

            image.setPixel(x, y, sf::Color(r, g, b));
        }
    }
}

void Mandelbrot::RunCalculation ()
{
    int start = 0;
    int end = windowHeight / threadCount;

    for (auto& t : threads)
    {
        if (!colorScheme)
        {
            t = std::thread(&Mandelbrot::CalculateMT, this, start, end);
        }else
        {
            t = std::thread(&Mandelbrot::TolerantCalculateMT, this, start, end);
        }

        start = end;
        end += windowHeight / threadCount;
    }
}

void Mandelbrot::Move (int right, int up)
{
    center_x += view / 3 * right;
    center_y += view / 3 * up;

    for (auto & t : threads)
    {
        t.join();
    }

    RunCalculation();
}

void Mandelbrot::Zoom ()
{
    view *= .5;

    for (auto & t : threads)
    {
        t.join();
    }

    RunCalculation();
}

void Mandelbrot::Unzoom ()
{
    view *= 2;

    for (auto & t : threads)
    {
        t.join();
    }

    RunCalculation();
}

void Mandelbrot::Recalc ()
{
    for (auto & t : threads)
    {
        t.join();
    }

    RunCalculation();
}

void Mandelbrot::Draw ()
{
    sf::Texture t;
    sf::Sprite s;

    t.loadFromImage(image);
    s.setTexture(t);

    window.draw(s);
}