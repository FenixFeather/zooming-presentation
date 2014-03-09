#ifndef ZPTREADER_HPP
#define ZPTREADER_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidxml.hpp"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

namespace zpt
{
     class ZptReader{
     public:
	  /**
	   * Default constructor.
	   */
	  ZptReader();

	  /**
	   * Destroyer
	   */
	  ~ZptReader();
	  
	  /**
	   * Open the file, obtain all the data.
	   * @return True if successful, False if failed.
	   */
	  bool parseFile(std::string path);

	  /**
	   * Struct for holding presentation information.
	   */
	  struct Metadata{
	       std::string imagePath;
	       unsigned int framerate;
	       sf::Vector2f prefSize;
	  };
	  
	  /**
	   * Struct for holding slide information.
	   */
	       struct Slide{
		    sf::Vector2f center;
		    unsigned int speed;
		    float percent;
		    float angle;
		    sf::Vector2f size;
	       };

	  /**
	   * Vector of all the slides.
	   */
	  std::vector<Slide> slides;

	  /**
	   * The struct holding presentation information.
	   */
	  Metadata presentationInfo;
     
     private:
	  rapidxml::xml_document<> doc;

	  rapidxml::xml_node<>* rootNode;

	  std::string filePath;
     
	  bool readMetaData();
     
	  bool readSlides();
     
     };
};

#endif //ZPTREADER_HPP
