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
           double xTemp, yTemp;

           for (int i = 0; i < depth; i++)
           {
               xTemp = x_;
               yTemp = y_;
               x_ = xTemp * xTemp - yTemp * yTemp + currentPoint.x;
               y_ = yTemp * xTemp + currentPoint.y;

               if (x_ + y_ > 4)
               {
                   break;
               }

               color += 255. / depth;
           }

           image.setPixel(x, y, sf::Color(0, color, 0, 128));

       }
   }
}

void Julia::RunCalculation ()
{
    int start = 0;
    int end = windowHeight / threadCount;

    for (auto& t : threads)
    {
        t = std::thread(&Julia::CalculateMT, this, start, end);

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
