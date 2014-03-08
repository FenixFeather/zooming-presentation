/*
  @file ViewMode.cpp
  ViewMode.cpp -- View mode handles the viewing.

  @author Thomas Liu
  @date March 2014
*/

#include "ViewMode.hpp"

ViewMode::ViewMode(std::string imagePath = "")
{
     modes = sf::VideoMode::getFullscreenModes();
     window.create(modes[0], "Presentation",sf::Style::Fullscreen);
     windowSize = window.getSize();
     mainView.setCenter(sf::Vector2f(2360,3000));
     mainView.setSize(sf::Vector2f(windowSize.x, windowSize.y));
     window.setFramerateLimit(60);
     backgroundPath = imagePath;
}

bool ViewMode::init()
{
     if (not backgroundTexture.loadFromFile(backgroundPath)){
	  std::cout << "Error loading image." << std::endl;
	  return false;
     }
     return true;
}

void ViewMode::run()
{
     while (window.isOpen()){
	  //sf event handler
	  while (window.pollEvent(event)){
	       if (event.type == sf::Event::Closed){
		    window.close();
	       }
	  }
	  
	  window.clear(sf::Color::Black);
	  
	  background.setTexture(backgroundTexture);
     
	  window.draw(background);

	  window.setView(mainView);
	  
	  window.display();
     }
}

void ViewMode::close()
{
}

