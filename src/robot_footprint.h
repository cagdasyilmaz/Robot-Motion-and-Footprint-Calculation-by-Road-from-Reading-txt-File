//
// Created by cagdas on 22.02.2021.
//

#ifndef CPP_ROBOT_FOOTPRINT_H
#define CPP_ROBOT_FOOTPRINT_H

#include <cmath>

class RobotFootprint
{
public:
    /*
     * Explicit constructor initially takes default values
     * Inputs: Previous X position
     *         Previous Y position
     *         Length of the robot
     *         Width of the robot
     */
    RobotFootprint(float prevXPos, float prevYPos, float length, float width)
            : previousXposition(prevXPos), previousYposition(prevYPos), robotLength(length), robotWidth(width)
    {
        // Initial area covered by robot
        currentFootPrint = robotLength * robotWidth;

        // Initial accumulated footprint area -> set function will also check it
        accumulatedFootprint = currentFootPrint;
        isPositionChanged = false;
    }

    /*
     * Default Destructor of the RobotFootprint Class
     * In this class no action is performed. Class objects destroyed at the end of program
     */
    ~RobotFootprint() {}

    // Set previous X position
    void setPreviousXPosition(float &prevXPos)
    {
        previousXposition = prevXPos;
    }

    // Set previous Y position
    void setPreviousYPosition(float &prevYPos)
    {
        previousYposition = prevYPos;
    }

    // Set current Footprint
    void setCurrentFootprint(float &currentArea)
    {
        currentFootPrint = currentArea;
    }

    // Set Accumulated Footprint
    void setAccumulatedFootprint(float curXpos, float curYpos)
    {
        // check robot in the same position or not
        if( previousXposition == curXpos && previousYposition == curYpos )
        {
            // position has not changed!!!
            isPositionChanged = false;
        }
        else
        {
            // position has changed!!!
            isPositionChanged = true;

            // calculate the accumulated footprint
            // first find the linear replacement of the middle point of the robot
            float linear_line = sqrt( pow( curXpos - getPreviousXPosition(), 2 ) + pow( curYpos - getPreviousYPosition(), 2 ) );

            // Multiply this linear line with the width of the robot and then add the accumulated footprint of the robot

            currentFootPrint = linear_line * robotWidth;

            accumulatedFootprint = accumulatedFootprint + currentFootPrint;
        }

        // assign current position information to previous position
        setPreviousXPosition( curXpos );
        setPreviousYPosition( curYpos );
    }

    // Get previous X position
    float getPreviousXPosition() const
    {
        return previousXposition;
    }

    // Get previous Y position
    float getPreviousYPosition() const
    {
        return previousYposition;
    }

    // Get current footprint
    float getCurrentFootprint() const
    {
        return currentFootPrint;
    }

    // Get accumulated footprint
    float getAccumulatedFootprint() const
    {
        return accumulatedFootprint;
    }

    // For notification purposes, take the position information whether it changed or not
    bool getIsPositionChange() const
    {
        return isPositionChanged;
    }

private:
    float previousXposition;    // previous x position of the center of body
    float previousYposition;    // previous y position of the center of body

    float robotLength;          // length of the robot
    float robotWidth;           // width of the robot

    float currentFootPrint;     // area covered between two points
    float accumulatedFootprint; // complete covered area taken from initial state to current state
    bool isPositionChanged;     // check robot is in new position or not
};

#endif //CPP_ROBOT_FOOTPRINT_H
