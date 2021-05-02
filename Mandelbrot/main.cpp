#include "Mandelbrot.h"
#include "Julia.h"
#include "RandomString.h"

int main ()
{
    int drawJulia = 0;

    Mandelbrot m;

//    m.Calculate();
//    m.CalculateMT(700, 900);

    m.RunCalculation();

    Julia jul(std::ref(m.window));
    jul.SetCurrentPoint();
    jul.RunCalculation();

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
                    if (event.key.code == sf::Keyboard::O)
                    {
                        m.Zoom();
                        jul.Zoom();
                    }

                    if (event.key.code == sf::Keyboard::P)
                    {
                        m.Unzoom();
                        jul.Unzoom();
                    }

                    if (event.key.code == sf::Keyboard::A)
                    {
                        m.Move(-1, 0);
                        jul.Move(-1, 0);
                    }

                    if (event.key.code == sf::Keyboard::D)
                    {
                        m.Move(1, 0);
                        jul.Move(1, 0);
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        m.Move(0, -1);
                        jul.Move(0, -1);
                    }

                    if (event.key.code == sf::Keyboard::W)
                    {
                        m.Move(0, 1);
                        jul.Move(0, 1);
                    }

                    if (event.key.code == sf::Keyboard::Z)
                    {
                        m.depth -= 5;
                        jul.depth -= 5;
                    }

                    if (event.key.code == sf::Keyboard::X)
                    {
                        m.depth += 5;
                        jul.depth += 5;
                    }

                    if (event.key.code == sf::Keyboard::Space)
                    {
                        m.Recalc();
                    }

                    if (event.key.code == sf::Keyboard::LShift && drawJulia)
                    {
                        jul.Recalc();
                    }

                    if (event.key.code == sf::Keyboard::Tab && drawJulia)
                    {
                        jul.SetCurrentPoint();
                        jul.RunCalculation();
                    }

                    if (event.key.code == sf::Keyboard::BackSpace)
                    {
                        drawJulia = 1 - drawJulia;
                    }

                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        m.colorScheme = 1 - m.colorScheme;
                        m.Recalc();
                    }

                    if (event.key.code == sf::Keyboard::RShift)
                    {
                        sf::Texture screenshot;
                        screenshot.create(1800, 1800);
                        screenshot.update(m.window);
                        screenshot.copyToImage().saveToFile("../Images/" + RandomString(7) + ".png");
                    }

                    break;
                }
            }
        }

        m.window.clear();

        m.Draw();

        if (drawJulia)
        {
            jul.Draw();
        }

        m.window.display();
    }
}
