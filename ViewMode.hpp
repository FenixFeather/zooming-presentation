#ifndef VIEWMODE_HPP
#define VIEWMODE_HPP

#include "AbstractMode.hpp"
#include <vector>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class ViewMode : public AbstractMode{
public:
     /**
      * Constructor for ViewMode. Initializes member variables.
      */
     ViewMode(std::string);
     
     /**
      * Create the window, sets video mode, loads picture.
      * @return Return True if successful, false if not.
      */
     bool init();

     /**
      * Contain main loop. Handle events.
      */
     void run();

     /**
      * Clean up.
      */
     void close();
     
private:
     /**
      * Holds a list of all fullscreen video modes.
      */
     std::vector<sf::VideoMode> modes;

     /**
      * Main window.
      */
     sf::RenderWindow window;

     /**
      * Main view.
      */
     sf::View mainView;

     /**
      * Size of window (x,y).
      */
     sf::Vector2u windowSize;

     /**
      * Main texture used as background.
      */
     sf::Texture backgroundTexture;

     /**
      * Holds path to background image.
      */
     std::string backgroundPath;
     
     /**
      * Sprite that displays the background texture.
      */
     sf::Sprite background;
     
     /**
      * Event handler.
      */
     sf::Event event;
     
};


#endif //VIEWMODE_HPP
