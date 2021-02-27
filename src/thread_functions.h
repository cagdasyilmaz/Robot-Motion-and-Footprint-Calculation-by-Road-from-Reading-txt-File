#ifndef CPP_THREAD_FUNCTIONS_H
#define CPP_THREAD_FUNCTIONS_H

#include <thread>
#include <condition_variable>

#include "locked_stream.h"
#include "robot_motion.h"
#include "robot_footprint.h"
#include "log_watch.h"

// Global declation for mutex, condition_variable, and boolean variables
std::mutex m;
std::condition_variable cv;
bool ready = false;
bool processed = false;

/*
 * This thread function locks the output stream operation and takes values while it is printing
 * to console. This function calls LockedStream class member function which based on mutex principles
 * The necessity of this function come up at the beginning of the thread call.
 */
LockedStream locked_cout()
{
    // Return streaming operation to console without race condition.
    return LockedStream(std::cout);
}


/*
 * In this thread motion of the robot is performed. During the this operation thread locks the motion.setMotion()
 * member function with an condition variable so area miscalculation in the current and accumulated footprint area
 * can be prevented. This thread and the thread below will be running concurrently
 */
void motionThreadFn(std::vector<std::pair<float, float>> &roadCoordinates,
                    std::vector<std::pair<float, float>>::iterator &roadVectorIterator,
                    RobotMotion &motion, int sleepTime)
{
    //std::cout << "***Motion Threat***" << std::endl;

    // Write initial values of the robot's state
    /*
     * log_watch is the class which gives the current time with millisecond
     * locked_cout is the modified std::cout for multi-threading
     */

    log_watch<std::chrono::milliseconds> setMotionTime("%X.");
    locked_cout() << setMotionTime << ": [X Position: " << motion.getCurrentX() << " (m)] | [Y Position: " <<
                  motion.getCurrentY() <<" (m)] | [Orientation: " << motion.getOrientation() << "(rad)]" << std::endl;

    while( roadVectorIterator != roadCoordinates.end())
    {
        /*
         * In this scope this process is locked and this thread will wait 2 second for the other thread
         * to finish its job. This time can be adjusted as configurable, condition variable waits 2 seconds
         * for other thread and continue its job.
         */
        {
            //std::lock_guard<std::mutex> lk(m);

            std::unique_lock<std::mutex> lk(m);

            cv.wait_for(lk, std::chrono::milliseconds(2));

            // move the robot to next road coordinates
            motion.setMotion();

            // Write the robot states with sensitive time stamps
            log_watch<std::chrono::milliseconds> setMotionTime("%X.");
            locked_cout() << setMotionTime << ": [X Position: " << motion.getNextX() << " (m)] | [Y Position: " <<
                          motion.getNextY() << " (m)] | [Orientation: " << motion.getOrientation() << "(rad)]"
                          << std::endl;

            ready = true;
        }
        // notification that gives information to other thread, so meaningful footprint calculation can be done
        cv.notify_one();

        // sleep for certain amount time. It can be considered as sampling time of the robot motion
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

        // Check whether the iterator reached or not at the end of the vector
        if( roadVectorIterator == roadCoordinates.end() )
        {
            motion.setMotionFinished(true);
        }
    }
}

/*
 * Footprint area calculation is done in this thread. With the help of condition variable this thread is provided to
 * stay in while up until the notification is sent to it. So the calculation can be done. If the calculation is not done
 * in specific time, then above thread will wait certain amount of time. This can be utilized by using member function
 * of the wait_for() function.
 */
void FootprintThreadFn(RobotMotion &motion,
                       RobotFootprint &calculateFootprint,
                       int sleepTime)
{
    //std::cout << "***Accumulated Footprint Thread***" << std::endl;

    // Write initial values of the robot's footprint
    /*
     * log_watch is the class which gives the current time with millisecond
     * locked_cout is the modified std::cout for multi-threading
     */
    log_watch<std::chrono::milliseconds> setFootPrintTime("%X.");
    locked_cout()  << setFootPrintTime << ": [Current Covered Area: " << calculateFootprint.getCurrentFootprint()
                   << " m^2] | [Accumulated Total Area: " << calculateFootprint.getAccumulatedFootprint()
                   << " m^2]" << std::endl;

    while( !(motion.getMotionFinished()))
    {
        // Wait until other thread sends data
        std::unique_lock<std::mutex> lk(m);

        // this wait state waits this thread until ready is true
        cv.wait(lk, []{return ready;});

        // calculate the robot to current and accumulated footprint area
        calculateFootprint.setAccumulatedFootprint( motion.getNextX(), motion.getNextY());

        // Write out the robot's footprints with sensitive time stamps
        log_watch<std::chrono::milliseconds> setFootPrintTime("%X.");
        std::cout << setFootPrintTime << ": [Current Covered Area: " << calculateFootprint.getCurrentFootprint()
                  << " m^2] | [Accumulated Total Area: " << calculateFootprint.getAccumulatedFootprint()
                  << " m^2]" << std::endl;

        // change the boolean variables for conditional_variable
        processed = true;
        ready = false;


        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lk.unlock();
        cv.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime) );
    }
}

#endif //CPP_THREAD_FUNCTIONS_H
