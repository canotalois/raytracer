/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef AGUIPLUGIN_HPP
#define AGUIPLUGIN_HPP

#include <map>
#include <cstdlib>
#include <mutex>
#include <thread>

#include "IGuiPlugin.hpp"

class AGuiPlugin : public IGuiPlugin {
protected:
    std::unordered_map<EventType, std::vector<std::pair<void(*)(IEvent*, void*), void*>>> eventCallbacks;

public:
    void registerCallback(EventType type, void(*callback)(IEvent*, void*), void* context) override;
    ~AGuiPlugin() override = 0;
};

#endif
