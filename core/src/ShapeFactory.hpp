/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef SHAPE_FACTORY_HPP
#define SHAPE_FACTORY_HPP

#include <unordered_map>
#include <functional>
#include <memory>
#include <utility>
#include "INode.hpp"
#include "Group.hpp"
#include "Sphere.hpp"
#include "Cylinder.hpp"
#include "MaterialFactory.hpp"
#include "Group.hpp"
#include "LimitedCylinder.hpp"
#include "Triangle.hpp"
#include "Cone.hpp"
#include "LimitedCone.hpp"
#include "TransformationFactory.hpp"
#include "Plane.hpp"
#include "Quad.hpp"

namespace RayTracer {
    class ShapeFactory {
    public:
        using CreatorFunc = std::shared_ptr<INode>(*)(const libconfig::Setting&, MaterialFactory&, std::shared_ptr<Group>);
        std::unordered_map<std::string, CreatorFunc> creators;
        MaterialFactory materialFactory;
        TransformationFactory transformationFactory;
        std::shared_ptr<Group> shapeContainer;

        explicit ShapeFactory(std::shared_ptr<Group>& group);

        void createShape(const std::string& type, const libconfig::Setting& settings, const std::shared_ptr<Group>& group);
        static void addMaterial(std::string& name, std::shared_ptr<RayTracer::IMaterial> material);
        static void printMaterialNames() ;

    private:
        static std::unordered_map<std::string, std::shared_ptr<RayTracer::IMaterial>> materials;

