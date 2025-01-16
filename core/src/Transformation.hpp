/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef TRANSFORMATION_HPP
#define TRANSFORMATION_HPP

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Point.hpp"

namespace RayTracer {
    class Transformation {
    public:
        Math::Matrix<4, 4> matrix;
        Math::Matrix<4, 4> inverse;

        Transformation() : matrix(Math::Matrix<4, 4>::Identity()), inverse(Math::Matrix<4, 4>::Identity()) {}

        void setTranslation(const Math::Vector<3>& translation) {
            Math::Matrix<4, 4> trans = Math::Matrix<4,4>::Identity();
            trans.at(0,3) = translation.at(0,0);
            trans.at(1,3) = translation.at(1,0);
            trans.at(2,3) = translation.at(2,0);
            applyMatrix(trans);
        }

        void setRotation(double angle, const Math::Vector<3>& axis) {
            Math::Vector<3> n = axis.normalize();
            double cosTheta = std::cos(angle);
            double sinTheta = std::sin(angle);
            double oneMinusCosTheta = 1 - cosTheta;

            Math::Matrix<3, 3> rot;
            rot.at(0, 0) = cosTheta + n.at(0, 0) * n.at(0, 0) * oneMinusCosTheta;
            rot.at(0, 1) = n.at(0, 0) * n.at(1, 0) * oneMinusCosTheta - n.at(2, 0) * sinTheta;
            rot.at(0, 2) = n.at(0, 0) * n.at(2, 0) * oneMinusCosTheta + n.at(1, 0) * sinTheta;
            rot.at(1, 0) = n.at(1, 0) * n.at(0, 0) * oneMinusCosTheta + n.at(2, 0) * sinTheta;
            rot.at(1, 1) = cosTheta + n.at(1, 0) * n.at(1, 0) * oneMinusCosTheta;
            rot.at(1, 2) = n.at(1, 0) * n.at(2, 0) * oneMinusCosTheta - n.at(0, 0) * sinTheta;
            rot.at(2, 0) = n.at(2, 0) * n.at(0, 0) * oneMinusCosTheta - n.at(1, 0) * sinTheta;
            rot.at(2, 1) = n.at(2, 0) * n.at(1, 0) * oneMinusCosTheta + n.at(0, 0) * sinTheta;
            rot.at(2, 2) = cosTheta + n.at(2, 0) * n.at(2, 0) * oneMinusCosTheta;

            Math::Matrix<4, 4> rot4 = Math::Matrix<4, 4>::Identity();
            rot4.setBlock(0, 0, rot);

            applyMatrix(rot4);
        }

        void setScale(const Math::Vector<3>& scale) {
            Math::Matrix<4, 4> scl = Math::Matrix<4, 4>::Identity();
            scl.at(0,0) = scale.at(0,0);
            scl.at(1,1) = scale.at(1,0);
            scl.at(2,2) = scale.at(2,0);
            applyMatrix(scl);
        }

        void setShear(double s_xy, double s_xz, double s_yx, double s_yz, double s_zx, double s_zy) {
            Math::Matrix<4, 4> shear = Math::Matrix<4, 4>::Identity();
            shear.at(0, 1) = s_xy;
            shear.at(0, 2) = s_xz;
            shear.at(1, 0) = s_yx;
            shear.at(1, 2) = s_yz;
            shear.at(2, 0) = s_zx;
            shear.at(2, 1) = s_zy;
            applyMatrix(shear);
        }

        void applyMatrix(const Math::Matrix<4,4>& mat) {
            matrix = mat * matrix;
            inverse = matrix.inverse();
        }

        [[nodiscard]] Math::Point<3> transformPoint(const Math::Point<3>& point) const {
            Math::Matrix<4, 1> pt({point.at(0, 0), point.at(1, 0), point.at(2, 0), 1.0});
            auto transformedPt = (matrix * pt);
            return Math::Point<3>({transformedPt.at(0, 0), transformedPt.at(1, 0), transformedPt.at(2, 0)});
        }

        [[nodiscard]] Math::Vector<3> transformVector(const Math::Vector<3>& vector) const {
            Math::Matrix<4, 1> vec({vector.at(0, 0), vector.at(1, 0), vector.at(2, 0), 0.0});
            auto transformedVec = (matrix * vec);
            return Math::Vector<3>({transformedVec.at(0, 0), transformedVec.at(1, 0), transformedVec.at(2, 0)});
        }
    };
}

#endif
