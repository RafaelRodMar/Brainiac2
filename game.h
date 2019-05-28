class Game{
public:
    int screenwidth = -1;
    int screenheight = -1;
    sf::RenderWindow app;

    enum game_states {MENU, GAME, END_GAME};
    int state = MENU;
    std::vector<int> vhiscores;
    int score = -1;

    //keyboard handling
    // The keyboard's state in the current- and the previous frame
    bool CurrentKeyState[sf::Keyboard::KeyCount];
    bool PreviousKeyState[sf::Keyboard::KeyCount];

    //sound buffers
    sf::SoundBuffer SelectBuffer;
    sf::SoundBuffer MatchBuffer;
    sf::SoundBuffer MismatchBuffer;
    sf::SoundBuffer WinBuffer;
    //sound play
    sf::Sound Select;
    sf::Sound Match;
    sf::Sound Mismatch;
    sf::Sound Win;

    //fonts
    sf::Font font;
    sf::Text showScore;

    sf::Texture t0,t1,t2,t3,t4,t5,t6,t7,t8;

    //sprites
    sf::Sprite tile[9];

    bool TileStates[4][4];
    int Tiles[4][4];
    sf::Vector2i tile1,tile2;
    int matches,tries;
    bool mouseclicked = false;
    sf::Vector2i mousepos;

    void init(int pscrw, int pscrh, int pposx, int pposy, std::string pname);
    void input();
    void update();
    void draw();

    //keyboard functions
    bool KeyPressed(sf::Keyboard::Key Key)
        { return (CurrentKeyState[Key] && !PreviousKeyState[Key]); }

    bool KeyReleased(sf::Keyboard::Key Key)
        { return (!CurrentKeyState[Key] && PreviousKeyState[Key]); }

    bool KeyHeld(sf::Keyboard::Key Key)
        { return CurrentKeyState[Key]; }
};

void Game::init(int pscrw, int pscrh, int pposx, int pposy, std::string pname)
{
    screenwidth = pscrw;
    screenheight = pscrh;
    app.create(sf::VideoMode(pscrw,pscrh),pname);
    app.setPosition(sf::Vector2i(pposx,pposy));

    t0.loadFromFile("assets/img/tileblank.png");
    t1.loadFromFile("assets/img/tile1.png");
    t2.loadFromFile("assets/img/tile2.png");
    t3.loadFromFile("assets/img/tile3.png");
    t4.loadFromFile("assets/img/tile4.png");
    t5.loadFromFile("assets/img/tile5.png");
    t6.loadFromFile("assets/img/tile6.png");
    t7.loadFromFile("assets/img/tile7.png");
    t8.loadFromFile("assets/img/tile8.png");

    //assign textures to sprites
    tile[0].setTexture(t0);
    tile[1].setTexture(t1);
    tile[2].setTexture(t2);
    tile[3].setTexture(t3);
    tile[4].setTexture(t4);
    tile[5].setTexture(t5);
    tile[6].setTexture(t6);
    tile[7].setTexture(t7);
    tile[8].setTexture(t8);

    // Load the text font
    font.loadFromFile("assets/fonts/sansation.ttf");

    // Load the sounds used in the game
    SelectBuffer.loadFromFile("assets/sounds/select.wav");
    Select.setBuffer(SelectBuffer);
    MatchBuffer.loadFromFile("assets/sounds/match.wav");
    Match.setBuffer(MatchBuffer);
    MismatchBuffer.loadFromFile("assets/sounds/mismatch.wav");
    Mismatch.setBuffer(MismatchBuffer);
    WinBuffer.loadFromFile("assets/sounds/win.wav");
    Win.setBuffer(WinBuffer);

    //keyboard buffers initialization
    memset(CurrentKeyState,     false, sizeof(CurrentKeyState));
    memset(PreviousKeyState,    false, sizeof(PreviousKeyState));

}

