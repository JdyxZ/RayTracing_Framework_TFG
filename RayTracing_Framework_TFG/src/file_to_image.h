#ifndef FILE_TO_IMAGE_H
#define FILE_TO_IMAGE_H

#include <fstream>
#include <filesystem>
#include <iostream>

std::ofstream get_file()
{
    // Get the current local date and time
    std::time_t now = std::time(nullptr);
    std::tm localTime;

    // Use localtime_s for safer local time conversion
    if (localtime_s(&localTime, &now) != 0)
    {
        std::cerr << "Error: Could not retrieve local time!" << std::endl;
        return std::ofstream(); // Return an empty ofstream on error
    }

    // Format the date and time as "YYYY-MM-DD_HH-MM-SS"
    char dateTimeStr[25];
    std::strftime(dateTimeStr, sizeof(dateTimeStr), "%Y-%m-%d_%H-%M-%S", &localTime);
    std::string dateTime = std::string(dateTimeStr);

    // Construct the file name and path
    std::string folderPath = "images/";
    std::string fileName = folderPath + dateTime + ".ppm";

    // Create directory for images in case it does not already exist
    std::filesystem::create_directories(folderPath);

    // Create the file
    std::ofstream file(fileName);

    // Check if the file was opened successfully and return file ready to write in
    if (file.is_open())
    {
        std::cout << "File opened successfully to " << fileName << std::endl;
    }
    else
    {
        std::cerr << "Error: Could not open the file!" << std::endl;
    }

    return file;
}


#endif FILE_TO_IMAGE_H
