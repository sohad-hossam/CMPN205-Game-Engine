#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        pipelineState.setup(); //setting up pipeline state 
        shader->use(); //set shader to be used
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
   
    // An example where this material can be used is when the whole object has only color which defined by tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        
        //set up of its parent where parent is the Material class so:
        Material::setup();

        //setting tint uniform to the value in the member variable tint for the shader (a color that will be sent to the shader)
        shader->set("tint",tint);  //it takes string and vec4
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        
        //setup of parent which is TintedMaterial 
        TintedMaterial::setup();

        //setting the alphaThreshold uniform to the value in member variable alpha Threshold
        shader->set("alphaThreshold",alphaThreshold);

        //bind the texture and sampler to a texture unit
        //bind for texture and sampler
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        sampler->bind(0);
       
        //send the unit number to the uniform variable tex "0 ->index of texture unit used"
        //set for shader
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}