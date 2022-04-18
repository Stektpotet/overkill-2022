#pragma once
#include <glm/glm.hpp>
#include "BaseComponents.hpp"
#include <overkill/data/Light.hpp>

namespace OK 
{
    struct LightSource : public Component
    {
        enum class Type { DIRECTIONAL, POINT, SPOT };
        
        Type light_type;
        SpotLight light; // SPOT LIGHT HOLDS ALL THE DATA USED FOR ANY OF THE LIGHT SOURCES

        // Register as light source AND eventual sub-types, only register as LightSource ONCE if the component is in fact a LightSource, and not a subtype
        inline virtual const std::vector<std::type_index> component_type_signatures() const override
        {
            auto base = std::type_index(typeid(LightSource));
            auto sub = std::type_index(typeid(*this));
            if (sub == base) {
                return { base };
            }
            return { sub, base };
        }

    protected:
        inline LightSource() : Component() {};
    };
}
