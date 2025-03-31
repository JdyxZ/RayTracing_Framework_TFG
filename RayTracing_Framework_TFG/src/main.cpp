// Headers
#include "core.hpp"
#include "logger.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "image_writer.hpp"
#include "log_writer.hpp"

int main()
{
    // Create render objects
    Scene scene;
    Camera camera;
    ImageWriter image;
    LogWriter log;

    // Scene start
    scene.start();

    // Build scene
    scene.build(camera, image);

    // Intialize image
    image.initialize();

    // Initialize the camera
    camera.initialize(scene, image);

    // Render scene
    camera.render(scene, image);

    // Encode and save image with desired format
    image.save();

    // Scene end
    scene.end();

    // Write scene log
    log.write(scene, camera, image);    
}
        