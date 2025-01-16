/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef EVENT_CALLBACK_WRAPPER_HPP
#define EVENT_CALLBACK_WRAPPER_HPP

#include "Camera.hpp"
#include <unordered_map>
#include <vector>
#include <iostream> // Pour les messages de débogage

#include "../../plugins_src/GUI/interface/IEvent.hpp"


namespace RayTracer {
    class EventCallbackWrapper {
    public:
        EventCallbackWrapper(Camera &cam, Renderer &ren) : camera(cam), renderer(ren) {}

        static void KeyEventCallback(IEvent *event, void *context) {
            auto *self = static_cast<EventCallbackWrapper *>(context);
            self->handleKeyEvent(dynamic_cast<KeyEvent *>(event));
        }

        void handleKeyEvent(KeyEvent* keyEvent) {
            if (!keyEvent) {
                return;
            }

            const double translationStep = 0.1;
            const double rotationStep = 0.1;

            switch (keyEvent->getAction()) {
                case KeyAction::Pressed:
                    switch (keyEvent->getKey()) {
                        case KeyType::Z: {
                            Math::Vector<3> forward = {0, 0, -1};
                            camera.translate(forward.normalize() * translationStep);
                            break;
                        }
                        case KeyType::S: {
                            Math::Vector<3> backward = {0, 0, 1};
                            camera.translate(backward.normalize() * translationStep);
                            break;
                        }
                        case KeyType::Q: {
                            Math::Vector<3> left = {-1, 0, 0};
                            camera.translate(left.normalize() * translationStep);
                            break;
                        }
                        case KeyType::D: {
                            Math::Vector<3> right = {1, 0, 0};
                            camera.translate(right.normalize() * translationStep);
                            break;
                        }
                        case KeyType::A: {
                            camera.rotate(rotationStep, {0, 1, 0});
                            break;
                        }
                        case KeyType::E: {
                            camera.rotate(-rotationStep, {0, 1, 0});
                            break;
                        }
                        case KeyType::ESC:
                            // Handle ESC key if necessary
                            break;
                        default:
                            break;
                    }
                    renderer.interrupt(); // Interrompre le rendu en cours
                    break;
                case KeyAction::Released:
                    // Optionnel : gérez les actions à effectuer lorsque la touche est relâchée
                    break;
                default:
                    break;
            }
        }


    private:
        Camera &camera;
        Renderer &renderer;
    };
}

#endif
