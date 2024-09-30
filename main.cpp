#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;


// Player Position
enum class side { LEFT, RIGHT, NONE };

//log animations
vector<Sprite> flyinglogs;
vector<float> logsidex;
vector<float> logspeed;
vector<float> logsidey;


//branches on tree
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
side branchPositions[NUM_BRANCHES];
double branchleft[NUM_BRANCHES];
double branchright[NUM_BRANCHES];

const int treeheight = 700;

enum class State {
    PAUSED, GAME_OVER, PLAYING, MENU, OPTIONS, COUNTDOWN
};

static String pathBase = "/Users/colinwang/Downloads/TimberManGraphics/";
int main(int, char const**) {
    // Create a video mode object
    sf::VideoMode vm(1920, 1080);

    // Create and open a window for the game
    sf::RenderWindow window(vm, "Timber!!!");
    //______________________________________________________TEXTURES________________________________________________________________________________________


    //background
    sf::Texture textureBackground;
    textureBackground.loadFromFile(pathBase+"background.png");
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    //Tree
    Texture textureTree;
    textureTree.loadFromFile(pathBase+"tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //Player
    Texture texturePlayer;
    texturePlayer.loadFromFile(pathBase+"player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(550, 700);

    //Grave (on death)
    Texture textureRIP;
    textureRIP.loadFromFile(pathBase+"rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(550, 700);

    //logs (used later)
    Texture textureLog;
    textureLog.loadFromFile(pathBase+"log.png");

    //Axe (for the player)
    Texture textureAxe;
    textureAxe.loadFromFile(pathBase+"axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(670, 820);
    spriteAxe.setRotation(-45);


    //logo
    Texture texturetimberman;
    texturetimberman.loadFromFile(pathBase+"timberman.png");
    Sprite spritetimberman;
    spritetimberman.setTexture(texturetimberman);
    spritetimberman.setScale(3,3);
    spritetimberman.setPosition(490, 0);

    Texture menuTexture;
    menuTexture.loadFromFile(pathBase+"woodBoard.jpeg");
    Sprite menuBackground;
    menuBackground.setTexture(menuTexture);
    menuBackground.setScale(.8,.5);
    menuBackground.setPosition(200,400);

    //bee (used later)
    Texture textureBee;
    textureBee.loadFromFile(pathBase+"bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(-100, 800); //starts offscreen


    //clouds
    Texture textureCloud;
    textureCloud.loadFromFile(pathBase+"cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    //clouds start offscreen
    spriteCloud1.setPosition(-300, 0);
    spriteCloud2.setPosition(-300, 150);
    spriteCloud3.setPosition(-300, 300);

    //branches (used later)
    Texture textureBranch;
    textureBranch.loadFromFile(pathBase+"branch.png");




    //______________________________________________________SOUNDS/MUSIC________________________________________________________________________________________

    //CHOP sound
    SoundBuffer chopbuffer;
    chopbuffer.loadFromFile(pathBase+"chop.wav");
    Sound chopsound;
    chopsound.setBuffer(chopbuffer);

    //SQUASH (die from branch) sound
    SoundBuffer squashbuffer;
    squashbuffer.loadFromFile(pathBase+"death.wav");
    Sound squashsound;
    squashsound.setBuffer(squashbuffer);
    squashsound.setVolume(20);

    //SIREN (die from time) sound
    SoundBuffer timebuffer;
    timebuffer.loadFromFile(pathBase+"out_of_time.wav");
    Sound timesound;
    timesound.setBuffer(timebuffer);

    // BEEP sound (in countdown)
    SoundBuffer beepbuffer;
    beepbuffer.loadFromFile(pathBase+"beep.wav");
    Sound beepsound;
    beepsound.setBuffer(beepbuffer);

    //SONGS
    const int songnum=5;
    Music songs[songnum];
    for(int i=0; i<songnum; i++){
        songs[i].openFromFile(pathBase+"music"+to_string(i+1)+".ogg");
    }




    //______________________________________________________________TEXTS_________________________________________________________________________________________

    //Set the font
    Font font;
    font.loadFromFile(pathBase+"KOMIKAP_.ttf");


    //Text during Pause
    Text pauseText;
    pauseText.setFont(font);
    pauseText.setString(
            "  Press Enter to continue!\n Press F to change speed!\n Press SpaceBar to chop!\n    Press Esc to Exit!\n   Press R to restart!");
    pauseText.setCharacterSize(75);
    pauseText.setFillColor(Color::White);
    pauseText.setPosition(400, 250);


    //Text during start
    Text starttext;
    starttext.setFont(font);
    starttext.setString(
            "Press  Return  to start!\nPress  A  D  to move!\nPress   Space   to chop!\nPress  R  to restart!");
    starttext.setCharacterSize(75);
    starttext.setFillColor(Color{80, 10, 0});
    starttext.setPosition(300, 450);

    RectangleShape menuSpace;
    menuSpace.setSize(Vector2f(300, 70));
    menuSpace.setFillColor(Color::Transparent);
    menuSpace.setOutlineThickness(10);
    menuSpace.setOutlineColor(Color::Black);
    menuSpace.setPosition(600, 460);

    RectangleShape menuEnter;
    menuEnter.setSize(Vector2f(60, 60));
    menuEnter.setFillColor(Color::Transparent);
    menuEnter.setOutlineThickness(10);
    menuEnter.setOutlineColor(Color::Black);
    menuEnter.setPosition(600, 565);

    RectangleShape menuA;
    menuA.setSize(Vector2f(60, 60));
    menuA.setFillColor(Color::Transparent);
    menuA.setOutlineThickness(10);
    menuA.setOutlineColor(Color::Black);
    menuA.setPosition(710, 565);

    RectangleShape menuD;
    menuD.setSize(Vector2f(230, 60));
    menuD.setFillColor(Color::Transparent);
    menuD.setOutlineThickness(10);
    menuD.setOutlineColor(Color::Black);
    menuD.setPosition(645, 670);

    RectangleShape menuR;
    menuR.setSize(Vector2f(60, 60));
    menuR.setFillColor(Color::Transparent);
    menuR.setOutlineThickness(10);
    menuR.setOutlineColor(Color::Black);
    menuR.setPosition(600, 775);



    //Text during gameOver
    Text GameOver;
    GameOver.setFont(font);
    GameOver.setString("         GameOver, \n"
                       "Press Return to continue...");
    GameOver.setCharacterSize(75);
    GameOver.setFillColor(Color::White);
    GameOver.setPosition(500, 500);


    //Showing your score
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score = 0");
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(0, 0);


    //Text showing "3,2,1,Go"
    Text countdowntext;
    countdowntext.setFont(font);
    countdowntext.setString(" ");
    countdowntext.setCharacterSize(100);
    countdowntext.setFillColor(Color::White);
    countdowntext.setPosition(900, 540);


    //Text showing highscore
    Text HighScore;
    HighScore.setFont(font);
    HighScore.setString("Score = 0");
    HighScore.setCharacterSize(50);
    HighScore.setFillColor(Color::White);
    HighScore.setPosition(0, 75);


    //Text showing Speed
    Text overallSpeed;
    overallSpeed.setFont(font);
    overallSpeed.setString("1x Speed");
    overallSpeed.setCharacterSize(50);
    overallSpeed.setFillColor(Color::White);
    overallSpeed.setPosition(1640, 0);


    //Text showing framerate
    Text framerate;
    framerate.setFont(font);
    framerate.setString("");
    framerate.setCharacterSize(50);
    framerate.setFillColor(Color::White);
    framerate.setPosition(1640, 75);


    //Text showing menu
    Text nmenu;
    nmenu.setFont(font);
    nmenu.setString("Press Enter for Menu");
    nmenu.setCharacterSize(50);
    nmenu.setFillColor(Color::White);
    nmenu.setPosition(0, 150);


    //The time bar that decreases over time
    //background
    RectangleShape timeBackground;
    timeBackground.setSize(Vector2f(480, 80));
    timeBackground.setFillColor(Color{211, 211, 211, 100});
    timeBackground.setOutlineThickness(10);
    timeBackground.setOutlineColor(Color::Black);
    timeBackground.setPosition(710, 10);
    //the bar itself
    RectangleShape timeBar;
    timeBar.setSize(Vector2f(480, 80));
    timeBar.setPosition(710, 10);
    timeBar.setFillColor(Color::Green);


    //The dim background in pause or loss
    RectangleShape pauselossBackground;
    pauselossBackground.setSize(Vector2f(1920, 1080));
    pauselossBackground.setFillColor(Color{0, 0, 0, 150});
    pauselossBackground.setOutlineThickness(0);
    pauselossBackground.setPosition(0, 0);

    //background in the menu
    Sprite startbackground;
    startbackground.setTexture(textureBackground);
    startbackground.setPosition(0, 0);



    //__________________________________________________SETTING UP________________________________________________________________________________________


    //set the random
    srand(time(NULL));

    //game types
    bool gameover = false;
    int started = 0;
    int ispaused = 0;

    int countdownclock = 399; //time between each number in the countdown
    int countdown = 4;

    //set up numbers
    int speed = 1;
    int score = 0;
    int highscore = 0; //2.718281828
    double timeRemaining = 8;

    //setup the clouds and bees

    //find if clouds and bee are on screen
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    bool beeActive = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    double beeSpeed = 1.1;


    //set up the branches
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branchPositions[i] = side::NONE;
        branchright[i] = (treeheight / NUM_BRANCHES) * (i + 1);
        branchleft[i] = branchright[i] + 100;
        branches[i].setTexture(textureBranch);
    }


    //The player starts on the left of the screen
    side playerpos = side::LEFT;


    /*start the game*/
    Clock clock;
    int close = 0;

    //Key pressing help
    bool canpause = true;
    bool canstart = true;
    bool canspeed = true;
    bool canscore = true;
    bool canrestart = true;


    //set up the music
    int musicnum = rand()%songnum;
    songs[musicnum].play();

    //_____________________________________________________GAME LOOP___________________________________________________________________________________________________

    while (window.isOpen()) {
        //poll a new event
        Event event;
        window.pollEvent(event);

        //count a new unit of time
        Time dt = clock.restart();

        //Event 1: Escape key, or x button (close the game)
        if (event.type == Event::Closed) {
            if (close > 1) {
                window.close();
            }
            close++;
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
            window.close();
        }


        //if the game is resetted
        if(started==0){
            //_____________________________________________________NOT IN GAME___________________________________________________________________________________________________

            //reset the numbers
            timeRemaining = 8;
            score = 0;
            canpause = false;
            canscore = true;
            gameover = false;
            countdown = 4;
            countdowntext.setString(" ");
            countdownclock=399;
            timeBar.setFillColor(Color::Green);
            timeBar.setSize(Vector2f(480, 80));
            for (int i = 0; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
                branches[i].setPosition(Vector2f(0, (treeheight / NUM_BRANCHES) * (i + 1)));
                branches[i].setTexture(textureBranch);
            }

            //if Enter is pressed, start the game
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                started=1;
                songs[musicnum].stop();
                musicnum = rand()%songnum;
                songs[musicnum].play();
            }
        }
        else {
            //_____________________________________________________IN GAME___________________________________________________________________________________________________

            if (gameover) {
                if (event.type == Event::KeyReleased && event.key.code == Keyboard::Enter) {
                    //change mode to not in game
                    started=0;
                    songs[musicnum].stop();
                    musicnum = rand()%songnum;
                    songs[musicnum].play();
                }
            } else {


                //Event: Enter (pause)
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && canpause) {
                    ispaused++;
                    ispaused %= 2;
                    canpause = false;
                } else if (event.type == Event::KeyReleased && event.key.code == Keyboard::Enter && !canpause) {
                    canpause = true;
                }



                //Event: R (restart)
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::R && canrestart) {
                    gameover = true;
                    canpause = false;
                } else if (event.type == Event::KeyReleased && event.key.code == Keyboard::R && !canrestart) {
                    canrestart = true;
                }




                //Game is not paused
                if (ispaused % 2 == 0) {

                    //If the game is in countdown
                    if (countdown >= 0) {
                        countdownclock++;
                        countdowntext.setString(to_string(countdown));
                        if (countdown == 0) {
                            countdowntext.setString("GO!");
                        }
                        if (countdownclock >= 400) {
                            countdown--;
                            countdownclock = 0;
                            if(countdown>=0) {
                                beepsound.play();
                            }
                        }

                    } else {

                        //actual playing part


                        //GameOver by time running out
                        timeRemaining = min(timeRemaining, 8.0);
                        timeRemaining -= dt.asSeconds();
                        if (timeRemaining <= 0) {
                            timesound.play();
                            gameover = true;

                        }


                        //GameOver by branch squashing
                        if (branchPositions[NUM_BRANCHES - 1] == playerpos) {
                            squashsound.play();
                            gameover = true;
                        }


                        //Change the timeBar
                        timeBar.setSize(Vector2f(60 * timeRemaining, 80));
                        timeBar.setFillColor(Color::Green);
                        if (timeRemaining <= 16 / 3) {
                            timeBar.setFillColor(Color::Yellow);
                        }
                        if (timeRemaining <= 8 / 3) {
                            timeBar.setFillColor(Color::Red);
                        }


                        //Move the player to the left
                        if (event.type == Event::KeyPressed &&
                            (event.key.code == Keyboard::Left || event.key.code == Keyboard::A)) {
                            spritePlayer.setPosition(550, 700);
                            spriteRIP.setPosition(550, 700);
                            spriteAxe.setPosition(670, 820);
                            spriteAxe.setRotation(-45);
                            playerpos = side::LEFT;
                        }


                        //Move the player to the right
                        if (event.type == Event::KeyPressed &&
                            (event.key.code == Keyboard::Right || event.key.code == Keyboard::D)) {
                            spritePlayer.setPosition(1250, 700);
                            spriteRIP.setPosition(1250, 700);
                            spriteAxe.setPosition(1170, 710);
                            spriteAxe.setRotation(45);
                            playerpos = side::RIGHT;
                        }


                        //Event: Space (when you hit the tree)
                        if (((event.type == Event::KeyPressed && event.key.code == Keyboard::Space) ||
                             event.type == Event::MouseButtonPressed) && canscore) {
                            //make the axe flat
                            spriteAxe.setRotation(0);
                            if (spritePlayer.getPosition() == Vector2f(550, 700)) {
                                //no need to change the position, the rotation looks normal
//                        spriteAxe.setPosition(670, 820);
                            } else {
                                //move the axe down a bit to make it look mor real
                                spriteAxe.setPosition(1110, 810);
                            }

                            //increase score
                            score++;

                            //gain some extra time
                            timeRemaining += 0.2;

                            //move the branches down
                            for (int i = NUM_BRANCHES; i > 0; i--) {
                                branchPositions[i] = branchPositions[i - 1];
                            }


                            //create a new branch, using random numbers
                            int randnum = rand() % 3;
                            if (randnum == 0 || score % 2 == 0) {
                                branchPositions[0] = side::NONE;
                            } else if (randnum == 1) {
                                branchPositions[0] = side::LEFT;
                            } else {

                                branchPositions[0] = side::RIGHT;
                            }

                            //flying logs animation
                            flyinglogs.emplace_back();
                            flyinglogs.back().setPosition(810, treeheight);
                            flyinglogs.back().setTexture(textureLog);
                            if (playerpos == side::LEFT) {
                                logsidex.push_back(1);
                            } else {
                                logsidex.push_back(-1);
                            }
                            logspeed.push_back((rand() % 4800) + 200);
                            logsidey.push_back((rand() % 200) / 100.0 - 10);



                            chopsound.play();

                            //score delay
                            canscore = false;


                        }

                        //When you release the space key
                        if ((event.type == Event::KeyReleased && event.key.code == Keyboard::Space) ||
                            event.type == Event::MouseButtonReleased) {

                            //reset the axe to the original position
                            if (spritePlayer.getPosition() == Vector2f(550, 700)) {
//                        spriteAxe.setPosition(670, 820);
                                spriteAxe.setRotation(-45);
                            } else {
                                spriteAxe.setPosition(1170, 710);
                                spriteAxe.setRotation(45);
                            }


                            canscore = true;
                        }

                        //update the highscore
                        highscore = max(highscore, score);
                    }
                }
            }
        }
        //_____________________________________________________UPDATE THINGS___________________________________________________________________________________________________

        //Event: F (change background speed)
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::F && canspeed) {
            speed *= 2;
            if (speed == 128) {
                speed = 1;
            }
            canspeed = false;
        } else if (event.type == Event::KeyReleased && event.key.code == Keyboard::F) {
            canspeed = true;
        }


        //update the bee
        if (!beeActive) {
            //If the bee is off the screen
            beeSpeed = (rand() % 200) + 200;
            double beeheight = (rand() % 1000) + 1;
            spriteBee.setPosition(2000, beeheight);
            beeActive = true;

        }
        else{
            //The bee is already on the screen
            spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * (dt.asSeconds()) * speed),
                                  spriteBee.getPosition().y);

            // if the bee goes off the screen
            if (spriteBee.getPosition().x < -100) {
                beeActive = false;
            }
        }

        //Do the same with cloud 1
        if (!cloud1Active) {
            cloud1Speed = (rand() % 200) + 10;
            double cloudheight = (rand() % 200) + 1;
            spriteCloud1.setPosition(2000, cloudheight);
            cloud1Active = true;
        }
        else {
            spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x - (cloud1Speed * (dt.asSeconds()) * speed),
                    spriteCloud1.getPosition().y);
            if (spriteCloud1.getPosition().x < -300) {
                cloud1Active = false;
            }
        }


        //Do the same with cloud 2
        if (!cloud2Active) {
            cloud2Speed = (rand() % 200) + 10;
            double cloudheight = (rand() % 200) + 1;
            spriteCloud2.setPosition(2000, cloudheight);
            cloud2Active = true;
        }
        else {
            spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x - (cloud2Speed * (dt.asSeconds()) * speed),
                    spriteCloud2.getPosition().y);
            if (spriteCloud2.getPosition().x < -300) {
                cloud2Active = false;
            }
        }

        //do the same with cloud 3
        if (!cloud3Active) {
            cloud3Speed = (rand() % 200) + 10;
            double cloudheight = (rand() % 200) + 1;
            spriteCloud3.setPosition(2000, cloudheight);
            cloud3Active = true;
        } else {
            spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x - (cloud3Speed * (dt.asSeconds()) * speed),
                    spriteCloud3.getPosition().y);
            if (spriteCloud3.getPosition().x < -300) {
                cloud3Active = false;
            }
        }

        //Update the animated logs' positions
        for (int i = 0; i < flyinglogs.size(); i++) {

            //move the log
            flyinglogs[i].setPosition(
                    flyinglogs[i].getPosition().x + logsidex[i] * (logspeed[i] * (dt.asSeconds())),
                    flyinglogs[i].getPosition().y + logsidey[i]);
            logsidey[i] += 0.1;

            //spin the log
            flyinglogs[i].setRotation(flyinglogs[i].getRotation() + 1);

            //if the logs are off the screen
            if (flyinglogs[i].getPosition().x < -100 || flyinglogs[i].getPosition().x > 2000) {
                flyinglogs.erase(flyinglogs.begin() + i);
                logsidex.erase(logsidex.begin() + i);
                logsidey.erase(logsidey.begin() + i);
                logspeed.erase(logspeed.begin() + i);
            }
        }

        //_____________________________________________________DRAW THE SCENE__________________________________________________________________________________________________


        window.clear();


        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteBee);
        window.draw(spriteTree);

        if (!gameover) {
            window.draw(spritePlayer);
            window.draw(spriteAxe);
        }

        //draw all the branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
            if (branchPositions[i] != side::NONE) {
                if (branchPositions[i] == side::LEFT) {
                    branches[i].setPosition(810, branchleft[i]);
                    branches[i].setRotation(180);
                } else {
                    branches[i].setPosition(1110, branchright[i]);
                    branches[i].setRotation(0);
                }
                window.draw(branches[i]);
            }

        }

        //draw all the animating logs
        for (int i = flyinglogs.size() - 1; i >= 0; i--) {
            window.draw(flyinglogs[i]);
        }

        //show the texts in game
        overallSpeed.setString(to_string(speed) + "x Speed");
        framerate.setString(to_string(1 / dt.asSeconds()));
        scoreText.setString("Score = " + to_string(score));
        HighScore.setString("High score = " + to_string(highscore));


        if (countdown >= 0) {
            window.draw(countdowntext);
        }
        if (ispaused % 2 == 1 && !gameover) {
            window.draw(pauselossBackground);
            window.draw(menuBackground);
            window.draw(starttext);
            window.draw(menuA);
            window.draw(menuD);
            window.draw(menuR);
            window.draw(menuEnter);
            window.draw(menuSpace);
        }
        //Add these things over the pause screen
        window.draw(overallSpeed);
        window.draw(framerate);
        window.draw(scoreText);
        window.draw(timeBackground);
        window.draw(timeBar);


        if (gameover) {
            window.draw(spriteRIP);
            window.draw(pauselossBackground);
            window.draw(GameOver);
        }

        //Add highscore over everything
        window.draw(HighScore);

        if(started==0) {
            window.draw(startbackground);
            window.draw(menuBackground);
            window.draw(starttext);
            window.draw(menuA);
            window.draw(menuD);
            window.draw(menuR);
            window.draw(menuEnter);
            window.draw(menuSpace);
            window.draw(spritetimberman);
        }


        window.display();
    }

    return 0;
}