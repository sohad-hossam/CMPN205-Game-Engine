#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
 /////////////////////////////////////
// MY PART

glm::mat4 sca = glm::mat4(1.0f);
glm::mat4 rot = glm::mat4(1.0f);
glm::mat4 trans = glm::mat4(1.0f);

 sca = glm::scale(sca,scale); 
 rot = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z); //yaw y , pitch x , roll z
 trans = glm::translate(trans,position);


glm::mat4 final(1.0f);

final =trans*rot*sca ; //multiplying in the order of transformation


 ///////////////////////////////

        return final; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}