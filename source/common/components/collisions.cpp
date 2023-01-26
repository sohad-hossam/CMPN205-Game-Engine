#include "collisions.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    //for collision to occur we need to know the radius and center of the collided obj
    // Reads radius and center of the collision from the given json object
    void CollisionComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        //getting radius and center:
        center=data.value("center", center);
        radius=data.value("radius", radius);
        
    }
    
}