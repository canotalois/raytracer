/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "Parser.hpp"
#include <utility>

PhongReflectionModel RayTracer::Parser::phongModel;

RayTracer::Parser::Parser(std::string configFilePath, ShapeFactory& shapeFactory, LightFactory& lightFactory, std::shared_ptr<RayTracer::Group>& parentGroup, RayTracer::Camera& camera, RayTracer::Renderer& renderer) : _configFilePath(std::move(configFilePath)), shapeFactory(shapeFactory), lightFactory(lightFactory), parentGroup(parentGroup), _camera(camera), _renderer(renderer)
{
    try {
        GUIState = false;
        _cfg.readFile(_configFilePath.c_str());
    } catch(const libconfig::ParseException &e) {
        std::cerr << "Parse error at " << e.getFile() << ":" << e.getLine() << " - " << e.getError() << std::endl;
    }
}

void RayTracer::Parser::parseGUI()
{
    const libconfig::Setting& root = _cfg.getRoot();
    libconfig::Setting& guiSettings = root["GUI"];

    std::string state;
    if (!guiSettings.lookupValue("state", state)) {
        throw std::runtime_error("State of GUI isn't correct or missing.");
    }
    if (state == "true") {
        GUIState = true;
    } else if (state == "false") {
        GUIState = false;
    } else {
        throw std::runtime_error("State of GUI isn't correct or missing.");
    }
}

void RayTracer::Parser::parseCamera()
{
    const libconfig::Setting& root = _cfg.getRoot();
    libconfig::Setting& cameraSettings = root["camera"];

    double aspectRatio, fieldOfView;
    int imageWidth;
    Math::Point<3> position, pointOfView;
    Math::Vector<3> rotation;

    if (!cameraSettings.lookupValue("aspectRatio", aspectRatio)) {
        throw std::runtime_error("Aspect Ratio not specified in the camera settings.");
    }
    if (!cameraSettings.lookupValue("imageWidth", imageWidth)) {
        throw std::runtime_error("Image width not specified in the camera settings.");
    }
    if (!cameraSettings.lookupValue("fieldOfView", fieldOfView)) {
        throw std::runtime_error("Field of view not specified in the camera settings.");
    }
    const libconfig::Setting& positionSetting = cameraSettings["position"];
    position = Math::Point<3>({
        positionSetting["x"],
        positionSetting["y"],
        positionSetting["z"]
    });
    const libconfig::Setting& pointOfViewSetting = cameraSettings["pointOfView"];
    pointOfView = Math::Point<3>({
        pointOfViewSetting["x"],
        pointOfViewSetting["y"],
        pointOfViewSetting["z"]
    });
    const libconfig::Setting& rotationSetting = cameraSettings["cameraUpVector"];
    rotation = Math::Vector<3>({
        rotationSetting["x"],
        rotationSetting["y"],
        rotationSetting["z"]
    });
    auto camera = RayTracer::Camera(aspectRatio, imageWidth, fieldOfView, position, pointOfView, rotation);
    _camera = camera;
}

