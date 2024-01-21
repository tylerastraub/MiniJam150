#pragma once

#include "SpritesheetPropertiesComponent.h"
#include "MineralComponent.h"
#include "ItemType.h"
#include "Hue.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class Mineral {
    public:
        Mineral() = default;
        ~Mineral() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos, ItemType mineralType);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(Spritesheet* spritesheet);
        static float getMineralMineSpeed(ItemType mineralType);
        static Hue getMineralHue(ItemType mineralType);

    };
}