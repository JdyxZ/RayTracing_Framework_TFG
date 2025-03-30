// Internal Headers
#include "core.hpp"
#include "image_writer.hpp"
#include "utilities.hpp"
#include "chrono.hpp"

// Macros
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__

// External Headers
#include "../external/stb_image_write.hpp"

ImageWriter::ImageWriter()
{
    encoding_chrono = make_shared<Chrono>();
}

void ImageWriter::initialize()
{
    // Get current date and time
    name = get_current_timestamp();

    // Set image extension
    switch (format)
    {
    case PNG:
        format_str = ".png";
        break;
    case JPG:
        format_str = ".jpg";
        break;
    default:
        format_str = ".error";
        break;
    }

    // Set formatted image name 
    fmt_name = name + format_str;

    // Set path for image saving
    image_path = output_path + "\\" + name + format_str;

    // Construct the file name and path
    string folderPath = output_directory + "\\";

    // Create directory for images in case it does not already exist
    fs::create_directories(folderPath);

    // Calculate height given the width
    height = int(width / aspect_ratio);
    height = (height < 1) ? 1 : height;

    // Reserve space for image data pointer
    data.resize(3 * width * height);

    // Log info
    Logger::info("ImageWriter", "Image frame succesfully initialized");
}

void ImageWriter::write_pixel(int pixel_position, tuple<int, int, int> RGB_color)
{
    int red_byte, green_byte, blue_byte;
    std::tie<int, int, int>(red_byte, green_byte, blue_byte) = RGB_color;

    data[pixel_position + 0] = red_byte;
    data[pixel_position + 1] = green_byte;
    data[pixel_position + 2] = blue_byte;
}

void ImageWriter::save()
{
    Logger::info("ImageWriter", "Encoding to image started.");

    bool success = false;

    encoding_chrono->start();

    switch (format)
    {
    case PNG:
        success = savePNG();
        break;
    case JPG:
        success = saveJPG(quality);
        break;
    }

    encoding_chrono->end();

    if (success)
        Logger::info("ImageWriter", "Image successfully written: " + fmt_name);
    else
        Logger::info("ImageWriter", "Failed to write image: " + fmt_name);

    size = make_shared<file_size>(get_file_size(image_path));
}

bool ImageWriter::savePNG()
{
    int success = stbi_write_png(image_path.c_str(), width, height, channels, data.data(), width * channels);
    return success;
}

bool ImageWriter::saveJPG(int quality)
{
    int success = stbi_write_jpg(image_path.c_str(), width, height, channels, data.data(), quality);
    return success;
}