std::string RayTracer::Parser::parseRenderer()
{
    const libconfig::Setting& root = _cfg.getRoot();
    libconfig::Setting& rendererSettings = root["Renderer"];

    RayTracer::Color ambientColor, diffuseColor, specularColor, backgroundColor;
    unsigned int samplesPerPixel, maxDepth;
    double r, g, b;
    std::string mode;
    std::string outputFilename = "output.ppm";

    const libconfig::Setting &ambientColorSetting = rendererSettings["ambientColor"];
    if (ambientColorSetting.lookupValue("r", r) &&
        ambientColorSetting.lookupValue("g", g) &&
        ambientColorSetting.lookupValue("b", b)) {
        ambientColor = RayTracer::Color(r, g, b);
    } else {
        throw std::runtime_error("Ambient color settings are incomplete.");
    }

    const libconfig::Setting &diffuseColorSetting = rendererSettings["diffuseColor"];
    if (diffuseColorSetting.lookupValue("r", r) &&
        diffuseColorSetting.lookupValue("g", g) &&
        diffuseColorSetting.lookupValue("b", b)) {
        diffuseColor = RayTracer::Color(r, g, b);
    } else {
        throw std::runtime_error("Diffuse color settings are incomplete.");
    }

    const libconfig::Setting &specularColorSetting = rendererSettings["specularColor"];
    if (specularColorSetting.lookupValue("r", r) &&
        specularColorSetting.lookupValue("g", g) &&
        specularColorSetting.lookupValue("b", b)) {
        specularColor = RayTracer::Color(r, g, b);
    } else {
        throw std::runtime_error("Specular color settings are incomplete.");
    }

    const libconfig::Setting &backgroundColorSetting = rendererSettings["backgroundColor"];
    if (backgroundColorSetting.lookupValue("r", r) &&
        backgroundColorSetting.lookupValue("g", g) &&
        backgroundColorSetting.lookupValue("b", b)) {
        backgroundColor = RayTracer::Color(r, g, b);
    } else {
        throw std::runtime_error("Background color settings are incomplete.");
    }

    if (!rendererSettings.lookupValue("samplesPerPixel", samplesPerPixel)) {
        throw std::runtime_error("Samples per pixel setting is missing.");
    }

    if (!rendererSettings.lookupValue("maxDepth", maxDepth)) {
        throw std::runtime_error("Max depth setting is missing.");
    }
    if (!rendererSettings.lookupValue("mode", mode)) {
        throw std::runtime_error("mode setting is missing.");
    }
    rendererSettings.lookupValue("outputFilename", outputFilename);
    this->filename = outputFilename;
    _renderer.setCamera(_camera);
    _renderer.setAmbientColor(ambientColor);
    _renderer.setDiffuseColor(diffuseColor);
    _renderer.setSpecularColor(specularColor);
    _renderer.setBackgroundColor(backgroundColor);
    _renderer.setPhongModel(phongModel);
    return mode;
}

void RayTracer::Parser::parseMaterials()
{
    const libconfig::Setting& root = _cfg.getRoot();
    if (!root.exists("materials")) {
        throw std::runtime_error("'materials' section doesn't exist in configuration file.");
    }

    const libconfig::Setting& materials = root["materials"];

    for (int i = 0; i < materials.getLength(); ++i) {
        const libconfig::Setting& materialSetting = materials[i];

        std::string name = materialSetting.getName();
        std::string type;
        if (!materialSetting.lookupValue("type", type)) {
            throw std::runtime_error("Material type not specified for " + name);
        }
        std::shared_ptr<RayTracer::IMaterial> material = shapeFactory.materialFactory.createMaterial(type, materialSetting);
        if (!material) {
            throw std::runtime_error("Failed to create material for " + name);
        }
        RayTracer::ShapeFactory::addMaterial(name, material);
    }
}

void RayTracer::Parser::parseGroup()
{
    const libconfig::Setting& root = _cfg.getRoot();

    if (!root.exists("group")) {
        throw std::runtime_error("The 'group' section doesn't exist in the configuration file.");
    }
    const libconfig::Setting& groupSettings = root["group"];
//    if (!groupSettings.exists("children")) {
//    }
    parseSubGroup(groupSettings, parentGroup);
}

void RayTracer::Parser::parseSubGroup(const libconfig::Setting& groupSetting, const std::shared_ptr<RayTracer::Group>& parentGroup)
{
    std::shared_ptr<RayTracer::Group> currentGroup = std::make_shared<RayTracer::Group>();

    if (groupSetting.exists("transformations")) {
        const libconfig::Setting& transformations = groupSetting["transformations"];
        for (int i = 0; i < transformations.getLength(); ++i) {
            const libconfig::Setting& trans = transformations[i];
            RayTracer::Transformation transformation = parseTransformation(trans);
            currentGroup->setTransformation(transformation);
        }
    }
    parentGroup->addNode(currentGroup);
    if (groupSetting.exists("children")) {
        const libconfig::Setting& children = groupSetting["children"];
        parseChildren(children, currentGroup);
        if (children.exists("subgroups")) {
            const libconfig::Setting& subgroups = children["subgroups"];
            for (int i = 0; i < subgroups.getLength(); ++i) {
                parseSubGroup(subgroups[i], currentGroup);
            }
        }
    }
}

