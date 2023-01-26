#pragma once

#include "../ecs/component.hpp"
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

//the light component should save these data =>type, color [diffuse, specular, ambient], position, direction , 
//linar, constant, quadrtic[attenuation] , inner, outer [spot angle]

namespace our {

    //defining types of light
    enum class LightType 
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component {
    public:

        LightType type;
       
        glm::vec3 color={1, 1, 1};  //color is initially white [we combined diffusion with ambient with specular]

        // Attenuation==>Used for Point and Spot Lights only 
        struct
        {
            float constant, linear, quadratic;
        } attenuation; 

        // Spot angle==> Used for Spot Lights only
        struct
        {
            float inner, outer;
        } spotAngle; 

        glm::vec3 diffuse;
        glm::vec3 specular;
         glm::vec3 ambient;
        // The ID of this component type is "light"
        static std::string getID() { return "Light"; }

        // Reads  from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}