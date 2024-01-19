#pragma once

#include <entt/entity/registry.hpp>

#include "Level.h"

class CollisionSystem {
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;

    void updateLevelCollisionsOnXAxis(entt::registry& ecs, Level level);
    void updateLevelCollisionsOnYAxis(entt::registry& ecs, Level level);

private:

};