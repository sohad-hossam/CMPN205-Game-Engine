#pragma once

#include "../ecs/world.hpp"
#include "../components/collisions.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../components/win_lose.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The collision system is responsible for checking if a collision occured.
    //if a collision occured with a certain object[fish] then points+1
    //if collided with a certain object [rat] then game ends [wins]
    //if collided with bushes[maze] points-1
    //entities that would be updated when collision occur is the win state[score] and the disapperance of the other object [rat or fish] but not the bushes
 
    //logic:
    /*
    Each entity contains its components, from them [collision component] and [winstate component] ( and camera and controller????)
    collision component contains:
                             =>center and radius of the entity
                             =>getting ID to check its a collision component
                             =>reads the radius and center from json file from deserialize function and update radius and center
    so we should loop on the entities =>we care about[cat, fish, rat, bushes of maze]
    
    1.Entities having camera and controller: cat
        while loop on entities:
        check if they have a camera/controller componnet
        if entity is cat  => get its position =>break
    
    //get center and radius of the cat's collision component

    2.Entity having winstate: game_status and it has  aparent
        while loop on entities again:
        check if they have WINSTATE & has a parent    (a camera/controller componnet NOT SURE] 
        if entity is cat then our entity parent is game_state =>break

    3.Entities that have collision components are: fish, bush and rat
        while loop on entities again:
        check if they have a collision component and that they are not cat entity

   
         get=>[center and radius from collision component]

         now see the distance between the cat and the collided entity
         they will overlapp [collide] if distance <=2R [radius+radius]

                     .--------R cat         R entity it collided with---------.
                     ---------------------------------------------------------- distance
        distance is the distance between the center of both


        My question now is HOW TO KNOW IF ITS A FISH? A CAT? OR A BUSH? MAYBE BY ENTITY->NAME???????????
        BUT WHERE WILL WE DEFINE THE NAME??????????????????????????????????????????????????????????????

        suppose we will use name:
        if entity is RAT and they collided:
                                           => life++ 
                                           =>mark for removal->rat 
                                           =>should end game and clear all entities but idk where to put that code???????????????
        if entity is BUSH and they collide 
                                           => score--
                                           => nothing will be removed
                                           =>check if score<0 life--  and all entities are deleted by clea()????????????????
        if entity is fish and they collide 
                                           => score++
                                           => mark for removal->fish 

note:
    if the life<0 then game over =>lose => markForRemoval()  from the world function then => deleteMarkedEntities() on cat entity => then clear()  that destroys all entities
    if life>0 then game over =>win =>clear() [delete the world]

    */


 //////////////////// ///////////////////////////////////////////////////////////////////////////////////////////////
 //this class main role is to update world and delete speciefic entities when collided and update life and score of game_status entity
    class CollisionSystem {
    public:

       bool winner=false;
        bool collieded=false;
        int score=0;
        // This should be called every frame to update all entities containing a CollisionComponent. 
        void update(World* world, float deltaTime) {

            //defining the needed entities:
            Entity* cat= nullptr;
          
          
            //win_lose_Component* winLose= nullptr;

            // For each entity in the world -------------------(1)
            for(auto entity : world->getEntities())
            {
               
               //if it is an cat set our entity by cat 
                if (entity->name=="cat")
                {
                    cat = entity;
                    break;
                }
            }

     
            //  // Getting the cat position and radius from its collision component
             glm::vec3 cat_center = cat->getLocalToWorldMatrix() * glm::vec4(cat->getComponent<CollisionComponent>()->center, 1.0f);
             float cat_radius = cat->getComponent<CollisionComponent>()->radius;
            //std::cout<<cat->getComponent<CollisionComponent>()->radius<<std::endl;
            //std::cout << "cat center" << cat_center.x << std::endl;

            //for each entity in the world ----------------------(2)
            // for(auto entity : world->getEntities())
            // {
            //      if (entity->parent)
            //     {
            //         //winLose = entity->getComponent<win_lose_Component>();
            //         cam = entity->parent->getComponent<CameraComponent>();
            //         control = entity->parent->getComponent<FreeCameraControllerComponent>();
            //         if ((cam && control))
            //         {
            //             game_status = entity;
            //             break;
            //         }
            //     }
            // }


            //for each entity in the world ---------------------------(3)
            //note: here we will not need to break because we need all entities that have collision component
             for (auto entity : world->getEntities())
            {
                // Get the CollisionComponent component if it exists
                CollisionComponent *collision = entity->getComponent<CollisionComponent>();
                // If the CollisionComponent component exists
                if (collision && (entity != cat))
                {
                    //get center and radius of the collided entity from its collision components
                    glm::vec3 collided_entity_center = entity->getLocalToWorldMatrix() * glm::vec4(collision->center, 1.0f); 
                    float collided_entity_radius = collision->radius;

                    //get distance between 2 centers:
                    float distance = glm::distance(collided_entity_center, cat_center);

                    //now see if a collision occured
                    if (distance <= cat_radius + collided_entity_radius)
                    {
                       //if collided then check name of the entity
                       std::string entity_name=entity->name;

                       //if it is a fish then he wins 
                       if(entity_name =="fish")
                       {
                            world->markForRemoval(entity);
                            winner=true;
                       }
                       //if coin increase the score and delete it
                       else if(entity_name == "coin")
                       {
                         world->markForRemoval(entity);
                         score++;
                         collieded=true;
                       }
                    }
                }
                
            }
            //delete all entities that we marked
            world->deleteMarkedEntities();
        }

    };

}