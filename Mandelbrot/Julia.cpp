//
// Created by Anton Karpov on 02.05.2021.
//

#include "Julia.h"
#include <thread>

Julia::Julia (sf::RenderWindow &w) : window(w)
{
    sf::Texture texture;
    texture.create(windowWidth, windowHeight);
    texture.update(window);

    texture.copyToImage().saveToFile("../julia.png");

    image.loadFromFile("../julia.png");
}

void Julia::Draw ()
{
    sf::Texture texture;
    sf::Sprite sprite;

    texture.loadFromImage(image);
    sprite.setTexture(texture);
    window.draw(sprite);
}

void Julia::CalculateMT_2 (int start, int end)
{
   for (int x = 0; x < windowWidth; x++)
   {
       for (int y = start; y < end; y++)
       {
           auto X = static_cast<double>(x), Y = static_cast<double>(y);

           X = X / (windowWidth) * (2 * view) + center_x - view;
           Y = (windowHeight - Y) / (windowHeight) * (2 * view) + center_y - view;

           double color = 200;

           double x_ = X, y_ = Y;
           double xTemp;

           for (int i = 0; i < depth; i++)
           {
                xTemp = x_ * x_ - y_ * y_;
                y_ = 2 * x_ * y_ + currentPoint.y;
                x_ = xTemp + currentPoint.x;

               if (x_ * x_ + y_ * y_ > 4)
               {
                   break;
               }

               color -= 200. / depth;

               if (i == depth - 1)
               {
                   color = 200.;
               }
           }

           image.setPixel(x, y, sf::Color(255, 255, 0, color));

       }
   }
}

void Julia::CalculateMT (int start, int end)
{
    for (int x = 0; x < windowWidth; x++)
    {
        for (int y = start; y < end; y++)
        {
            auto X = static_cast<double>(x), Y = static_cast<double>(y);

            X = X / (windowWidth) * (2 * view) + center_x - view;
            Y = (windowHeight - Y) / (windowHeight) * (2 * view) + center_y - view;

            double color = 0;

            double x_ = X, y_ = Y;
            double xTemp;

            for (int i = 0; i < depth; i++)
            {
                xTemp = x_ * x_ - y_ * y_;
                y_ = 2 * x_ * y_ + currentPoint.y;
                x_ = xTemp + currentPoint.x;

                if (x_ * x_ + y_ * y_ > 4)
                {
                    break;
                }

                color += 200. / depth;
            }

            image.setPixel(x, y, sf::Color(0, 255, 255, color));

        }
    }
}


void Julia::RunCalculation ()
{
    int start = 0;
    int end = windowHeight / threadCount;

    for (auto& t : threads)
    {
        switch (colorScheme)
        {
            case 1:
            {
                t = std::thread(&Julia::CalculateMT, this, start, end);
                break;
            }
            case 2:
            {
                t = std::thread(&Julia::CalculateMT_2, this, start, end);
                break;
            }
        }
        start = end;
        end += windowHeight / threadCount;
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

void Julia::SetCurrentPoint ()
{
    currentPoint = static_cast<sf::Vector2<double>>(sf::Mouse::getPosition(window));
    currentPoint.x = currentPoint.x / (windowWidth) * (2 * view * zoom) + center_x - view * zoom + offset_x;
    currentPoint.y = (windowHeight - currentPoint.y) / (windowHeight) * (2 * view * zoom) + center_y - view * zoom + offset_y;
}

void Julia::Move (int right, int up)
{
    offset_x += view * zoom / 3 * right;
    offset_y += view * zoom / 3 * up;
}

void Julia::Zoom()
{
    zoom *= .5;
}

void Julia::Unzoom()
{
    zoom *= 2;
}

void Julia::Recalc ()
{
    RunCalculation();
}

