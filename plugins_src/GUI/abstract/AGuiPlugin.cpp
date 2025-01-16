/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "AGuiPlugin.hpp"

#include <iostream>

void AGuiPlugin::registerCallback(EventType type, void(*callback)(IEvent*, void*), void* context) {
    eventCallbacks[type].emplace_back(callback, context);
}


AGuiPlugin::~AGuiPlugin() = default;
