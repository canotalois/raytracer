/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef TRANSFORMATION_FACTORY_HPP
#define TRANSFORMATION_FACTORY_HPP

#include <libconfig.h++>
#include "Transformation.hpp"
#include <unordered_map>

namespace RayTracer {
    class TransformationFactory {
    public:
        using TranformationCreatorFunc = std::function<void(const libconfig::Setting&, RayTracer::Transformation&)>;

        std::unordered_map<std::string, TranformationCreatorFunc> transformationCreators;

        TransformationFactory() {
            transformationCreators["Translation"] = &TransformationFactory::createTranslation;
            transformationCreators["Rotation"] = &TransformationFactory::createRotation;
            transformationCreators["Scale"] = &TransformationFactory::createScale;
            transformationCreators["Shear"] = &TransformationFactory::createShear;
        }

        void createTransformation(const std::string& type, const libconfig::Setting& settings, RayTracer::Transformation& t) {
            auto it = transformationCreators.find(type);
            if (it != transformationCreators.end()) {
                it->second(settings, t);
                return;
            }
            throw std::runtime_error("Transformation type'" + type + "' not supported.");
        }
    private:
        static void createTranslation(const libconfig::Setting& settings, RayTracer::Transformation& t) {
            const libconfig::Setting& vectorSetting = settings["vector"];
            double x, y, z;
            vectorSetting.lookupValue("x", x);
            vectorSetting.lookupValue("y", y);
            vectorSetting.lookupValue("z", z);
            t.setTranslation(Math::Vector<3>({x, y, z}));
        }
        static void createRotation(const libconfig::Setting& settings, RayTracer::Transformation& t) {
            const libconfig::Setting& vectorSetting = settings["vector"];
            double angle;
            double x, y, z;
            settings.lookupValue("angle", angle);
            vectorSetting.lookupValue("x", x);
            vectorSetting.lookupValue("y", y);
            vectorSetting.lookupValue("z", z);
            t.setRotation(angle, Math::Vector<3>({x, y, z}));
        }
        static void createScale(const libconfig::Setting& settings, RayTracer::Transformation& t) {
            const libconfig::Setting& vectorSetting = settings["vector"];
            double x, y, z;
            vectorSetting.lookupValue("x", x);
            vectorSetting.lookupValue("y", y);
            vectorSetting.lookupValue("z", z);
            t.setScale(Math::Vector<3>({x, y, z}));
        }
        static void createShear(const libconfig::Setting& settings, RayTracer::Transformation& t) {
            const libconfig::Setting& vectorSetting = settings["vector"];
            double s_xy, s_xz, s_yx, s_yz, s_zx, s_zy;
            vectorSetting.lookupValue("s_xy", s_xy);
            vectorSetting.lookupValue("s_xz", s_xz);
            vectorSetting.lookupValue("s_yx", s_yx);
            vectorSetting.lookupValue("s_yz", s_yz);
            vectorSetting.lookupValue("s_zx", s_zx);
            vectorSetting.lookupValue("s_zy", s_zy);
            t.setShear(s_xy, s_xz, s_yx, s_yz, s_zx, s_zy);
        }
    };
}

#endif
