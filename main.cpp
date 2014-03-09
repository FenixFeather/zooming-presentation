/*
  main.cpp -- An SFML app for making zooming presentations.
*/

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "AbstractMode.hpp"
#include "ViewMode.hpp"

int main()
{
     AbstractMode* viewWindow = new ViewMode("test.xml");
     viewWindow->init();
     viewWindow->run();
     delete viewWindow;
     return 0;
}
// {
//      std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

//      // sf::Window window(sf::VideoMode(800,600), "Hello World");
//      sf::RenderWindow window(modes[0], "Hello World",sf::Style::Fullscreen);

//      sf::Vector2u windowSize = window.getSize();

//      sf::View mainView(sf::Vector2f(2360,3000), sf::Vector2f(windowSize.x, windowSize.y));
     
//      window.setFramerateLimit(60);
     
//      sf::Texture backgroundTexture;
     
//      if (not backgroundTexture.loadFromFile("revolucionsmall.png")){
// 	  std::cout << "Error loading image" << std::endl;
//      }

//      while (window.isOpen()){
// 	  //sf event handler
// 	  sf::Event event;
// 	  while (window.pollEvent(event)){
// 	       if (event.type == sf::Event::Closed){
// 		    window.close();
// 	       }
// 	  }
	  
// 	  window.clear(sf::Color::Black);
	  
// 	  sf::Sprite background;
	  
// 	  background.setTexture(backgroundTexture);
     
// 	  window.draw(background);

// 	  window.setView(mainView);
	  
// 	  window.display();
//      }

     
     
//      return 0;
// }
