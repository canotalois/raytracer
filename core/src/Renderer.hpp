/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Color.hpp"
#include "Ray.hpp"
#include "INode.hpp"
#include "ILight.hpp"
#include "PhongReflectionModel.hpp"
#include "PointLight.hpp"
#include "AmbientLight.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "SafeCounter.hpp"
#include <fstream>
#include "../../plugins_src/GUI/src/SFMLGuiPlugin.hpp"
#include <condition_variable>
#include "SafeBuffer.hpp"
#include "TaskQueue.hpp"

#include <thread>

namespace RayTracer {
    class Renderer {
    public:
        explicit Renderer(
                const Camera& camera,
                const Color& ambientColor = Color(0.1, 0.1, 0.1),
                const Color& diffuseColor = Color(0.0, 0.0, 0.0),
                const Color& specularColor = Color(0.0, 0.0, 0.0),
                const Color& backgroundColor = Color(0.5, 0.7, 1.0),
                const PhongReflectionModel& phongModel = PhongReflectionModel(),
                unsigned int samplesPerPixel = 10,
                unsigned int maxDepth = 10,
                SFMLGuiPlugin* guiPlugin = nullptr
        ) : camera(const_cast<Camera &>(camera)), ambientColor(ambientColor), diffuseColor(diffuseColor),
            specularColor(specularColor), backgroundColor(backgroundColor),
            phongModel(phongModel), samplesPerPixel(samplesPerPixel), maxDepth(maxDepth),
            gui(guiPlugin), renderingComplete(false),
            buffer(camera.imageWidth, camera.imageHeight),
            imageWidth(camera.imageWidth), imageHeight(camera.imageHeight),
            interruptRendering(false) {}

        [[nodiscard]] RayTracer::Color rayColor(
                const Ray &r,
                const std::shared_ptr<RayTracer::INode> &node,
                const std::vector<std::unique_ptr<RayTracer::ILight>> &lights,
                unsigned int depth
        ) const {
            if (depth == 0) {
                return {0, 0, 0};
            }

            RayTracer::Intersection rec;
            if (node->hit(r, RayTracer::Interval(0.001), rec)) {
                RayTracer::Ray scattered;
                RayTracer::Color attenuation;
                RayTracer::Color recursiveColor = {0, 0, 0};

                if (rec.material->scatter(r, rec, attenuation, scattered)) {
                    recursiveColor = attenuation * rayColor(scattered, node, lights, depth - 1);
                } else {
                    recursiveColor = attenuation;
                }

                RayTracer::Color ambient = ambientColor;
                RayTracer::Color diffuse = diffuseColor;
                RayTracer::Color specular = specularColor;

                for (const auto& light : lights) {
                    Math::Vector<3> light_dir = light->getLightDirection(rec.intersection);
                    double distance = (light_dir).length();
                    light_dir = light_dir.normalize();
                    double light_attenuation = light->getAttenuation(distance, phongModel);

                    if (dynamic_cast<RayTracer::AmbientLight *>(light.get())) {
                        ambient += light->getLightColor() * phongModel.ambientCoefficient;
                    } else {
                        double diff = std::max(rec.normal.dot(light_dir), 0.0);
                        diffuse += attenuation * diff * light_attenuation * phongModel.diffuseCoefficient;

                        Math::Vector<3> view_dir = Math::Vector<3>(-r.direction.normalize());
                        Math::Vector<3> reflect_dir = Math::Vector<3>::reflect(Math::Vector<3>(-light_dir), rec.normal);
                        double spec = pow(std::max(view_dir.dot(reflect_dir), 0.0), phongModel.shininess);
                        specular += light->getLightColor() * spec * light_attenuation * phongModel.specularCoefficient;

                        // Vérification des ombres
                        if (light->castsShadows()) {
                            RayTracer::Ray shadow_ray(Math::Point<3>(rec.intersection + rec.normal * 0.001), light_dir);
                            if (node->hit(shadow_ray, RayTracer::Interval(0.001, distance), rec)) {
                                diffuse *= 0.5;  // Assombrir en cas d'ombre
                                specular *= 0.5;
                            }
                        }
                    }
                }

                //  RayTracer::Color finalColor = recursiveColor * (ambient + diffuse * phongModel.diffuseCoefficient) + specular;
                RayTracer::Color finalColor = recursiveColor * (ambient + diffuse) + specular;

                return finalColor;
            } else {
                // Couleur de fond
                Math::Vector<3> unit_direction = r.direction.normalize();
                auto t = 0.5 * (unit_direction.at(1,0) + 1.0);
                return backgroundColor * (1.0 - t) + RayTracer::Color(0.5, 0.7, 1.0) * t;
            }
        }

