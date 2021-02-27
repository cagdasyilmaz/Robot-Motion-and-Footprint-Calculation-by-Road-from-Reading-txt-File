#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#ifndef FILE_MANIPULATION_H
#define FILE_MANIPULATION_H

// Function definition of the text file reading
/*
 * Input: stream object
 * Output: paired coordinate vectors which include path data
 */
std::vector< std::pair<float, float> > read_txt_file(std::ifstream &dataFileInput)
{
    //Declaration of the road coordinates vector which is the return variable of the function
    std::vector< std::pair<float, float> > roadCoordinates = {};

    //checking whether the file is open
    if (dataFileInput.is_open())
    {
        // define delimeters for parsing
        std::string delimiter_1 = "{";
        std::string delimiter_2 = "}";
        std::string delimiter_3 = ",";

        // size_t variables to identify the location of the delimeters
        size_t found_open_curly;
        size_t found_close_curly;

        // initiation of the boolean values whether is exists or not
        bool open_curly = false;
        bool close_curly = false;

        //First take the line as a coordinate string then parse it to meaningful parts
        std::string coordinates;

        // read .txt file line-by-line
        while( getline(dataFileInput, coordinates))
        {
            //read data from file object and put it into string.
            //cout << coordinates << "\n"; //print the data of the string

            // This do loop searches each lines and found the vectors located in that particular line
            do{

                // Find the place for "{" and "}", respectively
                found_open_curly = coordinates.find( delimiter_1 );
                found_close_curly = coordinates.find( delimiter_2 );

                // Check if the "{" is found or not
                if (found_open_curly != std::string::npos)
                {
                    //cout << "{ found at: " << found_open_curly << '\n';
                    open_curly = true;
                }
                else
                {
                    open_curly = false;
                }

                // Check if the "}" is found or not
                if (found_close_curly != std::string::npos)
                {
                    //cout << "} found at: " << found_close_curly << '\n';
                    close_curly = true;
                }
                else
                {
                    close_curly = false;
                }

                if( open_curly && close_curly )
                {
                    size_t pos = 0;

                    // Since string taken in between '{' and '}' parse the x and y pair
                    std::string pair_x_y_str = coordinates.substr(found_open_curly+1, (found_close_curly-found_open_curly)-1); // { , }
                    //cout << "2D coordinates: " << pair_x_y_str << endl;

                    if((pos = pair_x_y_str.find(delimiter_3)) != std::string::npos)
                    {
                        // Parse x,y coordinates and then push them (by converting float) into vector as Coord2D instance
                        std::string x_str = pair_x_y_str.substr(0, pos);
                        std::string y_str = pair_x_y_str.substr(pos+delimiter_3.length(), pair_x_y_str.length());

                        // write x and y coordinate values to road vector
                        roadCoordinates.emplace_back(std::stod(x_str), std::stod(y_str));
                    }

                    // Split the obtained road coordinate and check removal part
                    coordinates = coordinates.substr(found_close_curly+1,coordinates.size()-found_close_curly);
                }

            } while( open_curly && close_curly );
        }
        dataFileInput.close(); //close the file object.
    }

    return roadCoordinates;
}

#endif //FILE_MANIPULATION_H
