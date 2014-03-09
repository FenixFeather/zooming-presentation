#ifndef ZOOMER_HPP
#define ZOOMER_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>

class Zoomer{
public:
     /**
      * Construct a default Zoomer.
      */
     Zoomer();

     /**
      * Construct
      * @param center Center view should start on.
      * @param size Starting view size.
      * @param framerate Framerate in frames per second.
      */
     Zoomer(sf::Vector2f center, sf::Vector2f size, unsigned int framerate=60);

     /**
      * Initialize a zoomer.
      * @param center Center view should start on.
      * @param size Starting view size.
      * @param framerate Framerate in frames per second.
      * @param angle The starting angle.
      */
     void init(sf::Vector2f center, sf::Vector2f size, unsigned int framerate=60, float angle=0);
     
     /**
      * Set the view center manually.
      * @param center Coordinates view should center on.
      */
     void setCenter(sf::Vector2f center);

     /**
      * Set the view size manually.
      * @param size Size of the view.
      */
     void setSize(sf::Vector2f size);

     /**
      * Set the framerate.
      * @param framerate Framerate in frames per second.
      */
     void setFramerate(unsigned int framerate);
     
     /**
      * Get the current view.
      * @return Current sf::View
      */
     sf::View getCurrentView() const;
     
     /**
      * Set the target the viewer should move to.
      * Generate a queue of moves to execute frame by frame.
      * Clear the previous queue.
      * @param newTarget Target coordinates.
      * @param newSpeed Speed of translation in pixels per second.
      * @param percent The percentage of the translation animation
      * that should be smoothed.
      */
     void setAndCalculateTarget(sf::Vector2f newTarget,
				unsigned int newSpeed,
				float percent = 0.,
				float newAngle = 0.,
				sf::Vector2f newSize = sf::Vector2f(1366,768));

     /**
      * Get the next view in the zoomer's queue, presumably to update
      * the frame. Assume that there is stuff to pop.
      * @return An sf::View off the queue
      */
     sf::View popNextView();

     /**
      * Check if the move queue has stuff.
      * @return True if queue is not empty, false if it is.
      */
     bool hasMoves();
     
private:
     /**
      * Main view.
      */
     sf::View view;

     /**
      * Previous view.
      */
     sf::View previousView;
     
     /**
      * Position of target.
      */
     sf::Vector2f target;

     /**
      * Target angle.
      */
     float targetAngle;

     /**
      * Target size.
      */
     sf::Vector2f targetSize;

     /**
      * Default size. Calculated from the very first view.
      */
     sf::Vector2f defaultSize;
     
     /**
      * Holds framerate in FPS
      */
     unsigned int fps;

     /**
      * How much of the beginning and ending of the translation
      * animation should be smoothed, as a percentage.
      */
     float percentSmooth;
     
     /**
      * Defines a move for the view, consisting of a translation,
      * rotation, and zoom.
      */
     struct ViewMove{
	  
	  sf::Vector2f translationVector;
	  float rotationAngle;
	  sf::Vector2f zoomVector;

	  ViewMove(sf::Vector2f initialTranslationVector, float initialRotationAngle, sf::Vector2f initialZoomVector)
	       : translationVector(initialTranslationVector),
		 rotationAngle(initialRotationAngle),
		 zoomVector(initialZoomVector) {};
     };

     bool willOvershoot(sf::Vector2f futurePos);

     double distanceBetweenPositions(sf::Vector2f first, sf::Vector2f second);
     
     /**
      * Populate the moveQueue with ViewMoves.
      */
     void calculateViewMoves(unsigned int speed);
     
     /**
      * Helper function to calculate translations.
      */
     std::vector<float> calculateCoordinateTranslations(float finalPosition, float initialPosition, unsigned int speed);
     
     /**
      * Helper function to sum a bunch of moves.
      */
     float sumFloatVectorRange(std::vector<float> theVector, size_t start, size_t end);

     /**
      * Helper function to calculate total degrees to rotate, to
      * minimize rotation.
      */
     float calculateDeltaTheta();
     
     /**
      * Helper function to calculate how much to rotate each frame.
      */
     float calculateDTheta(size_t frames, size_t currFrame, float degreesToTurn);
     
     /**
      * Helper function to calculate how much to resize each frame.
      */
     sf::Vector2f calculateNewSize(size_t frames, size_t currFrame);
     
     /**
      * Holds ViewMoves.
      */
     std::queue<ViewMove> moveQueue;
     
};


#endif //ZOOMER_HPP
