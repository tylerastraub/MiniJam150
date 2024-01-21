#pragma once

#include "rect2.h"
#include "vec2.h"
#include "ItemType.h"

#include <cmath>
#include <entt/entity/entity.hpp>

struct MineralComponent {
    ItemType type = ItemType::NOVAL;
    strb::rect2f collisionRect = {0.f, 0.f, 0.f, 0.f};
    strb::vec2f collisionRectOffset = {0.f, 0.f};
    float mineSpeed = 0.01f;
    float minedPercent = 0.f;
    entt::entity minedBy = entt::null;

    int getMineralMinedSpritesheetXIndex() {
        return static_cast<int>(std::ceil(3.f * minedPercent));
    }

    int getMineralSpritesheetYIndex() {
        return static_cast<int>(type);
    }
};