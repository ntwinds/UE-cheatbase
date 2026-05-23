#pragma once

#include "core/Types.hpp"
#include <string>

namespace cheatbase::runtime {

class IModule {
public:
    virtual ~IModule() = default;

    virtual const std::string& GetName() const = 0;
    
    virtual void OnLoad() = 0;
    virtual void OnUnload() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
};

} // namespace cheatbase::runtime
