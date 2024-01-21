#pragma once

#include "Text.h"
#include "ItemType.h"

#include <entt/entity/registry.hpp>
#include <memory>

class InventorySystem {
public:
    InventorySystem() = default;
    ~InventorySystem() = default;
    
    void render(entt::registry& ecs, std::shared_ptr<Text> text);

private:
    int getYSpritesheetIndex(ItemType itemType);

};