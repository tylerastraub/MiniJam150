#pragma once


#include "Level.h"
#include "Audio.h"

#include <memory>
#include <entt/entity/registry.hpp>

class CollisionSystem {
public:
    CollisionSystem() = default;
    ~CollisionSystem() = default;

    void updateLevelCollisionsOnXAxis(entt::registry& ecs, Level level);
    void updateLevelCollisionsOnYAxis(entt::registry& ecs, Level level);

    void checkForMiningCollisions(entt::registry& ecs);
    void checkForItemPickupCollisions(entt::registry& ecs, float timescale, std::shared_ptr<Audio> audio);

private:

};