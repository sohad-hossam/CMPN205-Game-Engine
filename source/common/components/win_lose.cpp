#include "win_lose.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    //for win_lose we need to know the 
                                       //=>score [+ 1 if ate a fish  -1 if hit a bush] 
                                       //=>lives [its 0 only and if score became -ve then lives-- then lose [lives <0], if score +ve and hits rat==win [lives++ >0]]
    // Reads score and lives from the given json object

    void win_lose_Component::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        //getting score:
        score=data.value("score", score);
        //getting life
        life=data.value("live", life); //i changed smth
    }
}