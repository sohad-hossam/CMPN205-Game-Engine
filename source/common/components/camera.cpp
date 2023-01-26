#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt

        //eye is origin of camera [where camera is] so we trasnfomation by M * 0,0,0
        glm::vec3 eye = M * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);        //(0,0,0)
        //center is where the camera is looking at so we trasnfomation by M * 0,0,-1  "as z is -1"
        glm::vec3 center = M * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);    //(0,0,-1)
        //center is where the camera is looking at so we trasnfomation 
        glm::vec3 up = M * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);         //(0,1,0)
       
        glm::mat4 view_matrix = glm::lookAt(eye, center, up);

        return view_matrix;
    }

    //we now transformed from local to world then from world to camera
    //now we want to convert to canoncial space "which is the 3d to 2d but also 3d because we need z buffer depth"
    // we do that by either prespective or orthog projections

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective

        //aspect ratio:  x/y
        float aspect_ratio = (float)viewportSize.x / viewportSize.y;
        glm::mat4 projection_matrix;
        //get camera type
        if (cameraType == CameraType::ORTHOGRAPHIC)
        {
            //orthog projection: [all same size]
            //it takes left, right, bottom, top, near and far where The camera can’t see anything nearer than “near” and farther than “far”

            //the bottom is  -orthoHeight/2
            float bottom = -orthoHeight / 2;
            //top is  orthoHeight/2
            float top = orthoHeight / 2;

            //the left same as bottom but we multiply by ascpect ratio
            float left= (-orthoHeight / 2)*aspect_ratio ;
            //the left same as top but we multiply by ascpect ratio
            float right= (orthoHeight / 2) *aspect_ratio ;
        
            //the matrix:
           projection_matrix = glm::ortho(left, right, bottom, top);
        }
        else if(cameraType == CameraType::PERSPECTIVE)
        {
            //for presepective projection:    //things are lesser in size [far is smaller and near is bigger]
            //the matrix:
            projection_matrix = glm::perspective(fovY, aspect_ratio, near, far);
        }

        return projection_matrix;
    }
}