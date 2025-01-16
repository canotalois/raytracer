/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "AMaterial.hpp"

namespace RayTracer {
    class Dielectric : public AMaterial {
    public:
        double refrIndex;

        explicit Dielectric(double ri) : refrIndex(ri) {}

        bool scatter(
                const Ray& rIn, const Intersection& rec, Color& attenuation, Ray& scattered
        ) const override {
            attenuation = Color(1.0, 1.0, 1.0);
            double refractionRatio = rec.frontFace ? (1.0 / refrIndex) : refrIndex;

            Math::Vector<3> unitDirection = rIn.direction.normalize();
            double cosTheta = fmin((-unitDirection).dot(rec.normal), 1.0);
            double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

            bool cannotRefract = refractionRatio * sinTheta > 1.0;
            Math::Vector<3> direction;

            if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble()) {
                direction = Math::Vector<3>::reflect(unitDirection, rec.normal);
            } else {
                direction = Math::Vector<3>::refract(unitDirection, rec.normal, refractionRatio);
            }

            scattered = Ray(rec.intersection, direction);
            return true;
        }

    private:
        static double reflectance(double cosine, double refIdx) {
            auto r0 = (1 - refIdx) / (1 + refIdx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }

        static double randomDouble() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 1);

            return dis(gen);
        }
    };
}

#endif
