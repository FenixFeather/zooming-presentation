#ifndef VIEWMODE_HPP
#define VIEWMODE_HPP

#include "AbstractMode.hpp"
#include <vector>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Zoomer.hpp"
#include "ZptReader.hpp"

class ViewMode : public AbstractMode{
public:
     /**
      * Constructor for ViewMode. Initializes member variables.
      * @param filePath The path to the xml file.
      */
     ViewMode(std::string filePath);
     
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
      * Size of window (x,y).
      */
     sf::Vector2u windowSize;

     /**
      * Responsible for zooming around, getting views.
      */
     Zoomer zoomer;
     
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

     /**
      * File handler.
      */
     zpt::ZptReader zptFile;

     /**
      * Keeps track of the current slide number.
      */
     unsigned int currentSlideNumber;
};


#endif //VIEWMODE_HPP
