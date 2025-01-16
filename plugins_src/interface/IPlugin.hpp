/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef IPLUGIN_HPP
#define IPLUGIN_HPP

class IPlugin {
public:
    virtual void initialize() = 0;
    virtual void execute() = 0;
    virtual ~IPlugin() = default;
};

#endif
