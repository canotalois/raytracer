/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "SFMLGuiPlugin.hpp"

extern "C" IPlugin *create_plugin() {
    static SFMLGuiPlugin instance;

    return &instance;
}

void SFMLGuiPlugin::createWindow(int width, int height, const char *title) {
    std::lock_guard<std::mutex> lock(windowMutex);
    window.create(sf::VideoMode(width, height), title);
    texture.create(width, height);
    sprite.setTexture(texture);
}

void SFMLGuiPlugin::drawPixels(const uint8_t *pixels, unsigned int width, unsigned int height, unsigned int x,
                               unsigned int y) {
    std::lock_guard<std::mutex> lock(windowMutex);
    texture.update(pixels, width, height, x, y);
}

void SFMLGuiPlugin::display() {
    std::lock_guard<std::mutex> lock(windowMutex);
    window.clear();
    window.draw(sprite);
    window.display();
}

bool SFMLGuiPlugin::windowIsOpen() {
    std::lock_guard<std::mutex> lock(windowMutex);
    return window.isOpen();
}

#include <iostream>

void SFMLGuiPlugin::SFMLGuiPlugin::handleEvents() {
    std::lock_guard<std::mutex> lock(windowMutex);
    sf::Event sfEvent{};
    while (window.pollEvent(sfEvent)) {
        std::unique_ptr<IEvent> event = nullptr;
        switch (sfEvent.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                {
                    KeyType mappedKey = mapKey(sfEvent.key.code);
                    event = std::make_unique<KeyEvent>(mappedKey, KeyAction::Pressed);
                }
                break;
            case sf::Event::KeyReleased:
                {
                    KeyType mappedKey = mapKey(sfEvent.key.code);
                    event = std::make_unique<KeyEvent>(mappedKey, KeyAction::Released);
                }
                break;
            case sf::Event::MouseMoved:
                event = std::make_unique<MouseEvent>(MouseAction::Moved, sfEvent.mouseMove.x, sfEvent.mouseMove.y);
                break;
            case sf::Event::MouseButtonPressed:
                event = std::make_unique<MouseEvent>(MouseAction::ButtonPressed, sfEvent.mouseButton.x, sfEvent.mouseButton.y, sfEvent.mouseButton.button);
                break;
            case sf::Event::MouseButtonReleased:
                event = std::make_unique<MouseEvent>(MouseAction::ButtonReleased, sfEvent.mouseButton.x, sfEvent.mouseButton.y, sfEvent.mouseButton.button);
                break;
            default:
                break;
        }
        if (event) {
            auto callbacks = eventCallbacks.find(event->getType());
            if (callbacks != eventCallbacks.end()) {
                for (auto& callback_pair : callbacks->second) {
                    callback_pair.first(event.get(), callback_pair.second);
                }
            } else {
            }
        }
    }
}