#include "world.hpp"

namespace our {

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json& data, Entity* parent){
        if(!data.is_array()) return;
        for(const auto& entityData : data){
            Entity* entity=add(); //msh mot2kda mnhom 
            entity->parent=parent; 
            entity->deserialize(entityData);//msh 3araf HPASS el function pointer wla by refrence wla eh
            //TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            
            if(entityData.contains("children")){
                deserialize(entityData["children"],entity); //et2kdy mnha
        
                //TODO: (Req 8) Recursively call 'this' world's "deserialize" using the children data
                // and the current entity as the parent
                
            }
        }
    }

}