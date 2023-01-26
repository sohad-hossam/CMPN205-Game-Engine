#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    // This component denotes that the win_lose system will have a score and a live determining the win/lose state 
   
    class win_lose_Component : public Component {  //inhereted from class component
    public:
        //score initially 0:
        //life initially 0:
        int score=0;
        int life=0;

        // The ID of this component type is "win_lose"
        static std::string getID() { return "win_lose"; }

        // Reads the score and life from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}