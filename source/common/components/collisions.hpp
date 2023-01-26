#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    // This component denotes that the CollisionSystem will collide the owning entity at certain center and radius
   
    class CollisionComponent : public Component {  //inhereted from class component
    public:
        //center is at (0.5,0.5.0.5)
        glm::vec3 center = glm::vec3(0.5f,0.5f,0.5f);
        float radius = 1.0f;
        //float width = 1.0f;
        //float height = 1.0f;
        //std::string type;
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }
        
        // Reads the radius and center from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}