        void renderImage(
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights,
                const std::string& filename
        ) {
            if (gui) {
                std::clog << "Creating GUI window..." << std::endl;
                gui->createWindow(imageWidth, imageHeight, "Raytracer");
            }

            renderingThread = std::thread(&Renderer::renderingTask, this, std::ref(scene), std::ref(lights), filename);

            if (gui) {
                while (gui->windowIsOpen()) {
                    gui->handleEvents();
                    {
                        std::lock_guard<std::mutex> lock(renderingMutex);
                        gui->drawPixels(buffer.getData(), imageWidth, imageHeight, 0, 0);
                        gui->display();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }

            if (renderingThread.joinable()) {
                renderingThread.join();
            }
        }

        void renderImageMultithreaded(
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights,
                const std::string& filename
        ) {
            if (gui) {
                std::clog << "Creating GUI window..." << std::endl;
                gui->createWindow(imageWidth, imageHeight, "Raytracer");
            }

            renderingThread = std::thread(&Renderer::renderingTaskMultithreaded, this,
                                          std::ref(scene), std::ref(lights), filename);

            if (gui) {
                while (gui->windowIsOpen()) {
                    gui->handleEvents();
                    {
                        std::lock_guard<std::mutex> lock(renderingMutex);
                        gui->drawPixels(buffer.getData(), imageWidth, imageHeight, 0, 0);
                        gui->display();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }

            if (renderingThread.joinable()) {
                renderingThread.join();
            }
        }

        void renderInteractive(
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights
        ) {
            if (!gui) {
                std::cerr << "GUI must be enabled for interactive rendering mode." << std::endl;
                return;
            }

            std::clog << "Creating GUI window for interactive rendering..." << std::endl;
            gui->createWindow(imageWidth, imageHeight, "Interactive Raytracer");

            interruptRendering.store(false);

            const auto numWorkers = std::thread::hardware_concurrency();
            TaskQueue queue;
            std::vector<std::thread> workers;
            SafeCounter threadCounter;

            for (unsigned int i = 0; i < numWorkers; ++i) {
                workers.emplace_back([&queue, &threadCounter]() {
                    int threadId = threadCounter.getNextID() + 1;
                    queue.workerFunction(threadId);
                });
            }

            while (gui->windowIsOpen()) {
                interruptRendering.store(false);

                queue.addTask([&](int thread_id) {
                    renderBlockInteractive(0, 0, imageWidth, imageHeight, scene, lights, queue);
                });

                while (!interruptRendering.load() && gui->windowIsOpen()) {
                    gui->handleEvents();
                    {
                        std::lock_guard<std::mutex> lock(renderingMutex);
                        gui->drawPixels(buffer.getData(), imageWidth, imageHeight, 0, 0);
                        gui->display();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

                if (interruptRendering.load()) {
                    queue.reset();  // Clear the task queue to stop current tasks
                }
            }

            interruptRendering.store(true);  // Signal to interrupt the rendering thread
            queue.shutdown();  // Properly shutdown the task queue

            for (auto& worker : workers) {
                if (worker.joinable()) {
                    worker.join();  // Join the worker threads to ensure clean exit
                }
            }

            renderingComplete = true;
        }

        void setGui(SFMLGuiPlugin* guiPlugin) {
            this->gui = guiPlugin;
        }

        void setAmbientColor(const Color& color) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            ambientColor = color;
        }

        void setDiffuseColor(const Color& color) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            diffuseColor = color;
        }

        void setSpecularColor(const Color& color) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            specularColor = color;
        }

        void setBackgroundColor(const Color& color) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            backgroundColor = color;
        }

        void setPhongModel(const PhongReflectionModel& model) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            phongModel = model;
        }

        void setSamplesPerPixel(unsigned int samples) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            samplesPerPixel = samples;
        }

        void setMaxDepth(unsigned int depth) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            maxDepth = depth;
        }

        void setCamera(const Camera& newCamera) {
            std::lock_guard<std::mutex> lock(renderingMutex);
            camera = newCamera;
            imageWidth = newCamera.imageWidth;
            imageHeight = newCamera.imageHeight;
            buffer.resize(imageWidth, imageHeight);
        }

        Camera& getCamera() const {
            return camera;
        }

        void interrupt() {
            interruptRendering.store(true);
        }

    private:
        Camera& camera;
        Color ambientColor;
        Color diffuseColor;
        Color specularColor;
        Color backgroundColor;
        PhongReflectionModel phongModel;
        unsigned int samplesPerPixel;
        unsigned int maxDepth;
        mutable std::mutex logMutex;
        SFMLGuiPlugin* gui;
        std::thread renderingThread;
        std::mutex renderingMutex;
        std::condition_variable renderingCV;
        bool renderingComplete;
        SafeBuffer buffer;
        int imageWidth;
        int imageHeight;
        std::atomic<bool> interruptRendering;

