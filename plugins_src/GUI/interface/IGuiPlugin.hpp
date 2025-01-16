/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef IGUIPLUGIN_HPP
#define IGUIPLUGIN_HPP

#include <functional>
#include <cstdint>

#include "../../interface/IPlugin.hpp"
#include "IEvent.hpp"

class IGuiPlugin : public IPlugin {
public:
    virtual void createWindow(int width, int height, const char *title) = 0;
    virtual void drawPixels(const uint8_t* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) = 0;
    virtual void display() = 0;
    virtual bool windowIsOpen() = 0;
    virtual void handleEvents() = 0;
    virtual void registerCallback(EventType type, void(*callback)(IEvent*, void*), void* context) = 0;
    ~IGuiPlugin() override = default;
};

#endif
