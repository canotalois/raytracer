/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "ShapeFactory.hpp"
#include <utility>

std::unordered_map<std::string, std::shared_ptr<RayTracer::IMaterial>> RayTracer::ShapeFactory::materials;

RayTracer::ShapeFactory::ShapeFactory(std::shared_ptr<Group>& group) : shapeContainer(group)
{
    creators["sphere"] = &ShapeFactory::createSphere;
    creators["cylinder"] = &ShapeFactory::createCylinder;
    creators["limitedCylinder"] = &ShapeFactory::createLimitedCylinder;
    creators["triangle"] = &ShapeFactory::createTriangle;
    creators["cone"] = &ShapeFactory::createCone;
    creators["limitedCone"] = &ShapeFactory::createLimitedCone;
    creators["plane"] = &ShapeFactory::createPlane;
    creators["quad"] = &ShapeFactory::createQuad;
}

void RayTracer::ShapeFactory::createShape(const std::string &type, const libconfig::Setting &settings, const std::shared_ptr<Group>& group)
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        auto shape = it->second(settings, materialFactory, group);
        if (settings.exists("transformations")) {
            RayTracer::Transformation transformation;
            const libconfig::Setting& transformationArray = settings["transformations"];
            for (int i = 0; i < transformationArray.getLength(); ++i) {
                const libconfig::Setting& transformationSettings = transformationArray[i];
                std::string transformationType;
                if (transformationSettings.lookupValue("type", transformationType)) {
                    transformationFactory.createTransformation(transformationType, transformationSettings, transformation);
                }
            }
            shape->setTransformation(transformation);
        }
        if (shape) {
            group->addNode(shape);
        }
    } else {
        throw std::runtime_error("Shape type '" + type + "' not supported.");
    }
}

void RayTracer::ShapeFactory::addMaterial(std::string &name, std::shared_ptr<RayTracer::IMaterial> material)
{
    RayTracer::ShapeFactory::materials[name] = std::move(material);
}

void RayTracer::ShapeFactory::printMaterialNames()
{
    for (const auto& pair : materials) {
        std::cout << pair.first << std::endl;
    }
}
