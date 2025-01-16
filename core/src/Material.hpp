/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Ray.hpp"
#include "Color.hpp"
#include "Intersection.hpp"

namespace RayTracer {
    class Intersection;

    class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(
                const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered
        ) const {
            return false;
        }
    };

    class Lambertian : public Material {
    public:
        Lambertian(const Color& albedo) : albedo(albedo) {}

        bool scatter(const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered)
        const override {
            auto scatter_direction =  Math::Vector(rec.normal + Math::Vector<3>::random_unit_vector());

            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = Ray(rec.intersection, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Color albedo;
    };

    double random_double() {
        // Création d'un générateur de nombres aléatoires
        std::random_device rd;
        std::mt19937 gen(rd());

        // Distribution uniforme entre 0 et 1
        std::uniform_real_distribution<> dis(0, 1);

        return dis(gen);
    }

    class Metal : public Material {
    public:
        Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(std::min(fuzz, 1.0)) {}

        bool scatter(const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered)
        const override {
            //Math::Vector<N,T> reflected = Math::Vector<N,T>::reflect(r_in.direction, rec.normal);

            //scattered = Ray<N,T>(rec.intersection, reflected);
            //attenuation = albedo;
            //return true;


            Math::Vector reflected = Math::Vector<3>::reflect(r_in.direction, rec.normal);

            reflected = Math::Vector((reflected / reflected.length()) + (Math::Vector<3>::random_unit_vector() * fuzz));
            scattered = Ray(rec.intersection, reflected);
            attenuation = albedo;
            return (scattered.direction.dot(rec.normal) > 0);
        }

    private:
        Color albedo;
        double fuzz;
    };

    class Dielectric : public Material {
    public:
        Dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered)
        const override {
            attenuation = Color(1.0, 1.0, 1.0);
            double ri = rec.frontFace ? (1.0 / refraction_index) : refraction_index;

            //Math::Vector<N,T> unit_direction = unit_vector(r_in.direction);
            Math::Vector<3> unit_direction = Math::Vector<3>(r_in.direction / r_in.direction.length());

            double cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            Math::Vector<3> direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double())
                direction = Math::Vector<3>::reflect(unit_direction, rec.normal);
            else
                direction = Math::Vector<3>::refract(unit_direction, rec.normal, ri);

            scattered = {rec.intersection, direction};

            return true;
        }

    private:
        // Refractive index in vacuum or air, or the ratio of the material's refractive index over
        // the refractive index of the enclosing media
        double refraction_index;

        static double reflectance(double cosine, double refraction_index) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
    };
}

#endif
