/*
  ZptReader.cpp -- A class for reading xml files for presentations and
  holding their data.
*/

#include "ZptReader.hpp"

namespace zpt
{
     ZptReader::ZptReader()
     {
	  rootNode = NULL;
     }

     ZptReader::~ZptReader()
     {
	  delete rootNode;
     }
     
     bool ZptReader::parseFile(std::string path)
     {
	  if (not (readMetaData() and readSlides())){
	       std::cout << "Corrupt file" << std::endl;
	       return false;
	  }
	  return true;
     }

     bool ZptReader::readMetaData()
     {
	  std::ifstream file(filePath.c_str());
	  std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	  buffer.push_back('\0');
	  doc.parse<0>(&buffer[0]);
	  rootNode = doc.first_node();

	  rapidxml::xml_node<>* metaNode = rootNode->first_node("Metadata");
	  presentationInfo.imagePath = std::string(metaNode->first_node("Image Path")->value());
	  presentationInfo.framerate = atoi(metaNode->first_node("Framerate")->value());
	  float x = atof((metaNode->first_node("Preferred Size")->first_node("x")->value()));
	  float y = atof((metaNode->first_node("Preferred Size")->first_node("y")->value()));
	  presentationInfo.prefSize = sf::Vector2f(x,y);
	  //delete metaNode;
	  return true;
     }

     bool ZptReader::readSlides()
     {
	  // std::ifstream file(filePath.c_str());
	  // std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	  // buffer.push_back('\0');
	  // doc.parse<0>(&buffer[0]);
	  // rootNode = doc.first_node("document");
	  
	  for (rapidxml::xml_node<>* slideNode = rootNode->first_node("Slide");
	       slideNode;
	       slideNode->next_sibling()){
	       Slide currentSlide;

	       float centerX = atof(slideNode->first_node("Center")->first_node("x")->value());
	       float centerY = atof(slideNode->first_node("Center")->first_node("y")->value());
	       
	       currentSlide.center = sf::Vector2f(centerX, centerY);

	       currentSlide.speed = atoi(slideNode->first_node("Speed")->value());

	       currentSlide.percent = atof(slideNode->first_node("Smooth Percentage")->value());

	       float sizeX = atof(slideNode->first_node("Size")->first_node("x")->value());
	       float sizeY = atof(slideNode->first_node("Size")->first_node("y")->value());

	       currentSlide.size = sf::Vector2f(sizeX, sizeY);
	       slides.push_back(currentSlide);
	  }
	  return true;
     }
}
