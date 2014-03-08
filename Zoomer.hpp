#ifndef ZOOMER_HPP
#define ZOOMER_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include <math.h>
#include <cmath>
#include <vector>

class Zoomer{
public:
     /**
      * Construct
      * @param center Center view should start on.
      * @param size Starting view size.
      */
     Zoomer(sf::Vector2f center, sf::Vector2f size, unsigned int framerate=60);

     /**
      * Set the view center manually.
      * @param center Coordinates view should center on.
      */
     void setCenter(sf::Vector2f center);

     /**
      * Get the current view.
      * @return Current sf::View
      */
     sf::View getCurrentView() const;
     
     /**
      * Set the target the viewer should move to.
      * Generate a queue of moves to execute frame by frame.
      * @param newTarget Target coordinates.
      * @param newSpeed Speed of translation in pixels per second.
      */
     void setAndCalculateTarget(sf::Vector2f newTarget, unsigned int newSpeed);

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
      * Current position of target.
      */
     sf::Vector2f target;

     /**
      * Holds framerate in FPS
      */
     unsigned int fps;
     
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

     /**
      * Populate the moveQueue with ViewMoves.
      */
     void calculateViewMoves(unsigned int speed);
     
     /**
      * Helper function to calculate translations.
      */
     std::vector<float> calculateCoordinateTranslations(float finalPosition, float initialPosition);
     
     /**
      * Helper function to sum a bunch of moves.
      */
     float sumFloatVectorRange(std::vector<float> theVector, size_t start, size_t end);
     
     /**
      * Holds ViewMoves.
      */
     std::queue<ViewMove> moveQueue;
     
};


#endif //ZOOMER_HPP
