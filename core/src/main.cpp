#include "Camera.hpp"
#include "IGuiPlugin.hpp"
#include "DLLoader.hpp"
#include "Color.hpp"
#include "Interval.hpp"
#include "Group.hpp"
#include "DirectionalLight.hpp"
#include "Renderer.hpp"
#include "EventCallbackWrapper.hpp"
#include "ShapeFactory.hpp"
#include "LightFactory.hpp"
#include "Parser.hpp"

int main(int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "No enough arguments." << std::endl;
        return 84;
    }
    std::string filename = av[1];

    try {
        if (filename.find(".obj") != std::string::npos) {
                std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Could not open file." << std::endl;
                return 1;
            }
            RayTracer::DLLoader loader("plugins/libraytracer_gui.so");
            auto *gui = loader.getInstance<SFMLGuiPlugin>("create_plugin");
            std::vector<Math::Point<3>> points;
            std::vector<std::shared_ptr<RayTracer::Triangle>> triangles;
            auto material_right  = std::make_shared<RayTracer::Metal>(RayTracer::Color(0.7, 0.6, 0.5), 0.0);
            RayTracer::Camera camera;
            RayTracer::Renderer renderer(camera);
            renderer.setGui(gui);
            std::shared_ptr<RayTracer::Group> mainGroup = std::make_shared<RayTracer::Group>();
            std::vector<std::unique_ptr<RayTracer::ILight>> lights;
            lights.push_back(std::make_unique<RayTracer::AmbientLight>(RayTracer::Color{0.2, 0.2, 0.2}));

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string type;
                iss >> type;
                if (type == "v") {
                    double x, y, z;
                    if (!(iss >> x >> y >> z)) {
                        std::cerr << "Invalid vertex line: " << line << std::endl;
                        continue;
                    }
                    points.emplace_back(Math::Point<3>({x, y, z}));
                } else if (type == "f") {
                    int p1_idx, p2_idx, p3_idx;
                    if (!(iss >> p1_idx >> p2_idx >> p3_idx)) {
                        std::cerr << "Invalid face line: " << line << std::endl;
                        continue;
                    }
                    auto triangle = std::make_shared<RayTracer::Triangle>(points[p1_idx - 1], points[p2_idx - 1], points[p3_idx - 1], material_right);
                    triangles.push_back(triangle);
                }
            }
            for (const auto& triangle : triangles)
                mainGroup->addNode(triangle);
            camera.setCameraPosition({0,0,5});
            camera.setLookAtPoint({0,0,0});
            renderer.renderImageMultithreaded(mainGroup, lights, "output.ppm");
            return 0;
        } else {
            RayTracer::DLLoader loader("plugins/libraytracer_gui.so");
            auto *gui = loader.getInstance<SFMLGuiPlugin>("create_plugin");

            std::shared_ptr<RayTracer::Group> mainGroup = std::make_shared<RayTracer::Group>();
            RayTracer::ShapeFactory shapeFactory(mainGroup);
            RayTracer::LightFactory lightFactory;
            RayTracer::Camera camera;
            RayTracer::Renderer renderer(camera);
            RayTracer::Parser parser(av[1], shapeFactory, lightFactory, mainGroup, camera, renderer);
            parser.parseGUI();
            parser.parseCamera();
            parser.parseMaterials();
            parser.parseGroup();
            std::string mode = parser.parseRenderer();
            renderer.setCamera(parser.getCamera());

            if (parser.getGUIState()) {
                renderer.setGui(dynamic_cast<SFMLGuiPlugin *>(gui));
            }

            RayTracer::EventCallbackWrapper callbackWrapper(renderer.getCamera(), renderer);
            gui->registerCallback(EventType::KeyEvent, RayTracer::EventCallbackWrapper::KeyEventCallback,
                                  &callbackWrapper);

            if (mode == "Interactive") {
                renderer.renderInteractive(parser.getParentGroup(), parser.getLights());
            } else if (mode == "Image") {
                renderer.renderImage(parser.getParentGroup(), parser.getLights(), parser.getFilename());
            } else if (mode == "ImageMultithreaded") {
                renderer.renderImageMultithreaded(parser.getParentGroup(), parser.getLights(), parser.getFilename());
            } else {
                throw std::runtime_error("mode setting isn't specified.");
            }
        }
    } catch (std::exception &e) {
        std::clog << e.what() << std::endl;
        return 84;
    }

    return 0;
}
