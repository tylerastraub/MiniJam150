#pragma once

#include "SpritesheetPropertiesComponent.h"
#include "ItemType.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class ItemPickup {
    public:
        ItemPickup() = default;
        ~ItemPickup() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos, ItemType itemType);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(ItemType itemType);

    };
}