/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef MATERIAL_FACTORY_HPP
#define MATERIAL_FACTORY_HPP

#include <libconfig.h++>
#include "IMaterial.hpp"
#include "Lambertian.hpp"
#include "Dielectric.hpp"
#include "Metal.hpp"
#include "FlatColor.hpp"

namespace RayTracer {
    class MaterialFactory {
    public:
        using MaterialCreatorFunc = std::shared_ptr<IMaterial>(*)(const libconfig::Setting &);

        std::unordered_map<std::string, MaterialCreatorFunc> materialCreators;

        MaterialFactory() {
            materialCreators["Lambertian"] = &MaterialFactory::createLambertian;
            materialCreators["Metal"] = &MaterialFactory::createMetal;
            materialCreators["Dielectric"] = &MaterialFactory::createDielectric;
            materialCreators["FlatColor"] = &MaterialFactory::createFlatColor;
        }

        std::shared_ptr<IMaterial> createMaterial(const std::string &type, const libconfig::Setting &settings) {
            auto it = materialCreators.find(type);
            if (it != materialCreators.end()) {
                return it->second(settings);
            }
            return nullptr;
        }

    private:
        static std::shared_ptr<IMaterial> createLambertian(const libconfig::Setting &settings) {
            const libconfig::Setting& colorSettings = settings["color"];

            double r, g, b;
            colorSettings.lookupValue("r", r);
            colorSettings.lookupValue("g", g);
            colorSettings.lookupValue("b", b);
            return std::make_shared<Lambertian>(Color(r, g, b));
        }

        static std::shared_ptr<IMaterial> createMetal(const libconfig::Setting &settings) {
            const libconfig::Setting& colorSettings = settings["color"];

            double r, g, b, scattered;
            colorSettings.lookupValue("r", r);
            colorSettings.lookupValue("g", g);
            colorSettings.lookupValue("b", b);
            settings.lookupValue("scattered", scattered);
            return std::make_shared<Metal>(Color(r, g, b), scattered);
        }

        static std::shared_ptr<IMaterial> createDielectric(const libconfig::Setting &settings) {
            double ri;
            settings.lookupValue("refractiveIndex", ri);
            return std::make_shared<Dielectric>(ri);
        }
        static std::shared_ptr<IMaterial> createFlatColor(const libconfig::Setting& settings) {
            const libconfig::Setting& colorSettings = settings["color"];

            double r, g, b, scattered;
            colorSettings.lookupValue("r", r);
            colorSettings.lookupValue("g", g);
            colorSettings.lookupValue("b", b);
            return std::make_shared<FlatColor>(Color(r, g, b));
        }
    };
}

#endif