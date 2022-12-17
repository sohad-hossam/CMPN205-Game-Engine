#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

namespace our {

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //TODO: (Req 10) Pick the correct pipeline state to draw the sky
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            //in the skybox the camera is placed inside of a box so we want to see their front, rather than their back
            skyPipelineState.faceCulling.enabled=true; 
            skyPipelineState.faceCulling.culledFace = GL_FRONT;

            skyPipelineState.depthTesting.enabled = true; 
            //This allows the skybox to be render behind any other object in the scene.
            skyPipelineState.depthTesting.function = GL_LEQUAL;   

            //To render images with different levels of transparency 
            skyPipelineState.blending.enabled = true;
            skyPipelineState.blending.equation = GL_FUNC_ADD;
            skyPipelineState.blending.sourceFactor = GL_SRC_ALPHA;
            skyPipelineState.blending.destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
        
            skyPipelineState.colorMask = {true, true, true, true}; 
            skyPipelineState.depthMask = false; //enable writing to the depth bufffer
            //to render the sky behind the objects
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            //TODO: (Req 11) Create a framebuffer
            glGenFramebuffers(1, &postprocessFrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);   

            //TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits)
            GLenum mip_levels = (GLenum)glm::floor(glm::log2(glm::max<float>((float)windowSize.x, (float)windowSize.y))) + 1;
            colorTarget = texture_utils::empty(mip_levels,windowSize);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            colorTarget->unbind();
            //Create and Attach a Depth Target Texture
            depthTarget = new Texture2D();
            depthTarget->bind();
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, windowSize.x, windowSize.y); //DEPTH  ONLY  need 1 mip level
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);
            depthTarget->unbind();
            //TODO: (Req 11) Unbind the framebuffer just to be safe
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler* postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram* postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        //the camera component view matrix:
        auto owner =camera->getOwner();
        auto M = owner->getLocalToWorldMatrix();
        glm::vec3 cen = M*glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);    //(0,0,-1)
        glm::vec3 cameraForward = cen;  

        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand& first, const RenderCommand& second){
            //TODO: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 
            //we should sort so that first then second is sorted  and we use dot
            return (glm::dot(first.center , cameraForward) < glm::dot(second.center , cameraForward)); 
        });

        //getViewMatrix is used to transform vertices from world space to camera space
        //getProjectionMatrix is used for projecting
        //TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        glm::ivec2 viewportSize= glm::ivec2 (this->windowSize.x,this->windowSize.y); 
        glm::mat4 VP = camera->getProjectionMatrix(viewportSize) * camera->getViewMatrix();
        
        //TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        glViewport(0, 0, viewportSize.x, viewportSize.y);

        //TODO: (Req 9) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepthf(1.0f);
        
        //TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        //color mask to true
        glColorMask(true, true, true, true);
        //depth mask to true
        glDepthMask(true);

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //TODO: (Req 11) bind the framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, postprocessFrameBuffer);
        }

        //TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        //TODO: (Req 9) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
         for (auto opaque_comp : opaqueCommands)  //looping around the opaque commands 
        {
            //to draw opaque comp we need to set up material
            opaque_comp.material->setup(); 
            //then we need to  transform the component to the camera view then afterwards it is projected  
            opaque_comp.material->shader->set("transform", VP * opaque_comp.localToWorld);
            opaque_comp.mesh->draw();  
        }

        
        // If there is a sky material, draw the sky
        if(this->skyMaterial){
            //TODO: (Req 10) setup the sky material 
            skyMaterial->setup();
            //TODO: (Req 10) Get the camera position 
            glm::vec3 eye = M * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            //TODO: (Req 10) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
             glm::mat4 translationMat = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                eye.x, eye.y, eye.z, 1.0f
            );// translating to eye position then we it is converted from the world space to camera space and then it is projectd 
             glm::mat4 modelMat = camera->getProjectionMatrix(viewportSize)*camera->getViewMatrix()*translationMat;

             // TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
             //  We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
             glm::mat4 alwaysBehindTransform = glm::mat4(
                 1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 1.0f);
             //opengl uses column-major matrices therefore by multiplying this matrix the z-coordinate in the position
             //becomes equal to 1 which will make it drawn behind every object
             // TODO: (Req 10) set the "transform" uniform
             skyMaterial->shader->set("transform", alwaysBehindTransform*modelMat);

             // TODO: (Req 10) draw the sky sphere 
             skySphere->draw();
    
        
        }

        //TODO: (Req 9) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (auto transparent_comp : transparentCommands)
        {
            //same as drawing the opaque material 
            transparent_comp.material->setup();
            transparent_comp.material->shader->set("transform",VP * transparent_comp.localToWorld);
            transparent_comp.mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //TODO: (Req 11) Return to the default framebuffer
            //To switch back to the default framebuffer visible on your screen, simply pass 0
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            //TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            glBindVertexArray(postProcessVertexArray);
            glDisable(GL_DEPTH_TEST);//Depth test is disabled since we dont want to compare it with any of the entities
            //color buffer is binded in order to make it the texture of the spanning triangle
            colorTarget->bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

}