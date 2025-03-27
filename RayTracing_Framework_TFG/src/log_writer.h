#ifndef LOG_WRITER_H
#define LOG_WRITER_H

struct LogWriter
{
    LogWriter()
    {
        // Define name based on timestamp
        name = get_current_timestamp();

        // Create directory for logs in case it does not already exist
        string folderPath = logs_directory + "\\";
        fs::create_directories(folderPath);
    }

    void write(const Scene& scene, const Camera& camera, const ImageWriter& image)
    {
        // Log info
        Logger::info("LogWriter", "Writing log file started. Saving rendering statistics and debug information to " + name + ".md");

        // Get System and Project info instance
        auto sp_info = SystemProjectInfo::instance();

        // Create log
        std::ostringstream log;

        log << "# Render Log 📜\n\n";

        // System Section
        log << "## System 🖥️\n";
        log << "**Platform:** " << sp_info.platform << "  \n";
        log << "**CPU Threads Used:** " << sp_info.cpu_threads << "  \n\n";

        // Project Section
        log << "## Project 🏯\n";
        log << "**Raytracer Version:** " << sp_info.version << "  \n";
        log << "**Build Configuration:** " << sp_info.build_configuration << "  \n";
        log << "**Compiler:** " << sp_info.compiler << "  \n";
        log << "**Start Time:** " << scene._start << "  \n";
        log << "**End Time:** " << scene._end << "  \n";
        log << "**Elapsed time:** " << scene.full_pipeline.elapsed_to_string() << "\n\n";

        // Camera Section
        log << "## Camera 📷\n";
        log << "**Type:** Perspective  \n";
        log << "**Position:** " << camera.lookfrom << " \n";
        log << "**LookAt:** " << camera.lookat << " \n";
        log << "**Up Vector:** " << camera.world_up << " \n";
        log << "**FOV:** " << camera.vertical_fov << "  \n";
        log << "**Defocus Angle:** " << camera.defocus_angle << "  \n";
        log << "**Focus Distance:** " << camera.focus_distance << "  \n\n";

        // Scene Section
        log << "## Scene 🌆\n";
        log << "**Name:** " << scene.name << "  \n";
        log << "**Background Color:** " << scene.background << " \n";
        log << "**Samples per Pixel:** " << scene.samples_per_pixel << "  \n";
        log << "**Max Ray Bounces:** " << scene.bounce_max_depth << "  \n";
        log << "**Build Time:** " << scene.build_chrono.elapsed_to_string() << "\n\n";

        // BVH Section
        log << "## BVH 🍂\n";
        log << "**Main Depth:** " << scene.bvh_depth << "  \n";
        log << "**Main Nodes:** " << scene.bvh_nodes << "  \n";
        log << "**BVHs Build Time:** " << scene.bvh_chrono.elapsed_to_string() << "\n\n";

        // Primitives
        log << "## Primitives 🔵\n";
        log << "**Spheres:** " << scene.spheres << "  \n";
        log << "**Quads:** " << scene.quads << "  \n";
        log << "**Triangles:** " << scene.triangles << "  \n";
        log << "**Total:** " << scene.primitives << "  \n\n";

        // Meshes
        log << "## Meshes 🔺\n";
        for (auto mesh : scene.meshes)
        {
            log << "· `" << mesh->name() << "`:\n";
            log << "    - **Total Triangles:** " << mesh->num_triangles() << "  \n";
            log << "    - **Surfaces:** " << mesh->num_surfaces() << "  \n";
            log << "    - **Textures:** " << to_list(mesh->texture_names()) << "  \n";
            log << "    - **BVH build time:** " << mesh->bvh_chrono().elapsed_to_string() << " \n";
        }
        log << "\n";

        // Image Section
        log << "## Image 🖼️\n";
        log << "**Name:** `" << image.fmt_name << "`  \n";
        log << "**Resolution:** " << image.width << "x" << image.height << "  \n";
        log << "**Color precision:** " << image.precision << " bits  \n";
        log << "**File Size:** " << image.size.amount << " " << image.size.unit << "  \n";
        log << "**Encoding Time:** " << image.encoding_chrono.elapsed_to_string() << " \n\n";

        // Render Benchmark
        log << "## Render Benchmark 🎇\n";
        log << "**Rendering Time:** " << camera.render_chrono.elapsed_to_string() << " \n";
        log << "**Rays:**\n";
        log << "    - **Primary Rays:** " << camera.primary_rays << "  \n";
        log << "    - **Reflected Rays:** " << camera.reflected_rays << "  \n";
        log << "    - **Refracted Rays:** " << camera.refracted_rays << "  \n";
        log << "    - **Total Rays Casted:** " << camera.rays_casted << "  \n";
        log << "    - **Average Rays per Second:** " << camera.average_rays_per_second << "  \n\n";

        // Log messages
        log << "## Log 📋\n";
        for (auto message : Logger::messages())
            log << message;

        // Construct the log file
        std::string log_filename = logs_path + name + ".md";
        std::ofstream log_file(log_filename);

        if (!log_file)
            Logger::error("LogWriter", "Could not open log file for writing: " + name + ".md");

        // Write log content to file
        log_file << log.str();
        log_file.close();
        Logger::info("LogWriter", "Log file saved: " + name + ".md");

        // Clear log messages in case of multiple scene render
        Logger::clear();
    }

private:
    string name;
};

#endif