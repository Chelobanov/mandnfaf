#include "Mandelbrot.h"

int main ()
{
    Mandelbrot m;

//    m.Calculate();
//    m.CalculateMT(700, 900);

    m.RunCalculation();

    sf::Event event{};

    while (m.window.isOpen())
    {
        while (m.window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                {
                    m.window.close();
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    if (event.key.code == sf::Keyboard::Q)
                    {
                        m.ShowCoords();
                        break;
                    }

                    if (event.key.code == sf::Keyboard::O)
                    {
                        m.Zoom();
                    }

                    if (event.key.code == sf::Keyboard::P)
                    {
                        m.Unzoom();
                    }

                    if (event.key.code == sf::Keyboard::A)
                    {
                        m.Move(-1, 0);
                    }

                    if (event.key.code == sf::Keyboard::D)
                    {
                        m.Move(1, 0);
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        m.Move(0, -1);
                    }

                    if (event.key.code == sf::Keyboard::W)
                    {
                        m.Move(0, 1);
                    }

                    if (event.key.code == sf::Keyboard::Z)
                    {
                        m.depth -= 5;
                    }

                    if (event.key.code == sf::Keyboard::X)
                    {
                        m.depth += 5;
                    }

                    if (event.key.code == sf::Keyboard::Space)
                    {
                        m.Recalc();
                    }

                    break;
                }
            }
        }

        m.window.clear();

        m.Draw();

        m.window.display();
    }
}