        static std::shared_ptr<INode> createSphere(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for sphere.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double x, y, z, radius;
            if (!shapeSettings.lookupValue("x", x) ||
                !shapeSettings.lookupValue("y", y) ||
                !shapeSettings.lookupValue("z", z) ||
                !shapeSettings.lookupValue("radius", radius)) {
                throw std::runtime_error("Failed to read shape settings for the sphere.");
            }

            auto sphere = std::make_shared<Sphere>(Math::Point<3>({x, y, z}), radius, materialIt->second);
            if (!sphere) {
                throw std::runtime_error("Failed to create sphere.");
            }
            shapeContainer->addNode(sphere);
            return sphere;
        }
        static std::shared_ptr<INode> createCylinder(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for cylinder.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double x, y, z, radius;

            if (!shapeSettings["vector"].lookupValue("x", x) ||
                !shapeSettings["vector"].lookupValue("y", y) ||
                !shapeSettings["vector"].lookupValue("z", z) ||
                !shapeSettings.lookupValue("radius", radius)) {
                throw std::runtime_error("Failed to read shape settings for the cylinder.");
            }
            auto cylinder = std::make_shared<Cylinder>(Math::Point<3>({x, y, z}), radius, materialIt->second);
            if (!cylinder) {
                throw std::runtime_error("Failed to create cylinder.");
            }
            shapeContainer->addNode(cylinder);
            return cylinder;
        }
        static std::shared_ptr<INode> createLimitedCylinder(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for limitedCylinder.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double x, y, z, radius, height;
            if (!shapeSettings["base"].lookupValue("x", x) ||
                !shapeSettings["base"].lookupValue("y", y) ||
                !shapeSettings["base"].lookupValue("z", z) ||
                !shapeSettings.lookupValue("radius", radius) ||
                !shapeSettings.lookupValue("height", height)) {
                throw std::runtime_error("Failed to read shape settings for the limitedCylinder.");
            }
            auto limitedCylinder = std::make_shared<LimitedCylinder>(Math::Point<3>({x, y, z}), radius, height, materialIt->second);
            if (!limitedCylinder) {
                throw std::runtime_error("Failed to create limtedCylinder.");
            }
            shapeContainer->addNode(limitedCylinder);
            return limitedCylinder;
        }
        static std::shared_ptr<INode> createTriangle(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for limitedCylinder.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double v0x, v0y, v0z;
            if (!shapeSettings["v0"].lookupValue("x", v0x) ||
            !shapeSettings["v0"].lookupValue("y", v0y) ||
            !shapeSettings["v0"].lookupValue("z", v0z)) {
                throw std::runtime_error("Failed to read v0 shape settings for the triangle.");
            }
            Math::Point<3> v0({v0x, v0y, v0z});

            double v1x, v1y, v1z;
            if (!shapeSettings["v1"].lookupValue("x", v1x) ||
            !shapeSettings["v1"].lookupValue("y", v1y) ||
            !shapeSettings["v1"].lookupValue("z", v1z)) {
                throw std::runtime_error("Failed to read v1 shape settings for the triangle.");
            }
            Math::Point<3> v1({v1x, v1y, v1z});

            double v2x, v2y, v2z;
            if (!shapeSettings["v2"].lookupValue("x", v2x) ||
            !shapeSettings["v2"].lookupValue("y", v2y) ||
            !shapeSettings["v2"].lookupValue("z", v2z)) {
                throw std::runtime_error("Failed to read v2 shape settings for the triangle.");
            }
            Math::Point<3> v2({v2x, v2y, v2z});

            auto triangle = std::make_shared<Triangle>(v0, v1, v2, materialIt->second);
            if (!triangle) {
                throw std::runtime_error("Failed to create triangle.");
            }
            shapeContainer->addNode(triangle);
            return triangle;
        }
        static std::shared_ptr<INode> createCone(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for cone.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double x, y, z, angle;
            if (!shapeSettings["apex"].lookupValue("x", x) ||
                !shapeSettings["apex"].lookupValue("y", y) ||
                !shapeSettings["apex"].lookupValue("z", z) ||
                !shapeSettings.lookupValue("angle", angle)) {
                throw std::runtime_error("Failed to read shape settings for the cone.");
            }
            auto cone = std::make_shared<Cone>(Math::Point<3>({x, y, z}), angle, materialIt->second);
            if (!cone) {
                throw std::runtime_error("Failed to create cone.");
            }
            shapeContainer->addNode(cone);
            return cone;
        }
        static std::shared_ptr<INode> createLimitedCone(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for limitedCone.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double x, y, z, radius, height;
            if (!shapeSettings["apex"].lookupValue("x", x) ||
                !shapeSettings["apex"].lookupValue("y", y) ||
                !shapeSettings["apex"].lookupValue("z", z) ||
                !shapeSettings.lookupValue("radius", radius) ||
                !shapeSettings.lookupValue("height", height)) {
                throw std::runtime_error("Failed to read shape settings for the limitedCone.");
            }
            auto limitedCone = std::make_shared<LimitedCone>(Math::Point<3>({x, y, z}), radius, height, materialIt->second);
            if (!limitedCone) {
                throw std::runtime_error("Failed to create limitedCone.");
            }
            shapeContainer->addNode(limitedCone);
            return limitedCone;
        }
        static std::shared_ptr<INode> createPlane(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for plane.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double point_x, point_y, point_z;
            double normal_x, normal_y, normal_z;
            if (!shapeSettings["point"].lookupValue("x", point_x) ||
                !shapeSettings["point"].lookupValue("y", point_y) ||
                !shapeSettings["point"].lookupValue("z", point_z) ||
                !shapeSettings["normal"].lookupValue("x", normal_x) ||
                !shapeSettings["normal"].lookupValue("y", normal_y) ||
                !shapeSettings["normal"].lookupValue("z", normal_z)) {
                throw std::runtime_error("Failed to read shape settings for the plane.");
            }
            auto plane = std::make_shared<Plane>(Math::Point<3>({point_x, point_y, point_z}), Math::Vector<3>({normal_x, normal_y, normal_z}), materialIt->second);
            if (!plane) {
                throw std::runtime_error("Failed to create plane.");
            }
            shapeContainer->addNode(plane);
            return plane;
        }
        static std::shared_ptr<INode> createQuad(const libconfig::Setting& settings, MaterialFactory& materialFactory, std::shared_ptr<Group> shapeContainer) {
            const libconfig::Setting& shapeSettings = settings["shape"];
            std::string materialName;

            if (!settings["material"].lookupValue("name", materialName)) {
                throw std::runtime_error("Material name not specified for quad.");
            }

            auto materialIt = materials.find(materialName);
            if (materialIt == materials.end()) {
                throw std::runtime_error("Material '" + materialName + "' not found.");
            }

            double q_x, q_y, q_z, u_x, u_y, u_z, v_x, v_y, v_z;
            double normal_x, normal_y, normal_z;
            if (!shapeSettings["Q"].lookupValue("x", q_x) ||
                !shapeSettings["Q"].lookupValue("y", q_y) ||
                !shapeSettings["Q"].lookupValue("z", q_z) ||
                !shapeSettings["u"].lookupValue("x", u_x) ||
                !shapeSettings["u"].lookupValue("y", u_y) ||
                !shapeSettings["u"].lookupValue("z", u_z) ||
                !shapeSettings["v"].lookupValue("x", v_x) ||
                !shapeSettings["v"].lookupValue("y", v_y) ||
                !shapeSettings["v"].lookupValue("z", v_z)) {
                throw std::runtime_error("Failed to read shape settings for the quad.");
            }
            auto quad = std::make_shared<Quad>(Math::Point<3>({q_x, q_y, q_z}), Math::Vector<3>({u_x, u_y, u_z}), Math::Vector<3>({v_x, v_y, v_z}), materialIt->second);
            if (!quad) {
                throw std::runtime_error("Failed to create quad.");
            }
            shapeContainer->addNode(quad);
            return quad;
        }
    };
}

#endif
