#ifndef ZOOMER_HPP
#define ZOOMER_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <queue>
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>

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

     /**
      * Clears the move queue.
      */
     void cancelMoves();

     /**
      * Forces the zoomer to move instantly to its current target.
      */
     void forceViewToTarget();
     
private:
     //Properties
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

      /**
      * Holds ViewMoves.
      */
     std::queue<ViewMove> moveQueue;

     //Methods

     /**
      * Calculates whether the next movement will overshoot the
      * target.
      * @return True if it will overshoot, False if not.
      */
     bool willOvershoot(sf::Vector2f futurePos);

     /**
      * Calculates the distance between two points in R2.
      * @return double The distance between the two points.
      */
     double distanceBetweenPositions(sf::Vector2f first, sf::Vector2f second);

     /**
      * Populate the moveQueue with ViewMoves.
      * @param speed The magnitude of the velocity the camera should
      * move at.
      */
     void calculateViewMoves(unsigned int speed);

     /**
      * Stretch out a list of moves.
      * @param moves std::vector<float> to stretch.
      * @param length Length new list should be.
      * @return A vector of floats of size length.
      */
     std::vector<float> stretchMoves(std::vector<float>& moves, size_t length);

     /**
      * Calculates the individual components of velocity when given
      * just the magnitude
      * @param speed The magnitude of the velocity the camera should move.
      */
     sf::Vector2f calculateVelocityComponents(unsigned int speed);
     
     /**
      * Helper function to calculate translations. This function does
      * most of the work in determining movement, and dictates how
      * many frames are spent on moving.
      * @param finalPosition The final position along one direction.
      * @param initialPosition The initial position along one
      * direction.
      * @param speed The speed of the velocity component in this
      * direction.
      * @return A vector of instantaneous velocity components to move.
      */
     std::vector<float> calculateCoordinateTranslations(float finalPosition, float initialPosition, unsigned int speed);
     
     /**
      * Helper function to sum a bunch of moves.
      * @param theVector The vector to sum.
      * @param start The starting point on the vector.
      * @param end The ending point on the vector.
      */
     float sumFloatVectorRange(std::vector<float> theVector, size_t start, size_t end);

     /**
      * Helper function to calculate total degrees to rotate, to
      * minimize rotation.
      */
     float calculateDeltaTheta();
     
     /**
      * Helper function to calculate how much to rotate each frame.
      * @param frames The total number of frames of movement.
      * @param currFrame The current frame in the move animation.
      * @param degreesToTurn The change in degree from start to end.
      * @return Instantaneous angular velocity.
      */
     float calculateDTheta(size_t frames, size_t currFrame, float degreesToTurn);
     
     /**
      * Helper function to calculate how much to resize each frame.
      * @param frames Total time we have to move.
      * @param currFrame Current point in time.
      * @return The new size after adding on instantaneous size increase.
      */
     sf::Vector2f calculateNewSize(size_t frames, size_t currFrame);
     
    
     
};


#endif //ZOOMER_HPP
