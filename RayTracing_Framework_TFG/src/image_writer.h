#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

enum IMAGE_FORMAT
{
    PNG,
    JPG
};

class ImageWriter
{
public:

    string name;					    // The name is generated with timestamp
    string fmt_name;                    // The image name plus the image extension
    IMAGE_FORMAT format = PNG;          // Image format
    string format_str;                  // Image format string
    string image_path;                  // Short path based on cwd where image will be saved
    int precision = 8;                  // Bit precision of the output image
    int quality = 100;                  // Only for JPG images
    int width = 400;                    // Rendered image width in pixel count
    int height;                         // Rendered image height
    double aspect_ratio = 16.0 / 9.0;   // Ratio of image width over height
    file_size size;                     // Output image file size
    Chrono encoding_chrono = Chrono();  // Chrono to measure the image encoding time using stb_image_write

    void initialize()
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

    void write_pixel(int pixel_position, tuple<int, int, int> RGB_color)
    {
        int red_byte, green_byte, blue_byte;
        std::tie<int, int, int>(red_byte, green_byte, blue_byte) = RGB_color;

        data[pixel_position + 0] = red_byte;
        data[pixel_position + 1] = green_byte;
        data[pixel_position + 2] = blue_byte;
    }

    void save()
    {
        Logger::info("ImageWriter", "Encoding to image started.");

        bool success = false;

        encoding_chrono.start();

        switch (format)
        {
        case PNG:
            success = savePNG();
            break;
        case JPG:
            success = saveJPG(quality);
            break;
        }

        encoding_chrono.end();

        if (success)
            Logger::info("ImageWriter", "Image successfully written: " + fmt_name);
        else
            Logger::info("ImageWriter", "Failed to write image: " + fmt_name);
      
        size = get_file_size(image_path);
    }

private:

    vector<unsigned char> data;         // Image buffer
    int channels = 3;                   // RGB channels

    bool savePNG()
    {
        int success = stbi_write_png(image_path.c_str(), width, height, channels, data.data(), width * channels);
        return success;
    }

    bool saveJPG(int quality)
    {
        int success = stbi_write_jpg(image_path.c_str(), width, height, channels, data.data(), quality);
        return success;
    }
};

#endif 


