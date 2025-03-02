#ifndef IMAGE_H
#define IMAGE_H

#include <fstream>
#include <filesystem>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "stb_image_write.h"

enum IMAGE_FORMAT
{
    PNG,
    JPG
};

class Image
{
public:

    string name;					    // The name is generated with timestamp
    std::vector<unsigned char> data;    // Image buffer
    int width;                          // Rendered image width in pixel count
    int height;                         // Rendered image height
    double aspect_ratio;                // Ratio of image width over height
    int channels = 3;                   // RGB channels

    Image(int width, int& height, double aspect_ratio)
    {
        name = define_name();

        height = int(width / aspect_ratio);
        height = (height < 1) ? 1 : height;

        data.resize(3 * width * height);

        this->width = width;
        this->height = height;
        this->aspect_ratio = aspect_ratio;
    }

    void write_pixel(int pixel_position, tuple<int, int, int> RGB_color)
    {
        int red_byte, green_byte, blue_byte;
        std::tie<int, int, int>(red_byte, green_byte, blue_byte) = RGB_color;

        data[pixel_position + 0] = red_byte;
        data[pixel_position + 1] = green_byte;
        data[pixel_position + 2] = blue_byte;
    }

    void savePNG()
    {
        auto PNG_name = name + ".png";
        int success = stbi_write_png(PNG_name.c_str(), width, height, channels, data.data(), width * channels);

        if (success)
            std::cout << "Image successfully written: " << PNG_name << std::endl;
        else
            std::cerr << "Error: Failed to write image: " << PNG_name << std::endl;
    }

    void saveJPG(int quality)
    {
        auto JPG_name = name + ".jpg";
        int success = stbi_write_jpg(JPG_name.c_str(), width, height, channels, data.data(), quality);

        if (success)
            std::cout << "Image successfully written: " << JPG_name << std::endl;
        else
            std::cerr << "Error: Failed to write image: " << JPG_name << std::endl;
    }

private:

    string define_name()
    {
        // Get the current local date and time
        std::time_t now = std::time(nullptr);
        std::tm localTime;

        // Use localtime_s for safer local time conversion
        if (localtime_s(&localTime, &now) != 0)
        {
            std::cerr << "Error: Could not retrieve local time!" << std::endl;
            return string("error_in_get_file_func"); // Return a distinct string
        }

        // Format the date and time as "YYYY-MM-DD_HH-MM-SS"
        char dateTimeStr[25];
        std::strftime(dateTimeStr, sizeof(dateTimeStr), "%Y-%m-%d_%H-%M-%S", &localTime);
        string dateTime = string(dateTimeStr);

        // Construct the file name and path
        string folderPath = "images/";
        string fileName = folderPath + dateTime;

        // Create directory for images in case it does not already exist
        std::filesystem::create_directories(folderPath);

        return fileName;
    }
};

#endif IMAGE_H