RayTracer::Transformation RayTracer::Parser::parseTransformation(const libconfig::Setting& transformationSettings)
{
    std::string type;
    if (!transformationSettings.lookupValue("type", type)) {
        throw std::runtime_error("Transformation type not specified");
    }

    RayTracer::Transformation transformation;
    if (type == "Translation") {
        double x = 0, y = 0, z = 0;
        transformationSettings["vector"].lookupValue("x", x);
        transformationSettings["vector"].lookupValue("y", y);
        transformationSettings["vector"].lookupValue("z", z);
        transformation.setTranslation(Math::Vector<3>({x, y, z}));
    } else if (type == "Rotation") {
        double angle = 0, x = 0, y = 0, z = 0;
        transformationSettings.lookupValue("angle", angle);
        transformationSettings["vector"].lookupValue("x", x);
        transformationSettings["vector"].lookupValue("y", y);
        transformationSettings["vector"].lookupValue("z", z);
        transformation.setRotation(angle, Math::Vector<3>({x, y, z}));
    } else if (type == "Scale") {
        double x = 1, y = 1, z = 1;
        transformationSettings["vector"].lookupValue("x", x);
        transformationSettings["vector"].lookupValue("y", y);
        transformationSettings["vector"].lookupValue("z", z);
        transformation.setScale(Math::Vector<3>({x, y, z}));
    } else {
        throw std::runtime_error("Unsupported transformation type");
    }
    return transformation;
}

void RayTracer::Parser::parseChildren(const libconfig::Setting& children, const std::shared_ptr<RayTracer::Group>& group)
{
    for (int i = 0; i < children.getLength(); ++i) {
        const libconfig::Setting& child = children[i];
        std::string category = child.getName();

        if (category == "phongReflectionModel") {
            parsePhongModel(child);
        } else if (category == "lights") {
            parseLights(child);
        } else {
            for (int j = 0; j < child.getLength(); ++j) {
                const libconfig::Setting &shapeConfig = child[j];
                shapeFactory.createShape(category, shapeConfig, group);
            }
        }
    }
}


void RayTracer::Parser::parseLights(const libconfig::Setting& lights)
{
    for (int i = 0; i < lights.getLength(); ++i) {
        const libconfig::Setting& lightConfig = lights[i];
        std::string type;
        if (!lightConfig.lookupValue("type", type)) {
            throw std::runtime_error("Light type not specified.");
        }
        std::unique_ptr<RayTracer::ILight> light = lightFactory.createLight(type, lightConfig);
        if (light) {
            _lights.push_back(std::move(light));
        } else {
            throw std::runtime_error("Failed to create light of type " + type);
        }
    }
}

void RayTracer::Parser::parsePhongModel(const libconfig::Setting& phongModelSettings)
{
    double ambientCoefficient, diffuseCoefficient, specularCoefficient, shininess, constantAttenuation, linearAttenuation, quadraticAttenuation;
    if (!phongModelSettings.lookupValue("ambientCoefficient", ambientCoefficient) ||
    !phongModelSettings.lookupValue("diffuseCoefficient", diffuseCoefficient) ||
    !phongModelSettings.lookupValue("specularCoefficient", specularCoefficient) ||
    !phongModelSettings.lookupValue("shininess", shininess) ||
    !phongModelSettings.lookupValue("constantAttenuation", constantAttenuation) ||
    !phongModelSettings.lookupValue("linearAttenuation", linearAttenuation) ||
    !phongModelSettings.lookupValue("quadraticAttenuation", quadraticAttenuation)) {
        throw std::runtime_error("Phong Model Reflection settings are incorrect or missing.");
    }
    phongModel.setAmbientCoefficient(ambientCoefficient).setDiffuseCoefficient(diffuseCoefficient).setSpecularCoefficient(specularCoefficient).setShininess(shininess).setConstantAttenuation(constantAttenuation).setLinearAttenuation(linearAttenuation).setQuadraticAttenuation(quadraticAttenuation);
}

const std::vector<std::unique_ptr<RayTracer::ILight>> & RayTracer::Parser::getLights() const
{
    return _lights;
}

const RayTracer::Camera& RayTracer::Parser::getCamera() const
{
    return _camera;
}

PhongReflectionModel RayTracer::Parser::getPhongModel() const
{
    return phongModel;
}

RayTracer::Renderer& RayTracer::Parser::getRenderer() const
{
    return _renderer;
}

std::shared_ptr<RayTracer::Group>& RayTracer::Parser::getParentGroup() const {
    return parentGroup;
}

std::string RayTracer::Parser::getFilename() const
{
    return filename;
}

bool RayTracer::Parser::getGUIState() const {
    return GUIState;
}

