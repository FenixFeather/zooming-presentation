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
     //window.create(modes[0], "Presentation",sf::Style::Fullscreen);
     window.create(modes[0], "Presentation");
     windowSize = window.getSize();
     window.setFramerateLimit(60);
     backgroundPath = imagePath;
}

bool ViewMode::init()
{
     if (not backgroundTexture.loadFromFile(backgroundPath)){
	  std::cout << "Error loading image." << std::endl;
	  return false;
     }
     zoomer.init(sf::Vector2f(2360,3000), sf::Vector2f(windowSize.x, windowSize.y));
     background.setTexture(backgroundTexture);
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
	       else if (event.type == sf::Event::MouseButtonPressed){
		    if (event.mouseButton.button == sf::Mouse::Left){
			 zoomer.setAndCalculateTarget(sf::Vector2f(2360,4000), 500);
		    }
	       }
	  }
	  
	  window.clear(sf::Color::Black);
	  
	  window.draw(background);

	  if (zoomer.hasMoves()){
	       window.setView(zoomer.popNextView());
	       sf::View theView = zoomer.getCurrentView();
	       std::cout << theView.getSize().x << "," << theView.getSize().y << std::endl;
	  }
	  else{
	       window.setView(zoomer.getCurrentView());
	  }
	  
	  window.display();
     }
}

void ViewMode::close()
{
}

