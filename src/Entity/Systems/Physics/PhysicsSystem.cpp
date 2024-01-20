#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

#include <iostream>
#include <algorithm>

bool PhysicsSystem::updateX(entt::registry& ecs, float timescale) {
    bool entityMoved = false;
    auto entities = ecs.view<PhysicsComponent, TransformComponent>();
    for(auto ent : entities) {
        auto& physics = ecs.get<PhysicsComponent>(ent);
        auto& transform = ecs.get<TransformComponent>(ent);

        transform.lastPosition.x = transform.position.x;
        if(physics.velocity.x != 0.f) {
            entityMoved = true;
            transform.position.x += physics.velocity.x * timescale;
            float friction = (physics.touchingGround) ? physics.frictionCoefficient : physics.airFrictionCoefficient;
            moveToZero(physics.velocity.x, friction);
            if(physics.velocity.x > physics.maxVelocity.x) physics.velocity.x = physics.maxVelocity.x;
            else if(physics.velocity.x < physics.maxVelocity.x * -1.f) physics.velocity.x = physics.maxVelocity.x * -1.f;
        }
    }
    return entityMoved;
}

bool PhysicsSystem::updateY(entt::registry& ecs, float timescale) {
    bool entityMoved = false;
    auto entities = ecs.view<PhysicsComponent, TransformComponent>();
    for(auto ent : entities) {
        auto& physics = ecs.get<PhysicsComponent>(ent);
        auto& transform = ecs.get<TransformComponent>(ent);

        transform.lastPosition.y = transform.position.y;
        physics.velocity.y += physics.gravity;
        if(physics.velocity.y != 0.f) {
            entityMoved = true;
            transform.position.y += physics.velocity.y * timescale;
            if(physics.velocity.y > physics.maxVelocity.y) physics.velocity.y = physics.maxVelocity.y;
            else if(physics.velocity.y < physics.maxVelocity.y * -1.f) physics.velocity.y = physics.maxVelocity.y * -1.f;
        }
        if(physics.touchingGround) physics.offGroundCount = 0;
        else physics.offGroundCount++;
    }

    return entityMoved;
}

void PhysicsSystem::moveToZero(float &value, float amount) {
    if(value != 0.f) {
        if(value > 0.f) {
            value = (value > amount) ? value - amount : 0.f;
        }
        else {
            value = (std::abs(value) > amount) ? value + amount : 0.f;
        }
    }
}