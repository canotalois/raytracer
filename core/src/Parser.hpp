/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include <libconfig.h++>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Camera.hpp"
#include "INode.hpp"
#include "ShapeFactory.hpp"
#include "ILight.hpp"
#include "LightFactory.hpp"
#include "PhongReflectionModel.hpp"
#include "Renderer.hpp"

namespace RayTracer {
    class Parser {
    private:
        libconfig::Config _cfg;
        std::string _configFilePath;
        std::vector<std::unique_ptr<RayTracer::ILight>> _lights;
        RayTracer::Camera& _camera;
        ShapeFactory& shapeFactory;
        LightFactory& lightFactory;
        std::shared_ptr<RayTracer::Group>& parentGroup;
        static PhongReflectionModel phongModel;
        RayTracer::Renderer& _renderer;
        std::string filename;
        bool GUIState;

    public:
        explicit Parser(std::string configFilePath, ShapeFactory& shapeFactory, LightFactory& lightFactory, std::shared_ptr<RayTracer::Group>& parentGroup, RayTracer::Camera& camera, RayTracer::Renderer &renderer);
        void parseCamera();
        void parseMaterials();
        void parseGroup();
        void parseSubGroup(const libconfig::Setting& groupSettings, const std::shared_ptr<RayTracer::Group>& parentGroup);
        Transformation parseTransformation(const libconfig::Setting& trans);
        void parseChildren(const libconfig::Setting& children, const std::shared_ptr<RayTracer::Group>& group);
        void parseLights(const libconfig::Setting& lights);
        static void parsePhongModel(const libconfig::Setting& phongModel);
        std::string parseRenderer();
        void parseGUI();

        [[nodiscard]] const std::vector<std::unique_ptr<RayTracer::ILight>>& getLights() const;
        [[nodiscard]] const RayTracer::Camera& getCamera() const;
        [[nodiscard]] PhongReflectionModel getPhongModel() const;
        [[nodiscard]] Renderer& getRenderer() const;
        [[nodiscard]] std::shared_ptr<RayTracer::Group>& getParentGroup() const;
        [[nodiscard]] std::string getFilename() const;
        bool getGUIState() const;

    };
}

#endif
