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
	  filePath = path;

	  if (not (readMetaData() and readSlides())){
	       std::cout << "Corrupt file" << std::endl;
	       return false;
	  }
	  return true;
     }

     bool ZptReader::readMetaData()
     {
	  const char* cFilePath = filePath.c_str();

	  std::ifstream file;
	  file.open(cFilePath);

	  std::stringstream buffer;
	  buffer << file.rdbuf();
	  file.close();
	  std::string content(buffer.str());
	  doc.parse<0>(&content[0]);

	  // std::ifstream file(filePath.c_str());
	  // std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	  // buffer.push_back('\0');
	  // doc.parse<0>(&buffer[0]);

	  rootNode = doc.first_node("presentation");

	  rapidxml::xml_node<>* metaNode = rootNode->first_node("Metadata");
	  presentationInfo.presentationTitle = std::string(metaNode->first_node("Title")->value());
	  presentationInfo.imagePath = std::string(metaNode->first_node("ImagePath")->value());
	  presentationInfo.framerate = atoi(metaNode->first_node("Framerate")->value());
	  float x = atof((metaNode->first_node("PreferredSize")->first_attribute("x")->value()));
	  float y = atof((metaNode->first_node("PreferredSize")->first_attribute("y")->value()));
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
	  const char* cFilePath = filePath.c_str();

	  std::ifstream file;
	  file.open(cFilePath);

	  std::stringstream buffer;
	  buffer << file.rdbuf();
	  file.close();
	  std::string content(buffer.str());
	  doc.parse<0>(&content[0]);

	  //for (rapidxml::xml_node<>* slideNode = rootNode->first_node("slide"); slideNode; slideNode->next_sibling()){
	  rapidxml::xml_node<>* slideNode = rootNode->first_node("slide");
	  while(slideNode != NULL){
	       Slide currentSlide;

	       float centerX = atof(slideNode->first_node("Center")->first_attribute("x")->value());
	       float centerY = atof(slideNode->first_node("Center")->first_attribute("y")->value());

	       currentSlide.center = sf::Vector2f(centerX, centerY);

	       currentSlide.speed = atoi(slideNode->first_node("Speed")->value());

	       currentSlide.percent = atof(slideNode->first_node("SmoothPercentage")->value());

	       currentSlide.angle = atof(slideNode->first_node("Angle")->value());
	       
	       float sizeX = atof(slideNode->first_node("Size")->first_attribute("x")->value());
	       float sizeY = atof(slideNode->first_node("Size")->first_attribute("y")->value());

	       currentSlide.size = sf::Vector2f(sizeX, sizeY);
	       slides.push_back(currentSlide);
	       slideNode = slideNode->next_sibling();
	  }
	  return true;
     }
}
