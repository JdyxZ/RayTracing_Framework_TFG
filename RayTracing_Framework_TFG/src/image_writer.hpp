#pragma once

// Headers
#include "core.hpp"

// Forward declarations
struct file_size;
struct Chrono;

enum IMAGE_FORMAT
{
    PNG,
    JPG
};

class ImageWriter
{
public:
    string name;					        // The name is generated with timestamp
    string fmt_name;                        // The image name plus the image extension
    IMAGE_FORMAT format = PNG;              // Image format
    string format_str;                      // Image format string
    string image_path;                      // Short path based on cwd where image will be saved
    int precision = 8;                      // Bit precision of the output image
    int quality = 100;                      // Only for JPG images
    int width = 400;                        // Rendered image width in pixel count
    int height;                             // Rendered image height
    double aspect_ratio = 16.0 / 9.0;       // Ratio of image width over height
    shared_ptr<file_size> size;             // Output image file size
    shared_ptr<Chrono> encoding_chrono;     // Chrono to measure the image encoding time using stb_image_write

    ImageWriter();

    void initialize();
    void write_pixel(int pixel_position, tuple<int, int, int> RGB_color);
    void save();
   
private:
    vector<unsigned char> data;             // Image buffer
    int channels = 3;                       // RGB channels

    bool savePNG();
    bool saveJPG(int quality);
};

 


