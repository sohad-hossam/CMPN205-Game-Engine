
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include "light.hpp"
#include<iostream>

namespace our {
    //getting light data from a json file
    void LightComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object())
            return;
       
        // setting data of light
        std::string typeName = data.value("light_type", typeName);
        if (typeName == "DIRECTIONAL")
        {
             type = LightType::DIRECTIONAL;
             std::cout << "here" << std::endl;
        }  
        else if (typeName == "SPOT")
        {
            type = LightType::SPOT;
            type = LightType::POINT;
            spotAngle.inner = data.value("inner", glm::radians(15.0f));
            spotAngle.outer = data.value("outer",  glm::radians(30.0f));
            //getting attenuation:
            attenuation.constant = data.value("constant", 0.0f);
            attenuation.linear = data.value("linear",0.0f);
            attenuation.quadratic = data.value("quadratic", 1.0f);
        }
        else
        {
            //getting attenuation:
            attenuation.constant = data.value("constant", 0.0f);
            attenuation.linear = data.value("linear",0.0f);
            attenuation.quadratic = data.value("quadratic", 1.0f);
        }
        //getting color data -->initially white
        color = data.value<glm::vec3>("color", color); 
        diffuse = glm::vec3(data.value("diffuse", glm::vec3(0.5f, 0.0f, 0.5f)));
        specular = glm::vec3(data.value("specular", glm::vec3(0.0f, 0.0f, 0.0f)));
        ambient = glm::vec3(data.value("ambient", glm::vec3(0.5f, 0.5f, 0.5f)));
    }
}
