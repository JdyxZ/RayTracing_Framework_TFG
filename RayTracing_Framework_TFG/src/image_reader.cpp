// Internal Headers
#include "core.hpp"
#include "image_reader.hpp"

// Macros
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#define __STDC_LIB_EXT1__

// External Headers
#include "../external/stb_image.hpp"

ImageReader::ImageReader(const char* image_filename)
{
    auto filename = string(image_filename);

    // Hunt for the image file in some likely locations.
    string prefix = "";
    for (int i = 0; i < 7; ++i)
    {
        if (load(prefix + textures_directory + "\\" + filename)) return;
        prefix += "..\\";
    }
    if (load(filename)) return;

    Logger::error("ImageReader", "Could not load image file: " + string(image_filename));
}

ImageReader::ImageReader() {}

ImageReader::~ImageReader()
{
    delete[] bdata;
    STBI_FREE(fdata);
}

int ImageReader::width()  const 
{ 
    return (fdata == nullptr) ? 0 : image_width; 
}

int ImageReader::height() const 
{ 
    return (fdata == nullptr) ? 0 : image_height; 
}

bool ImageReader::load(const std::string& filename)
{
    // Loads the linear (gamma=1) image data from the given file name. Returns true if the
    // load succeeded. The resulting data buffer points to the three RGB [0.0, 1.0]
    // floating-point values of the first pixel (red, then green, then blue). Pixels are
    // contiguous, going left to right for the width of the image, followed by the next row
    // below, for the full height of the image.

    auto n = bytes_per_pixel; // Dummy out parameter: original components per pixel
    fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);

    if (fdata == nullptr) return false;

    bytes_per_scanline = image_width * bytes_per_pixel;
    convert_to_bytes();

    return true;
}

const unsigned char* ImageReader::pixel_data(int x, int y) const
{
    // Return the address of the three RGB bytes of the pixel at x,y. If there is no image
    // data, returns magenta.
    static unsigned char magenta[] = { 255, 0, 255 };
    if (bdata == nullptr) return magenta;

    x = clamp(x, 0, image_width);
    y = clamp(y, 0, image_height);

    return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}

int ImageReader::clamp(int x, int low, int high)
{
    // Return the value clamped to the range [low, high).
    if (x < low) return low;
    if (x < high) return x;
    return high - 1;
}

unsigned char ImageReader::float_to_byte(float value)
{
    if (value <= 0.0)
        return 0;
    if (1.0 <= value)
        return 255;
    return static_cast<unsigned char>(256.0 * value);
}

void ImageReader::convert_to_bytes()
{
    // Convert the linear floating point pixel data to bytes, storing the resulting byte
    // data in the `bdata` member.
    int total_bytes = image_width * image_height * bytes_per_pixel;
    bdata = new unsigned char[total_bytes];

    // Iterate through all pixel components, converting from [0.0, 1.0] float values to
    // unsigned [0, 255] byte values.
    auto* bptr = bdata;
    auto* fptr = fdata;
    for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
        *bptr = float_to_byte(*fptr);
}