        static void logProgress(int current_line, int total_lines) {
            static std::mutex log_mtx;
            std::lock_guard<std::mutex> lock(log_mtx);
            std::clog << "\rRendering line " << current_line + 1 << " of " << total_lines << std::flush;
        }

        static std::vector<std::pair<int, int>> get_factors(int n) {
            std::vector<std::pair<int, int>> factors;
            for (int i = 1; i <= sqrt(n); ++i) {
                if (n % i == 0) {
                    factors.emplace_back(i, n / i);
                    if (i != n / i) {
                        factors.emplace_back(n / i, i);
                    }
                }
            }
            return factors;
        }

        static std::pair<int, int> select_best_factors(const std::vector<std::pair<int, int>>& factors, int width, int height) {
            double aspect_ratio = double(width) / height;
            std::pair<int, int> best_factor = {1, 1};
            double best_ratio_diff = std::numeric_limits<double>::max();

            for (const auto& factor : factors) {
                double current_ratio = double(factor.first) / factor.second;
                double ratio_diff = fabs(current_ratio - aspect_ratio);

                if (ratio_diff < best_ratio_diff) {
                    best_ratio_diff = ratio_diff;
                    best_factor = factor;
                }
            }
            return best_factor;
        }

        void renderBlock(
                int start_x,
                int start_y,
                int end_x,
                int end_y,
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights,
                int thread_id
        ) const {
            {
                std::lock_guard<std::mutex> lock(logMutex);
                std::clog << "Thread " << thread_id
                          << " rendering block (" << start_x << ", " << start_y << ") to ("
                          << end_x << ", " << end_y << ")" << std::endl;
            }
            for (int j = start_y; j < end_y; ++j) {
                for (int i = start_x; i < end_x; ++i) {
                    RayTracer::Color c(0, 0, 0);
                    for (int sample = 0; sample < samplesPerPixel; ++sample) {
                        RayTracer::Ray ray = camera.ray(i, j);
                        c += rayColor(ray, scene, lights, maxDepth);
                    }
                    c *= (1.0 / samplesPerPixel);
                    buffer.setPixelColor(i, j, c);
                }
            }
        }

        void renderBlockInteractive(
                int startX, int startY, int blockWidth, int blockHeight,
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights,
                TaskQueue& queue) {

            if (interruptRendering.load()) {
                return;  // Exit early if an interruption is signaled
            }

            if (blockWidth <= 1 && blockHeight <= 1) {
                int centerX = startX;
                int centerY = startY;

                RayTracer::Color color(0, 0, 0);
                for (int sample = 0; sample < 10 && !interruptRendering.load(); ++sample) {
                    RayTracer::Ray ray = camera.ray(centerX, centerY);
                    color += rayColor(ray, scene, lights, 10);
                }

                color *= (1.0 / 10);

                buffer.setBlockColor(startX, startY, blockWidth, blockHeight, color);
            } else {
                int centerX = startX + blockWidth / 2;
                int centerY = startY + blockHeight / 2;

                RayTracer::Color color(0, 0, 0);
                for (int sample = 0; sample < 10 && !interruptRendering.load(); ++sample) {
                    RayTracer::Ray ray = camera.ray(centerX, centerY);
                    color += rayColor(ray, scene, lights, 10);
                }

                color *= (1.0 / 10);

                buffer.setBlockColor(startX, startY, blockWidth, blockHeight, color);

                int newWidth = std::ceil(blockWidth / 2.0);
                int newHeight = std::ceil(blockHeight / 2.0);

                if (!interruptRendering.load()) {
                    queue.addTask([=, &lights, &queue](int thread_id) {
                        renderBlockInteractive(startX, startY, newWidth, newHeight, scene, lights, queue);
                    });
                }
                if (!interruptRendering.load()) {
                    queue.addTask([=, &lights, &queue](int thread_id) {
                        renderBlockInteractive(startX + newWidth, startY, newWidth, newHeight, scene, lights, queue);
                    });
                }
                if (!interruptRendering.load()) {
                    queue.addTask([=, &lights, &queue](int thread_id) {
                        renderBlockInteractive(startX, startY + newHeight, newWidth, newHeight, scene, lights, queue);
                    });
                }
                if (!interruptRendering.load()) {
                    queue.addTask([=, &lights, &queue](int thread_id) {
                        renderBlockInteractive(startX + newWidth, startY + newHeight, newWidth, newHeight, scene, lights, queue);
                    });
                }
            }
        }


