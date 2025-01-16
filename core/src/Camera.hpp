/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Rectangle.hpp"
#include "Ray.hpp"
#include "MathUtils.hpp"
#include "Color.hpp"
#include "Transformation.hpp"

namespace RayTracer {
    class Camera {
    public:
        Math::Point<3> cameraPosition;
        RayTracer::Rectangle imagePlane;
        double focalLength;

        Math::Vector<3> cameraDirection;
        Math::Vector<3> cameraUpVector;
        Transformation transformation;
        double aspectRatio;
        int imageWidth;
        int imageHeight;
        double verticalFov;

        // Default values
        explicit Camera(double aspectRatio = 16.0 / 9.0, int imageWidth = 600, double verticalFov = 90,
               const Math::Point<3>& cameraPosition = Math::Point<3>({-2, 2, 1}), const Math::Point<3>& lookAtPoint = Math::Point<3>({0, 0, -1}),
               const Math::Vector<3>& cameraUpVector = Math::Vector<3>({0, 1, 0}))
                : aspectRatio(aspectRatio), imageWidth(imageWidth), verticalFov(verticalFov),
                  cameraPosition(cameraPosition), cameraUpVector(cameraUpVector),
                  focalLength(Math::Vector(cameraPosition - lookAtPoint).length()),
                  cameraDirection(Math::Vector(cameraPosition - lookAtPoint).normalize()) {

            imageHeight = static_cast<int>(imageWidth / aspectRatio);
            updateImagePlane();
        }

        // Setters
        void setAspectRatio(double newAspectRatio) {
            aspectRatio = newAspectRatio;
            imageHeight = static_cast<int>(imageWidth / aspectRatio);
            updateImagePlane();
        }

        void setImageWidth(int newImageWidth) {
            imageWidth = newImageWidth;
            imageHeight = static_cast<int>(imageWidth / aspectRatio);
            updateImagePlane();
        }

        void setVerticalFov(double newVerticalFov) {
            verticalFov = newVerticalFov;
            updateImagePlane();
        }

        void setCameraPosition(const Math::Point<3>& newCameraPosition) {
            cameraPosition = newCameraPosition;
            updateImagePlane();
        }

        void setLookAtPoint(const Math::Point<3>& newLookAtPoint) {
            cameraDirection = Math::Vector(cameraPosition - newLookAtPoint).normalize();
            focalLength = Math::Vector(cameraPosition - newLookAtPoint).length();
            updateImagePlane();
        }

        void setCameraUpVector(const Math::Vector<3>& newCameraUpVector) {
            cameraUpVector = newCameraUpVector;
            updateImagePlane();
        }

        void translate(const Math::Vector<3>& translation) {
            Transformation oldTransformation = transformation;
            transformation.setTranslation(translation);
            applyTransformation();
            transformation = oldTransformation;
        }

        void rotate(double angle, const Math::Vector<3>& axis) {
            Transformation oldTransformation = transformation;
            transformation.setRotation(angle, axis);
            applyTransformation();
            transformation = oldTransformation;
        }

        void zoom(double factor) {
            focalLength *= factor;
            updateImagePlane();
        }

        [[nodiscard]] Ray ray(double u, double v) const {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-0.5, 0.5);

            auto x = dis(gen);
            auto y = dis(gen);

            Math::Point<3> screenPoint = imagePlane.pointAt(u + x, v + y);
            auto direction = Math::Vector<3>(screenPoint - cameraPosition);

            return {cameraPosition, direction};
        }

    private:
        void updateImagePlane() {
            double theta = Math::Utils::degrees_to_radians(verticalFov);
            double h = tan(theta / 2);
            double viewport_height = 2 * h * focalLength;
            double viewport_width = viewport_height * aspectRatio;

            Math::Vector<3> w = cameraDirection.normalize();
            Math::Vector<3> u = Math::Vector<3>::cross(cameraUpVector, w).normalize();
            Math::Vector<3> v = Math::Vector<3>::cross(w, u);

            Math::Vector<3> viewport_u = u * viewport_width;
            Math::Vector<3> viewport_v = -v * viewport_height;

            auto pixel_delta_u = viewport_u / imageWidth;
            auto pixel_delta_v = viewport_v / (imageWidth / aspectRatio);

            auto viewport_upper_left = cameraPosition - (w * focalLength) - viewport_u / 2 - viewport_v / 2;
            auto pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

            imagePlane = RayTracer::Rectangle(Math::Point<3>(pixel00_loc), Math::Vector<3>(pixel_delta_u), Math::Vector<3>(pixel_delta_v));
        }

        void applyTransformation() {
            cameraPosition = transformation.transformPoint(cameraPosition);
            cameraDirection = transformation.transformVector(cameraDirection).normalize();
            cameraUpVector = transformation.transformVector(cameraUpVector).normalize();
            updateImagePlane();
        }
    };
}

#endif
