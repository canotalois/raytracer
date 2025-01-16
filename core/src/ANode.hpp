/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef ANODE_HPP
#define ANODE_HPP

#include "INode.hpp"

namespace RayTracer {
    class ANode: public INode {
    public:
        ~ANode() override = 0;

        void setTransformation(const Transformation& t) override {
            transform = t;
        }

        [[nodiscard]] const Transformation& getTransformation() const override {
            return transform;
        }

        Transformation transform;
    };
}

#endif