        void renderingTaskMultithreaded(
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights,
                const std::string& filename
        ) {
            std::ofstream file(filename);
            if (!file) {
                std::cerr << "Error opening file " << filename << std::endl;
                return;
            }

            file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            std::clog << "Starting multithreaded image rendering..." << std::endl << std::endl;

            const auto numWorkers = std::thread::hardware_concurrency();
            TaskQueue queue;
            std::vector<std::thread> workers;
            SafeCounter threadCounter;

            for (unsigned int i = 0; i < numWorkers; ++i) {
                workers.emplace_back([&queue, &threadCounter]() {
                    int threadId = threadCounter.getNextID() + 1;
                    queue.workerFunction(threadId);
                });
            }

            auto factors = get_factors(static_cast<int>(numWorkers));
            auto best_factors = select_best_factors(factors, imageWidth, imageHeight);

            int blocks_x = best_factors.first;
            int blocks_y = best_factors.second;

            int block_width = imageWidth / blocks_x;
            int block_height = imageHeight / blocks_y;

            for (int by = 0; by < blocks_y; ++by) {
                for (int bx = 0; bx < blocks_x; ++bx) {
                    int start_x = bx * block_width;
                    int start_y = by * block_height;
                    int end_x = (bx + 1 == blocks_x) ? imageWidth : (bx + 1) * block_width;
                    int end_y = (by + 1 == blocks_y) ? imageHeight : (by + 1) * block_height;

                    queue.addTask([=, &scene, &lights](int thread_id) {
                        renderBlock(start_x, start_y, end_x, end_y, scene, lights, thread_id);
                    });
                }
            }
            queue.shutdown();

            for (auto& worker : workers) {
                worker.join();
            }

            uint8_t* bufferData = buffer.getData();
            for (int j = 0; j < imageHeight; ++j) {
                for (int i = 0; i < imageWidth; ++i) {
                    int index = (j * imageWidth + i) * 4;
                    file << static_cast<int>(bufferData[index]) << ' '
                         << static_cast<int>(bufferData[index + 1]) << ' '
                         << static_cast<int>(bufferData[index + 2]) << '\n';
                }
            }

            file.close();
            std::clog << std::endl << "Multithreaded image rendered and saved to " << filename << std::endl;

            {
                std::lock_guard<std::mutex> lock(renderingMutex);
                renderingComplete = true;
            }
            renderingCV.notify_one();
        }

        void renderingTask(
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights,
                const std::string& filename
        ) {
            std::ofstream file(filename);
            if (!file) {
                std::cerr << "Error opening file " << filename << std::endl;
                return;
            }

            file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            std::clog << "Starting image rendering..." << std::endl;

            for (int j = 0; j < imageHeight; ++j) {
                logProgress(j, imageHeight);
                for (int i = 0; i < imageWidth; ++i) {
                    RayTracer::Color pixel_color(0, 0, 0);

                    for (int sample = 0; sample < samplesPerPixel; ++sample) {
                        RayTracer::Ray ray = camera.ray(i, j);
                        pixel_color += rayColor(ray, scene, lights, maxDepth);
                    }

                    pixel_color *= (1.0 / samplesPerPixel);

                    buffer.setPixelColor(i, j, pixel_color);

                    auto byte_color = pixel_color.to_byte_color();
                    file << static_cast<int>(byte_color[0]) << ' '
                         << static_cast<int>(byte_color[1]) << ' '
                         << static_cast<int>(byte_color[2]) << '\n';
                }
            }

            file.close();
            std::clog << std::endl << "Image rendered and saved to " << filename << std::endl;
            {
                std::lock_guard<std::mutex> lock(renderingMutex);
                renderingComplete = true;
            }
            renderingCV.notify_one();
        }

        void interactiveRenderingTask(
                const std::shared_ptr<RayTracer::INode>& scene,
                const std::vector<std::unique_ptr<RayTracer::ILight>>& lights
        ) {
            std::clog << "Starting interactive rendering..." << std::endl;

            const auto numWorkers = std::thread::hardware_concurrency();
            TaskQueue queue;
            std::vector<std::thread> workers;
            SafeCounter threadCounter;

            for (unsigned int i = 0; i < numWorkers; ++i) {
                workers.emplace_back([&queue, &threadCounter]() {
                    int threadId = threadCounter.getNextID() + 1;
                    queue.workerFunction(threadId);
                });
            }

            queue.addTask([&](int thread_id) {
                renderBlockInteractive(0, 0, imageWidth, imageHeight,scene, lights, queue);
            });

            queue.shutdown();

            for (auto& worker : workers) {
                worker.join();
            }

            std::clog << std::endl << "Interactive rendering task completed." << std::endl;
        }
    };
}

#endif