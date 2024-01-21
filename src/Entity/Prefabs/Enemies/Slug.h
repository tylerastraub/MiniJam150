#pragma once

#include "SpritesheetPropertiesComponent.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class Slug {
    public:
        Slug() = default;
        ~Slug() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(Spritesheet* spritesheet);

        static const int MS_BETWEEN_IDLE_FRAMES = 250;
        static const int NUM_OF_IDLE_FRAMES = 6;

    };
}