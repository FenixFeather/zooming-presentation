/*
  @file Zoomer.cpp
  Zoomer.cpp -- Helps calculate movement and returns views.

  @author Thomas Liu
  @date March 2014
*/

#include "Zoomer.hpp"

Zoomer::Zoomer()
{
     view.setCenter(sf::Vector2f(0,0));
     view.setSize(sf::Vector2f(1,1));
     fps = 60;
}

Zoomer::Zoomer(sf::Vector2f center, sf::Vector2f size, unsigned int framerate)
{
     //Initialize the view to starting position.
     view.setCenter(center);
     view.setSize(size);
     defaultSize = size;
     fps = framerate;
}

void Zoomer::init(sf::Vector2f center, sf::Vector2f size, unsigned int framerate, float angle)
{
     //Initialize the view to starting position.
     view.setCenter(center);
     view.setSize(size);
     defaultSize = size;
     fps = framerate;
     view.setRotation(angle);
}

void Zoomer::setCenter(sf::Vector2f center)
{
     view.setCenter(center);
}

void Zoomer::setSize(sf::Vector2f size)
{
     view.setSize(size);
}

void Zoomer::setFramerate(unsigned int framerate)
{
     fps = framerate;
}

sf::View Zoomer::getCurrentView() const
{
     return view;
}

void Zoomer::setAndCalculateTarget(sf::Vector2f newTarget,
				   unsigned int newSpeed,
				   float percent,
				   float newAngle,
				   sf::Vector2f newSize)
{
     target = newTarget;
     
     previousView.setCenter(view.getCenter());
     previousView.setSize(view.getSize());
     previousView.setRotation(view.getRotation());

     percentSmooth = percent;
     targetAngle = newAngle;
     targetSize = newSize;

     while(not moveQueue.empty()){
	  moveQueue.pop();
     }
     
     calculateViewMoves(newSpeed);
}

sf::View Zoomer::popNextView()
{
     ViewMove nextMove = moveQueue.front();
     //std::cout << nextMove.translationVector.x << "," << nextMove.translationVector.y << std::endl;

     //Peek ahead to make sure we are not overshooting.
     // if (std::abs(view.getCenter().y + nextMove.translationVector.y - target.y)  (previousView.getCenter().y - target.y > 0) or
     // 	 (view.getCenter().x + nextMove.translationVector.x - target.x > 0) != (previousView.getCenter().x - target.x > 0)){
     
     
     if (not willOvershoot(view.getCenter() + nextMove.translationVector)){
	  view.move(nextMove.translationVector);
     }
     else{
	  std::cout << "diff pos " << (target - view.getCenter()).x << "," << (target - view.getCenter()).y << std::endl;
	  std::cout << "diff size " << (targetSize - view.getSize()).x << "," << (targetSize - view.getSize()).y << std::endl;
	  std::cout << "diff angle " << targetAngle - view.getRotation() << std::endl;
     }
     view.rotate(nextMove.rotationAngle);
     view.setSize(nextMove.zoomVector);
     moveQueue.pop();
     
     return view;
}

bool Zoomer::willOvershoot(sf::Vector2f futurePos)
{
     //sf::Vector2f currentPos = view.getCenter();
     sf::Vector2f previousPos = previousView.getCenter();
     return distanceBetweenPositions(futurePos, previousPos) > distanceBetweenPositions(target, previousPos);
}

double Zoomer::distanceBetweenPositions(sf::Vector2f first, sf::Vector2f second)
{
     return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
}

bool Zoomer::hasMoves()
{
     return not moveQueue.empty();
}

void Zoomer::cancelMoves()
{
     while (not moveQueue.empty()){
	  moveQueue.pop();
     }
}

void Zoomer::forceViewToTarget()
{
     view.setCenter(target);
     view.setSize(targetSize);
     view.setRotation(targetAngle);
}

void Zoomer::calculateViewMoves(unsigned int speed)
{
     //Calculate the translations.
     std::vector<float> yMoves = calculateCoordinateTranslations(target.y, view.getCenter().y, speed);
     std::vector<float> xMoves = calculateCoordinateTranslations(target.x, view.getCenter().x, speed);

     //Fill in coordinates so that they match up.
     while (yMoves.size() > xMoves.size()){
	  xMoves.push_back(0.);
     }
     while (xMoves.size() > yMoves.size()){
	  yMoves.push_back(0.);
     }

     //Calculate which way we should turn.
     float deltaTheta = calculateDeltaTheta();
     
     //Push moves onto the queue.
     for (size_t ii = 0; ii < yMoves.size(); ii++){
	  moveQueue.push(ViewMove(sf::Vector2f(xMoves[ii], yMoves[ii]),
				  calculateDTheta(yMoves.size(), ii, deltaTheta),
				  calculateNewSize(yMoves.size(), ii)));
     }
}

