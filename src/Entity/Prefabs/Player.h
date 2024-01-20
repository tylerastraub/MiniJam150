#pragma once

#include "SpritesheetPropertiesComponent.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class Player {
    public:
        Player() = default;
        ~Player() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(Spritesheet* spritesheet);

        static const int MS_BETWEEN_IDLE_FRAMES = 500;
        static const int NUM_OF_IDLE_FRAMES = 2;
        static const int MS_BETWEEN_MOVING_FRAMES = 150;
        static const int NUM_OF_MOVING_FRAMES = 4;

    };
}