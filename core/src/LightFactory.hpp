/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef LIGHTFACTORY_HPP
#define LIGHTFACTORY_HPP

#include <libconfig.h++>
#include <memory>
#include "ILight.hpp"
#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

namespace RayTracer {
    class LightFactory {
    public:
        using LightCreatorFunc = std::unique_ptr<ILight>(*)(const libconfig::Setting&);
        std::unordered_map<std::string, LightCreatorFunc> lightCreators;

        LightFactory() {
            lightCreators["Ambient"] = &LightFactory::createAmbient;
            lightCreators["Directional"] = &LightFactory::createDirectional;
            lightCreators["Point"] = &LightFactory::createPoint;
        }

        std::unique_ptr<ILight> createLight(const std::string& type, const libconfig::Setting& settings) {
            auto it = lightCreators.find(type);
            if (it != lightCreators.end()) {
                return it->second(settings);
            }
            throw std::runtime_error("'" + type + "' light type isn't supported.");
        }

    private:
        static std::unique_ptr<ILight> createAmbient(const libconfig::Setting& settings) {
            const libconfig::Setting& colorSettings = settings["color"];
            std::string type;
            double r, g, b;
            settings.lookupValue("type", type);

            if (!colorSettings.lookupValue("r", r) ||
            !colorSettings.lookupValue("g", g) ||
            !colorSettings.lookupValue("b", b)) {
                throw std::runtime_error("Color of a '" + type + "' light isn't correct or missing.");
            }
            return std::make_unique<AmbientLight>(RayTracer::Color(r, g, b));
        }
        static std::unique_ptr<ILight> createDirectional(const libconfig::Setting& settings) {
            const libconfig::Setting& directionSettings = settings["direction"];
            const libconfig::Setting& colorSettings = settings["color"];
            std::string type;
            double x, y, z;
            double r, g, b;

            settings.lookupValue("type", type);
            if (!directionSettings.lookupValue("x", x) ||
            !directionSettings.lookupValue("y", y) ||
            !directionSettings.lookupValue("z", z)) {
                throw std::runtime_error("Direction of a '" + type + "' light isn't correct or missing.");
            }
            if (!colorSettings.lookupValue("r", r) ||
                !colorSettings.lookupValue("g", g) ||
                !colorSettings.lookupValue("b", b)) {
                throw std::runtime_error("Color of a '" + type + "' light isn't correct or missing.");
            }
            return std::make_unique<DirectionalLight>(Math::Vector<3>({x, y, z}), RayTracer::Color(r, g, b));
        }
        static std::unique_ptr<ILight> createPoint(const libconfig::Setting& settings) {
            const libconfig::Setting& positionSettings = settings["position"];
            const libconfig::Setting& colorSettings = settings["color"];
            std::string type;
            double x, y, z;
            double r, g, b;

            settings.lookupValue("type", type);
            if (!positionSettings.lookupValue("x", x) ||
                !positionSettings.lookupValue("y", y) ||
                !positionSettings.lookupValue("z", z)) {
                throw std::runtime_error("Position of a '" + type + "' light isn't correct or missing.");
            }
            if (!colorSettings.lookupValue("r", r) ||
                !colorSettings.lookupValue("g", g) ||
                !colorSettings.lookupValue("b", b)) {
                throw std::runtime_error("Color of a '" + type + "' light isn't correct or missing.");
            }
            return std::make_unique<PointLight>(Math::Point<3>({x, y, z}), RayTracer::Color(r, g, b));
        }
    };
}

#endif
