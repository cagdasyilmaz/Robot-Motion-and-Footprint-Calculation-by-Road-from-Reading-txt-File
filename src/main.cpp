#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>

// user defined headers
#include "file_manipulation.h"
#include "robot_motion.h"
#include "robot_footprint.h"
#include "locked_stream.h"
#include "thread_functions.h"


// Global string definition
std::mutex LockedStream::s_out_mutex{};

int main() {

    // Create an input filestream
    std::ifstream dataFileInput;

    // Create an input filestream object for data set
    dataFileInput.open( "mLc80Pen.txt", std::ios::in );

    // Make sure the file is open
    if(!dataFileInput.is_open())
        throw std::runtime_error("Could not open file");

    std::vector< std::pair<float, float> > roadCoordinates = read_txt_file(dataFileInput);
    //for( auto coord :  roadCoordinates )
    //    cout << "[x, y]: [" << item.coord << ", " << item.coord << "]" << endl;

    // Declaration of the robot specific variables
    float currentX = 0.0f;
    float currentY = 0.0f;
    float currentOrientation = 0.0f;

    float lenght, width = {};
    // Sleep time for threads
    /*
     * @motionSleeptime takes values in millisecond range
     * @footprintSleeptime takes values in millisecond range
     */
    int motionSleeptime, footprintSleeptime = {};

    std::cout << "-------------------------------------------------\n"
              << "Please enter the robots lenght and width in meter\n"
              << "Lenght (m): ";
    std::cin >> lenght;
    std::cout << "Width  (m): ";
    std::cin >> width;
    std::cout << "*************************************************\n"
              << "Please enter the sleep times for threads\n"
              << "Sleep time for motion thread (ms): ";
    std::cin >> motionSleeptime;
    std::cout << "Sleep time for footprint thread (ms): ";
    std::cin >> footprintSleeptime;
    std::cout << "-------------------------------------------------" << std::endl;

    auto roadVectorIterator = roadCoordinates.begin();

    // Declaration a object from RobotMotion Class
    RobotMotion motion(roadVectorIterator, currentX, currentY);

    // Declaration a object from RobotFootprint Class
    RobotFootprint calculateFootprint(currentX, currentY, lenght, width);

    /*
     * Thread Declarations
     * thrMotion: in this thread, motion action is provided.
     * thrFootprint: in this thread footprint calculation will be done.
     */
    std::thread thrMotion{std::bind(motionThreadFn, std::ref(roadCoordinates),
            std::ref(roadVectorIterator),  std::ref(motion), motionSleeptime)};

    std::thread thrFootprint(FootprintThreadFn, std::ref(motion), std::ref(calculateFootprint), footprintSleeptime);

    thrMotion.join();
    thrFootprint.join();

    return 0;
}
