#pragma once

#include "Text.h"
#include "ItemType.h"

#include <entt/entity/registry.hpp>
#include <memory>

class InventorySystem {
public:
    InventorySystem() = default;
    ~InventorySystem() = default;

    void update(entt::registry& ecs);
    
    void render(entt::registry& ecs, std::shared_ptr<Text> text);

private:
    int getYSpritesheetIndex(ItemType itemType);

    const float COBALT_BRIGHTNESS_BOOST = 0.02f;
    const float COBALT_FALLOFF_BOOST = -0.008f;
    const float TOPAZ_BRIGHTNESS_BOOST = 0.1f;
    const float TOPAZ_FALLOFF_BOOST = -0.04f;
    const float EMERALD_MINE_SPEED_BOOST = -0.002f;

};