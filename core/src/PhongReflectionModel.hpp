/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef PHONG_REFLECTION_MODEL_HPP
#define PHONG_REFLECTION_MODEL_HPP

#include "AMaterial.hpp"

struct PhongReflectionModel {
    double ambientCoefficient = 1;
    double diffuseCoefficient = 0.9;
    double specularCoefficient = 1.0;
    double shininess = 32;
    double constantAttenuation = 1.0;
    double linearAttenuation = 0.09;
    double quadraticAttenuation = 0.032;

    PhongReflectionModel& setAmbientCoefficient(double value) {
        ambientCoefficient = value;
        return *this;
    }

    PhongReflectionModel& setDiffuseCoefficient(double value) {
        diffuseCoefficient = value;
        return *this;
    }

    PhongReflectionModel& setSpecularCoefficient(double value) {
        specularCoefficient = value;
        return *this;
    }

    PhongReflectionModel& setShininess(double value) {
        shininess = value;
        return *this;
    }

    PhongReflectionModel& setConstantAttenuation(double value) {
        constantAttenuation = value;
        return *this;
    }

    PhongReflectionModel& setLinearAttenuation(double value) {
        linearAttenuation = value;
        return *this;
    }

    PhongReflectionModel& setQuadraticAttenuation(double value) {
        quadraticAttenuation = value;
        return *this;
    }
};

#endif