std::vector<float> Zoomer::calculateCoordinateTranslations(float finalPosition,
							   float initialPosition,
							   unsigned int speed)
{
     //Get the total distance we need to travel so we can do
     //calculations properly.
     int deltaY = std::abs(int(round(finalPosition - initialPosition)));
     
     //Calculate, on average, how many pixels we need to move per
     //frame. If we're smoothing the movement curve, then this will be
     //the maximum instantaneous velocity.
     int sign = std::abs(finalPosition - initialPosition)/(finalPosition - initialPosition);
     float dy = float(speed) / float(fps);
     unsigned int totalYMovesNeeded = 0;
     unsigned int smoothFrames = 0;
     
     //Find how many total movement vectors we need to produce.
     if (std::abs(dy) > 1){
	  dy = round(dy);
	  totalYMovesNeeded = std::abs(deltaY/dy);
	  smoothFrames =  (unsigned int)(percentSmooth * totalYMovesNeeded);
     }
     //Else we just move 1 pixel at a time, but over several frames.
     else{
	  dy = (deltaY/std::abs(deltaY));
	  totalYMovesNeeded = std::abs(deltaY);
	  smoothFrames = 0;
     }

     //Generate vector of y moves.
     //std::vector<float> yMoves(totalYMovesNeeded, dy);
     std::vector<float> yMoves;
     for (unsigned int ii = 0; ii < totalYMovesNeeded; ii++){
	  yMoves.push_back(dy * sign);
     }

     //Smooth out the first segment.
     for (unsigned int ii=0; ii < smoothFrames; ii++){
	  yMoves[ii] = round(float(ii)/float(smoothFrames) * dy) * sign;
	  //std::cout << yMoves[ii] << std::endl;
     }

     //Figure out how many pixels we have to make up for.
     float lostPixels = smoothFrames * dy - std::abs(sumFloatVectorRange(yMoves, 0, smoothFrames));

     //Determine how many extra frames we need to move in the
     //middle. Multiply by 2 because the other end will lose pixels too.
     unsigned int additionalMovesNeeded = int(round(lostPixels / dy * 2));

     //Add the extra frames of movement.
     for (unsigned int ii = 0; ii < additionalMovesNeeded; ii++){
	  yMoves.push_back(dy * sign);
     }

     //Update totalYMovesNeeded to reflect new frames
     totalYMovesNeeded += additionalMovesNeeded;

     //Finally, smooth out the last part of the curve.
     for (unsigned int ii = totalYMovesNeeded - smoothFrames; ii < totalYMovesNeeded;ii++){
	  //The left end of where we start smoothing.
	  unsigned int leftEnd = totalYMovesNeeded - smoothFrames;

	  //How far we are in the animation as a ratio of the distance
	  //from leftEnd over the number of frames we need to smooth.
	  float animationPercentage  = (float(ii) - leftEnd)/float(smoothFrames);

	  //Ramp down the magnitude of the velocity as we get closer.
	  yMoves[ii] = (dy - (round(animationPercentage * dy))) * sign;
	  //std::cout << yMoves[ii] << std::endl;
     }

     // sf::Vector2f displacementVector = sumVelocityVectors(yMoves);
     
     return yMoves;
}

float Zoomer::calculateDeltaTheta()
{
     float degreesToTurn = targetAngle - previousView.getRotation();
     degreesToTurn = std::abs(degreesToTurn) > 180 ? 360 + degreesToTurn : degreesToTurn;
     return degreesToTurn;
}

float Zoomer::calculateDTheta(size_t frames, size_t currFrame, float degreesToTurn)
{
     return degreesToTurn / frames;
}

float Zoomer::sumFloatVectorRange(std::vector<float> theVector, size_t start, size_t end)
{
     float result = 0;
     for (size_t ii = start; ii < end; ii++){
	  result += theVector[ii];
     }
     return result;
}

// sf::Vector2f sumVelocityVectors(std::vector<float> )

sf::Vector2f Zoomer::calculateNewSize(size_t frames, size_t currFrame)
{
     float xComponent = (targetSize.x - previousView.getSize().x) / frames * currFrame + previousView.getSize().x;
     float yComponent = (targetSize.y - previousView.getSize().y) / frames * currFrame + previousView.getSize().y;
     return sf::Vector2f(xComponent, yComponent);
}
