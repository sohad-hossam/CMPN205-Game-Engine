#pragma once

#include "../ecs/world.hpp"
#include "../components/collisions.hpp"
#include "../components/win_lose.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{
    
    //logic:
    /*
    Each entity contains its components, from them [collision component] and [winstate component]
    the only entity we care about here is a the cat's health entity [a parent to the cat/ or all entities] 

    get the cat_health entity 

    the needed entity [cat] has 
                             parent: cat_healh
    cat_health entity 
                             components: win_lose component, (camera??/not sure and freecamera controller??? not sure)


    we will loop on all entities [we are looking for [cat]
    cat has a parent which is cat_health
    so check if the entity has a parent 
    if yes=>  
            get the needed entity then break =>the needed entity is if it has a win_lose component & [not sure= camera and freecam]
    
    now that we have the gamestate entity whcih is also [cat_health]
    we want to get from it the life and score (or life only)

    Note that what decides the score and the life is the collision [increment and decrement them]
      --when cat hits rat life++                             -win
      --when cat hit fish score++  
      --when cat hit bush score--  if reached<0 then life--  -lose

      what matters here only is to know if he won or lost [from the life]

    */




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   //The main role of this class is to return loss or win [and i think it should update the world????]

    class Win_Lose_System {
    public:
        void update(World* world, float deltaTime) {  }

        //because i need to return the game status
        int update(World* world)
        {
            //defining the needed entities:
            Entity* game_state;    //has the score and life of a cat 
           

            //defining the needed components:
            win_lose_Component* winLose= nullptr;
            CameraComponent* camera = nullptr;
            //FreeCameraControllerComponent *controller = nullptr;

            // For each entity in the world 
            for(auto entity : world->getEntities())
            {
                if (entity->parent) //not a nullptr "has a parent" [cat has  a parent]
                {
                    winLose = entity->getComponent<win_lose_Component>();
                    camera = entity->getComponent<CameraComponent>();
                    //controller = entity->getComponent<FreeCameraControllerComponent>();
                   // if(camera && controller && winLose) break; //found our entity
                   if(camera && winLose) break; //found our entity
                }   
            }
             // If there is no entity with all a CameraComponent and a FreeCameraControllerComponent and a winLose, we can do nothing so we return
            //if(!(camera && controller && winLose)) return;
            if(!(camera && winLose)) return 0;

             // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            game_state = camera->getOwner();

            //setting life and score:
            int life = game_state->getComponent<win_lose_Component>()->life;
           // int score = game_state->getComponent<win_lose_Component>()->score; idk if we will need score

            //if life <0 we should end game lose else win
            if (life>0)
            {
                return 1;      //1 for loss
            }
            else if(life>0)
            {
                return 2;      //2 for win
            }
            return 0;
        }
        //idk what to do when win or lose?------------------------------------------------------------------
        //check play-state.hpp-----------------------------------------------------------------------------
    };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
