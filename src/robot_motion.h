#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"
//
// Created by cagdas on 21.02.2021.
//

#ifndef CPP_ROBOT_MOVE
#define CPP_ROBOT_MOVE

#include <vector>
#include <cmath>
class RobotMotion
{
public:

    /*
     * Constructor of the RobotMotion Class
     * Inputs: Iterator of the road vector
     *         Initial X position
     *         Initial Y position
     *         Initial orientation
     */
    explicit RobotMotion(std::vector<std::pair<float, float>>::iterator &iter, double currentX = 0.0f,
                         double currentY = 0.0f, double orientation = 0.0f )
            : roadIter{iter}, current_x(currentX), current_y(currentY), next_orientation(orientation)
    {
        // initially set the next x position current x position
        setNextX(current_x);

        // initially set the next y position current y position
        setNextY(current_y);

        setMotionFinished(false);
    }

    /*
     * Default Destructor of the RobotMotion Class
     * In this class no action is performed. Class objects destroyed at the end of program
     */
    ~RobotMotion()
    {
        // Empty destructor of the RobotMotion class
    }

    // set current x value of the robot at the center of mass point
    void setCurrentX(float &currentX)
    {
        current_x = currentX;
    }

    // set current y value of the robot at the center of mass point
    void setCurrentY(float &currentY)
    {
        current_y = currentY;
    }

    // set next x value of the robot at the center of mass point
    void setNextX(float &nextX)
    {
        next_x = nextX;
    }

    // set next y value of the robot at the center of mass point
    void setNextY(float &nextY)
    {
        next_y = nextY;
    }

    /*
     * Set motion
     * In this member function new position obtained
     * Orientation of the robot will be changed
     */
    void setMotion()
    {
        // Before increasing operator assign current x and y position of the robot
        setCurrentX(next_x);
        setCurrentY(next_y);

        // Take first element of the road iterator as an next x position of the robot
        setNextX(roadIter->first);

        // Take second element of the road iterator as an next x position of the robot
        setNextY(roadIter->second);

        // Calculate the orientation value for logging purpose
        next_orientation = atan2( getNextY()- getCurrentY(), getNextX()- getCurrentX());

        ++roadIter;
    }

    void setMotionFinished(bool finished)
    {
        motionFinished = finished;
    }

    // get the current x value
    float getCurrentX() const
    {
        return current_x;
    }

    // get the current y value
    float getCurrentY() const
    {
        return current_y;
    }

    // get the next x value
    float getNextX() const
    {
        return next_x;
    }

    // get the next y value
    float getNextY() const
    {
        return next_y;
    }

    bool getMotionFinished() const
    {
        return motionFinished;
    }

    // get the next orientation value
    float getOrientation() const
    {
        return next_orientation;
    }

private:
    std::vector<std::pair<float, float>>::iterator& roadIter;
    float current_x;        // x position of robot
    float current_y;        // y position of robot
    float next_x;           // next x position of robot
    float next_y;           // next y position of robot
    float next_orientation; // yaw_angle
    bool motionFinished;    // check whether motion finished or not
};

#endif //CPP_ROBOT_MOVE_H
