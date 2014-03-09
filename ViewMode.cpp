/*
  @file ViewMode.cpp
  ViewMode.cpp -- View mode handles the viewing.

  @author Thomas Liu
  @date March 2014
*/

#include "ViewMode.hpp"

ViewMode::ViewMode(std::string filePath)
{
     if(not zptFile.parseFile(filePath)){
	  std::cout << "Error opening file." << std::endl;
     }
     modes = sf::VideoMode::getFullscreenModes();
     if (zptFile.presentationInfo.prefSize.x == modes[0].width and zptFile.presentationInfo.prefSize.y == modes[0].height){
	  window.create(modes[0], filePath, sf::Style::Fullscreen);
     }
     else{
	  window.create(sf::VideoMode((unsigned int)zptFile.presentationInfo.prefSize.x, (unsigned int)zptFile.presentationInfo.prefSize.y), filePath);
     }
     
     windowSize = window.getSize();
     window.setFramerateLimit(zptFile.presentationInfo.framerate);
     backgroundPath = zptFile.presentationInfo.imagePath;
}

bool ViewMode::init()
{
     if (not backgroundTexture.loadFromFile(backgroundPath)){
	  std::cout << "Error loading image." << std::endl;
	  return false;
     }
     backgroundTexture.setSmooth(true);
     //zoomer.init(sf::Vector2f(2360,3000), sf::Vector2f(windowSize.x,
     //windowSize.y));

     std::cout << zptFile.slides[0].center.x << std::endl;
     
     zoomer.init(zptFile.slides[0].center, zptFile.slides[0].size, zptFile.presentationInfo.framerate, zptFile.slides[0].angle);
     
     background.setTexture(backgroundTexture);

     currentSlideNumber = 1;
     
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

			 std::cout << zptFile.slides[currentSlideNumber].center.x << std::endl;
			 std::cout << zptFile.slides[currentSlideNumber].center.y << std::endl;
			 
			 zoomer.setAndCalculateTarget(zptFile.slides[currentSlideNumber].center,
						      zptFile.slides[currentSlideNumber].speed,
						      zptFile.slides[currentSlideNumber].percent,
						      zptFile.slides[currentSlideNumber].angle,
						      zptFile.slides[currentSlideNumber].size);
			 
			 //sf::Vector2f currentSize = zoomer.getCurrentView().getSize();
			 //zoomer.setAndCalculateTarget(sf::Vector2f(2360,4000),
			 //1500, 0.3, -45., sf::Vector2f(currentSize.x
			 //* 0.5, currentSize.y * 0.5));

			 currentSlideNumber = (currentSlideNumber + 1) % (zptFile.slides.size());
		    }
	       }
	  }
	  
	  window.clear(sf::Color::Black);
	  
	  window.draw(background);
	  
	  if (zoomer.hasMoves()){
	       window.setView(zoomer.popNextView());
	       //sf::View theView = zoomer.getCurrentView();
	       //std::cout << theView.getSize().x << "," << theView.getSize().y << std::endl;
	       // if (not zoomer.hasMoves()){
	       // 	    zoomer.setCenter(sf::Vector2f(2360,4000));
	       // }
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

