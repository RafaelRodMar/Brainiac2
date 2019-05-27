#include <SFML/Graphics.hpp>
#include <sfml/audio.hpp>
#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cstring>
#include <random>
#include <chrono>

#include "various.h"
#include "game.h"

int main()
{
    //the game class
    Game game;

    //create the main window
    game.init(528,502,200,0,"Brainiac2");

    ReadHiScores(game.vhiscores);

    sf::Clock clock;
    const sf::Time timePerFrame = sf::seconds(1.f/30.f); //30fps
    sf::Time elapsed = sf::Time::Zero;

    while(game.app.isOpen())
    {
        elapsed += clock.restart();

        game.input();

        while( elapsed >= timePerFrame )
        {
            game.update();
            elapsed -= timePerFrame;
        }

        game.draw();
    }

    WriteHiScores(game.vhiscores);

    return EXIT_SUCCESS;
}

