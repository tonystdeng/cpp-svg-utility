#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>

#include "svglib.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


//const int PI=3.141592653589793238462643383279502884197;    what the fuck was i thinking about, i fucking wasted 4 hours
//const double PI=3.141592653589793238462643383279502884197;// wasted on finding what is going on in another file, i am brain dead
const int width=1600, height=800;
const int hw=width/2, hh=height/2;
const sf::Time fpsupdateInterval = sf::seconds(1.0f);




class Game
{
    public:
    sf::RenderWindow *window;
    sf::RectangleShape dividLine;
    
    sf::Clock clock;
    sf::Time elapsed, fpsupdateCountdown;
    float fps;
    std::ostringstream fpsloader;
    sl::svginfo fileinfo;
    //sf::VertexArray svgshape{sf::LineStrip};
    std::vector<sf::VertexArray> svgshapes,simplifieds;

    Game(char *);~Game();

    void main();
    void updatefps();
    void common();
    bool checkEvent();
};

#endif