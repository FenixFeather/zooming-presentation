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
      * @brief Constructor for ViewMode.
      * Initializes member variables.
      * @param filePath The path to the xml file.
      * @param windowedMode
      */
     ViewMode(std::string filePath = "", bool windowedMode=false);

     /**
      * Destroyer for ViewMode.
      */
     ~ViewMode();
     
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
      * @brief Handles the processing for window creation.
      * @param windowTitle The title for the window.
      */
     void createWindow();

     /**
      * @brief Tests aspect ratios for difference.
      * @param first The first vector.
      * @param second The second vector.
      * @return 0 if they have the same aspect ratio, first - second
      * if they have different aspect ratios.
      */
     float compareAspectRatios(sf::Vector2f first, sf::Vector2f second);

     /**
      * @brief Helps keep the view aspect ratio consistent.
      * Puts black bars to help maintain consistent view across
      * different setups.
      */
     void processView();
     
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

     /**
      * @brief Keeps track of whether the window is windowed.
      */
     bool windowed;

     /**
      * @brief Keeps track of difference in aspect ratio.
      */
     float aspectDifference;

     /**
      * @brief Keeps track of the desktop.
      */
     sf::VideoMode desktop;

     /**
      * @brief Keeps track of the Window Title.
      */
     std::string windowTitle;
};


#endif //VIEWMODE_HPP
