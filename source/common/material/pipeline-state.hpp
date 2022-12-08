#pragma once
#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {

            glColorMask(colorMask.x,colorMask.y,colorMask.z,colorMask.w); //enable and disable writing of frame buffer color components
            glDepthMask(depthMask); //enable or disable writing into the depth buffer
           
    
            //TODO: (Req 4) Write this function
            //it spicfies the number of faces that should be rendred in order not to render things that the player won't see
            if (faceCulling.enabled==true)
            {
                //all the faces that are not front faces are discarded when enabling the cull face
                 glEnable(GL_CULL_FACE); 
                 //here we spicfy that we want to cull the back faces not front
                 glCullFace(faceCulling.culledFace);
                 //determine rather we want clockwise faces or counter clockwise faces to not be culled
                 glFrontFace(faceCulling.frontFace); 
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        //The depth-buffer is a buffer that stores information per fragment
        //it allows us to know which fragment is infront
            if (depthTesting.enabled==true)
            { 
                //when depth testing is enabled, OpenGL tests the depth value of a fragment against the content of the depth buffer. 
                //OpenGL performs a depth test and if this test passes, the fragment is rendered and the depth buffer 
                //is updated with the new depth value. If the depth test fails, the fragment is discarded.
                 glEnable(GL_DEPTH_TEST);
                 //OpenGL allows us to modify the comparison operators it uses for the depth test
                 //This allows us to control when OpenGL should pass or discard fragments and when to update the depth buffer
                 glDepthFunc(depthTesting.function); 
                 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
            // we use it to control the transperency
            if (blending.enabled==true)
            {
                glEnable(GL_BLEND); //To render images with different levels of transparency we have to enable blending  
                //set object blend color of the source and distantaion before using them in the blend function
                glBlendColor(blending.constantColor.x,blending.constantColor.y,blending.constantColor.z,blending.constantColor.w);
                //allows us to change the operator between the destination and source component before combaining them
                glBlendEquation(blending.equation);
                //combine the colors of two compoennts source and distantions 
                glBlendFunc(blending.sourceFactor,blending.destinationFactor); 
               
            }
            else
            {
                glDisable(GL_BLEND);
            }
        }
        
        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}