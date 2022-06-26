#include "window.h"

#include <iostream>
#include <string>

namespace engine {
    struct PlatformWindowData {
        PlatformWindowData(std::string const& name) 
        : m_platform_name{name} 
        {}

        std::string m_platform_name;
    };
}