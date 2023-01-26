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
        if(texture != NULL)
        {
            texture->bind();
        }
        else{
            Texture2D::unbind();
        }
        if(sampler != NULL)
        {
            sampler->bind(0);
        }
        else
        {
            sampler->unbind(0);
        }
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


    //===================================================New Partt=======================================
    //we also want to set up the light material and deserialize it:
     void LightMaterial::setup() const 
     {
        //setup of parent 
        Material::setup();

        //we want to set shader uniforms [which are ], bind textures [all what we jave] and bind sampler 
        //bind the texture and sampler to a texture unit
        //bind for texture and sampler

        //we now have many textures where each texture maps to one gl_texture unit starting from 0 and we have samplers than can take multiple texture

        //first we generate 1 texture onn the texture name we want where &num reference the text name
        //then   // we want to bind all textures, samplers and set the uniforms in the shader [textured material]

        glActiveTexture(GL_TEXTURE0);
        if(albedo)
        {
            albedo->bind();
        }
        else
        {
            Texture2D::unbind();
        }
        if (sampler)
        {
            sampler->bind(0);
        }
        else
        {
            sampler->unbind(0);
        }
        shader->set("tex_mat.albedo_map", 0);//take care of the naming
        
       // ===========================

        //glActiveTexture(0 + txt_num);
        //glGenTextures(1, &txt_num);
        glActiveTexture(GL_TEXTURE1);
        if(specular)
        {
            specular->bind();   
        }
        else
        {
            Texture2D::unbind();
        }
        shader->set("tex_mat.specular_map", 1);
        if(sampler)
        {
            sampler->bind(1);
        }
        else
        {
            sampler->unbind(1);
        }

                 // ===========================
        glActiveTexture(GL_TEXTURE2);
        if(roughness)
        {
            roughness->bind();
        }
        else
        {
            Texture2D::unbind();
        }
        shader->set("tex_mat.roughness_map", 2);
        if(sampler)
        {
            sampler->bind(2);
        }
        else
        {
            sampler->unbind(2);
        }

        glActiveTexture(GL_TEXTURE3);
        if(ambient_occlusion)
        {
            ambient_occlusion->bind();
            
        }
        else
        {
            Texture2D::unbind();
        }
        shader->set("tex_mat.ambient_occlusion_map", 3);
        if(sampler)
        {
            sampler->bind(3);
        }
        else
        {
            sampler->unbind(3);
        }

         // ===========================
        glActiveTexture(GL_TEXTURE4);
        if(emission)
        {
            emission->bind();      
        }
        else
        {
            Texture2D::unbind();
        }
        shader->set("tex_mat.emission_map", 4);
        if(sampler)
        {
            sampler->bind(4);
        }
        else
        {
            sampler->unbind(4);
        }
    }

    // This function read the material data from a json object
    void LightMaterial::deserialize(const nlohmann::json& data)
    {
        Material::deserialize(data);

        if(!data.is_object()) return;

        //albedo, specular, rough, ambient_occlusion and emission for textures
        //sampler for samler
        //like their names in frag shader: albedo_map, specular_map, ambient_occlusion_map, roughness_map, emissive_map
        
        albedo= AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular= AssetLoader<Texture2D>::get(data.value("specular", ""));
        ambient_occlusion= AssetLoader<Texture2D>::get(data.value("ambient-occlusion", ""));
        roughness= AssetLoader<Texture2D>::get(data.value("roughness", ""));
        emission= AssetLoader<Texture2D>::get(data.value("emission", ""));

        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }


}