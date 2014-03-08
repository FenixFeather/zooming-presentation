/*
  @file Zoomer.cpp
  Zoomer.cpp -- Helps calculate movement and returns views.

  @author Thomas Liu
  @date March 2014
*/

#include "Zoomer.hpp"

Zoomer::Zoomer(sf::Vector2f center, sf::Vector2f size, unsigned int framerate)
{
     //Initialize the view to starting position.
     view.setCenter(center);
     view.setSize(size);
     fps = framerate;
}

void Zoomer::setCenter(sf::Vector2f center)
{
     view.setCenter(center);
}

sf::View Zoomer::getCurrentView() const
{
     return view;
}

void Zoomer::setAndCalculateTarget(sf::Vector2f newTarget, unsigned int newSpeed);
{
     target = newTarget;
     previousView.setCenter(view.getCenter());
     previousView.setSize(view.getSize());
     previousView.setRotation(view.getRotation());
     calculateViewMoves(newSpeed);
}

sf::View Zoomer::popNextView()
{
     ViewMove nextMove = moveQueue.front();
     view.move(nextMove.translationVector);
     view.rotate(nextMove.translationAngle);
     view.setSize(nextMove.zoomVector);
     moveQueue.pop();
     return view;
}

bool Zoomer::hasMoves()
{
     return not moveQueue.empty();
}

void Zoomer::calculateViewMoves(unsigned int speed)
{
     //Calculate the translations.
     std::vector<float> yMoves = calculateCoordinateTranslations(target.y - view.getCenter().y, 0.1);
     std::vector<float> xMoves = calculateCoordinateTranslations(target.x - view.getCenter(.x, 0.1));

     //Fill in coordinates so that they match up.
     while (yMoves.size() > xMoves.size()){
	  xMoves.push_back(0.);
     }
     while (xMoves.size() > yMoves.size()){
	  yMoves.push_back(0.);
     }

     //Push moves onto the queue.
     for (size_t ii = 0; ii < yMoves.size(); ii++){
	  moveQueue.push(ViewMove(sf::Vector2f(xMoves[ii], yMoves[ii]), 0, sf::Vector2f(0,0)));
     }
}

std::vector<float> Zoomer::calculateCoordinateTranslations(float finalPosition,
							   float initialPosition,
							   float percentSmooth)
{
     //Used to calculate number of frames whose movement will be smoothed.
     float percentSmooth = percentSmooth;
     
     //Get the total distance we need to travel so we can do
     //calculations properly.
     int deltaY = int(round(finalPosition - initialPosition));
     
     //Calculate, on average, how many pixels we need to move per
     //frame. If we're smoothing the movement curve, then this will be
     //the maximum instantaneous velocity.
     float dy = float(speed) / float(fps);

     //Find how many total movement vectors we need to produce.
     if (std::abs(dy) > 1){
	  dy = round(dy);
	  unsigned int totalYMovesNeeded = std::abs(deltaY/dy);
	  unsigned int smoothFrames = typedef unsigned int(percentSmooth * totalYMovesNeeded);
     }

     //Generate vector of y moves.
     std::vector<float> yMoves(totalYMovesNeeded, dy);

     //Smooth out the first segment.
     for (unsigned int ii=0; ii < smoothFrames; ii++){
	  yMoves[ii] = round(float(ii)/float(smoothFrames));
     }

     //Figure out how many pixels we have to make up for.
     float lostPixels = float(deltaY) - sumFloatVectorRange(yMoves, 0, smoothFrames);

     //Determine how many extra frames we need to move in the
     //middle. Multiply by 2 because the other end will lose pixels too.
     unsigned int additionalMovesNeeded = int(round(lostPixels / dy)) * 2;

     //Add the extra frames of movement.
     for (unsigned int ii = 0; ii < additionalMovesNeeded; i++){
	  yMoves.push_back(dy);
     }

     //Update totalYMovesNeeded to reflect new frames
     totalYMovesNeeded += additionalMovesNeeded;

     //Finally, smooth out the last part of the curve.
     for (unsigned int ii = totalYMovesNeeded - smoothFrames; ii < totalYMovesNeeded;ii++){
	  yMoves[ii] = dy - round(float(ii)/float(smoothFrames));
     }
     return yMoves;
}

float Zoomer::sumFloatVectorRange(std::vector<float> theVector, size_t start, size_t end)
{
     float result = 0;
     for (size_t ii = start; ii < end; ii++){
	  result += theVector[ii];
     }
     return result;
}
