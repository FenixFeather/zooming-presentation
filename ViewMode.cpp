/*
  @file ViewMode.cpp
  ViewMode.cpp -- View mode handles the viewing.

  @author Thomas Liu
  @date March 2014
*/

#include "ViewMode.hpp"

ViewMode::ViewMode(std::string filePath, bool windowedMode)
{
     if(not zptFile.parseFile(filePath)){
	  std::cout << "Error opening file." << std::endl;
     }
     
     modes = sf::VideoMode::getFullscreenModes();

     aspectDifference = 0;
     
     std::string title = zptFile.presentationInfo.presentationTitle;

     windowTitle = title + " - " + filePath;

     windowed = windowedMode;

     createWindow();
     
     // if (zptFile.presentationInfo.prefSize.x == modes[0].width and zptFile.presentationInfo.prefSize.y == modes[0].height){
	  
     // }
     // else{
	  
     // }
     
     windowSize = window.getSize();
     window.setFramerateLimit(zptFile.presentationInfo.framerate);
     backgroundPath = zptFile.presentationInfo.imagePath;
}

ViewMode::~ViewMode()
{
}

void ViewMode::createWindow()
{
     desktop = sf::VideoMode::getDesktopMode();

     sf::Vector2f resolution = zptFile.presentationInfo.prefSize;
     
     if (windowed){
	  while (resolution.x > desktop.width or resolution.y > desktop.height){
	       resolution.x -= 100;
	       resolution.y -= 100;
	  }
	  window.create(sf::VideoMode(resolution.x, resolution.y), windowTitle);
	  
     }
     else{
	  //window.create(modes[0], windowTitle, sf::Style::Fullscreen);
	  window.create(sf::VideoMode(desktop.width, desktop.height, desktop.bitsPerPixel), windowTitle, sf::Style::Fullscreen);
	  //window.setVerticalSyncEnabled(true);
	  aspectDifference = compareAspectRatios(sf::Vector2f(desktop.width, desktop.height), resolution);
     }
     window.setMouseCursorVisible(false);
}

float ViewMode::compareAspectRatios(sf::Vector2f first, sf::Vector2f second)
{
     if (std::abs((first.x/first.y) - (second.x/second.y)) < 0.01) return 0;
     return (first.x/first.y) - (second.x/second.y);
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
	       else if (event.type == sf::Event::KeyPressed)
	       {
		    if (event.key.code == sf::Keyboard::Escape)
		    {
			 window.close();
		    }
		    if (event.key.code == sf::Keyboard::Return)
		    {
			 if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)){
			      windowed = not windowed;
			      createWindow();
			 }
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

	  if (not windowed and aspectDifference != 0){
	       processView();
	  }
	  window.display();
     }
}

void ViewMode::processView()
{
     //If the screen is less wide than the desired rectangle shape,
     //we're gonna need to expand the view to fit the same aspect
     //ratio as the screen, but add black bars to hide the extra.

     float desktopAspectRatio = desktop.width / desktop.height;
     

     const sf::View& windowView = window.getView();
	  
     sf::Vector2f tempSize = windowView.getSize();

     float& dimensionToIncrease = tempSize.y;
     float& otherDimension = tempSize.x;
     
     if (aspectDifference > 0){
	  float& dimensionToIncrease = tempSize.x;
	  float& otherDimension = tempSize.y;
     }
     float oldHeight = dimensionToIncrease;
	  
     //Increase the vertical view.
     dimensionToIncrease = otherDimension / (desktopAspectRatio);
	  
     //Fraction of pixels that are new (and therefore extra).
     // float newFraction = (dimensionToIncrease - oldHeight)/dimensionToIncrease;

     //Make the new view.
     sf::View newView(windowView.getCenter(), tempSize);
	  
     //The viewport is a rectangle with coordinates at 0, half
     //the fraction that are extra, width full, and reduce the
     //extra height.
     //sf::FloatRect viewPort(0, newFraction/2, 1, 1 - newFraction);//1 - newFraction/2);

     newView.setRotation(windowView.getRotation());
	  
     //newView.setViewport(viewPort);
     window.setView(newView);

}

void ViewMode::close()
{
}

