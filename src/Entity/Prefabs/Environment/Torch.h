#pragma once

#include "SpritesheetPropertiesComponent.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class Torch {
    public:
        Torch() = default;
        ~Torch() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(Spritesheet* spritesheet);

    };
}