void Game::input()
{
    switch(state)
        {
        case MENU:
            {
                sf::Event event;
                while (app.pollEvent(event))
                {
                    if ((event.type == sf::Event::Closed) ||
                        ((event.type == sf::Event::KeyPressed)
                         && (event.key.code == sf::Keyboard::Escape)))
                        app.close();

                    // S key pressed: change state to GAME
                    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
                    {
                        state=GAME;
                        score = 0;
                        //clear the tile states and images
                        for(int i=0;i<4;i++) {
                            for(int j=0;j<4;j++) {
                                TileStates[i][j] = false;
                                Tiles[i][j] = 0;
                            }
                        }
                        //initialize the tile images randomly
                        for(int i=0;i<2;i++)
                        {
                            for(int j=0;j<9;j++)
                            {
                                int x = rnd.getRndInt(0,3);
                                int y = rnd.getRndInt(0,3);
                                while( Tiles[x][y] != 0)
                                {
                                    x = rnd.getRndInt(0,3);
                                    y = rnd.getRndInt(0,3);
                                }
                                Tiles[x][y] = j;
                            }
                        }
                        //initialize the tile selections and match/try count
                        tile1.x = tile1.y = -1;
                        tile2.x = tile2.y = -1;
                        matches = tries = 0;
                    }
                }
                break;
            }
        case GAME:
            {
                sf::Event event;
                while (app.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        app.close();
                }

                // Save the state of each keyboard key (must be done before any Key* function is executed)
                for(unsigned int i = 0; i < sf::Keyboard::KeyCount; ++i)
                {
                    // Save the keyboard's state from the previous frame
                    PreviousKeyState[i] = CurrentKeyState[i];

                    // And save the keyboard's state in the current frame
                    CurrentKeyState[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
                }

                if(KeyPressed(sf::Keyboard::Escape))
                    app.close();

                if(event.type == sf::Event::MouseButtonPressed)
                {
                    if(event.mouseButton.button == sf::Mouse::Left)
                    {
                        mouseclicked = true;
                        mousepos = sf::Mouse::getPosition(app);
                    }
                }

                break;
            }
        case END_GAME:
            {
                sf::Event event;
                while (app.pollEvent(event))
                {
                    if ((event.type == sf::Event::Closed) ||
                        ((event.type == sf::Event::KeyPressed)
                         && (event.key.code == sf::Keyboard::Escape)))
                        app.close();

                    // Any key pressed: change state to MENU
                    if (event.type == sf::Event::KeyPressed)
                    {
                        state=MENU;
                    }
                }
                break;
            }
        default:
            break;
        }
}

void Game::update()
{
    //Game_cycle();
    if(state==GAME && mouseclicked == true)
    {
        //determine which tile was clicked
        int TileX = mousepos.x / tile[0].getGlobalBounds().width;
        int TileY = mousepos.y / tile[0].getGlobalBounds().height;

        //make sure the tile hasn't already been matched
        if( !TileStates[TileX][TileY])
        {
            //see if this is the first tile selected
            if( tile1.x == -1 )
            {
                //play sound for tile selection
                Select.play();

                //set the first tile selection
                tile1.x = TileX;
                tile1.y = TileY;
            }
            else if ((TileX != tile1.x) || (TileY != tile1.y))
            {
                if( tile2.x == -1)
                {
                    //play a sound for the tile selection
                    Select.play();

                    //increase the number of tries
                    tries++;

                    //set the second tile selection
                    tile2.x = TileX;
                    tile2.y = TileY;

                    //see if it's a match
                    if( Tiles[tile1.x][tile1.y] == Tiles[tile2.x][tile2.y])
                    {
                        //play a sound for the tile match
                        Match.play();

                        //set the tile state to indicate the match
                        TileStates[tile1.x][tile1.y] = true;
                        TileStates[tile2.x][tile2.y] = true;

                        //clear the tile selections
                        tile1.x = tile1.y = tile2.x = tile2.y = -1;

                        //update the match count and check for winner
                        if (++matches == 8)
                        {
                            //play a victory sound
                            Win.play();
                            UpdateHiScores(vhiscores,tries);
                            state = END_GAME;
                        }
                    }
                    else
                    {
                        //play a sound for the tile mismatch
                        Mismatch.play();
                    }
                }
                else
                {
                    //clear the tile selections
                    tile1.x = tile1.y = tile2.x = tile2.y = -1;
                }
            }
        }
        mouseclicked = false;
    }
}

void Game::draw()
{
    //Game_paint();
    //////draw//////
    app.clear();

    switch(state)
    {
    case MENU:
        {
        //Show hi scores
        std::string histr="     HiScores\n";
        for(int i=0;i<5;i++)
        {
            if(vhiscores[i]>0)
                histr = histr + "           " + std::to_string(vhiscores[i]) + "\n";
        }
        histr += "PRESS S TO START";
        Text(app,histr,200.f,150.f,sf::Color::White,20,font);
        }
        break;
    case GAME:
        {
            //Draw the tiles
            int tilewidth = tile[0].getGlobalBounds().width;
            int tileheight = tile[0].getGlobalBounds().height;

            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    if( TileStates[i][j] || (( i == tile1.x ) && ( j == tile1.y)) || (( i == tile2.x) && (j == tile2.y)))
                    {
                        sf::Vector2f pos(i*tilewidth, j*tileheight);
                        tile[Tiles[i][j]].setPosition(pos);
                        app.draw(tile[Tiles[i][j]]);
                    }
                    else
                    {
                        sf::Vector2f pos(i*tilewidth, j*tileheight);
                        tile[0].setPosition(pos);
                        app.draw(tile[0]);
                    }
                }
            }
        }
        break;
    case END_GAME:
        {
            std::string histr = "You won in " + std::to_string(tries) + " tries.";
            Text(app,histr,200.f,150.f,sf::Color::White,20,font);
        }
        break;
    default:
        break;
    }
    app.display();
}
