#pragma once

#include "SpritesheetPropertiesComponent.h"
#include "ParticleType.h"

#include <entt/entity/registry.hpp>

namespace prefab {
    class ParticleEffect {
    public:
        ParticleEffect() = default;
        ~ParticleEffect() = default;

        static entt::entity create(entt::registry& ecs);
        static entt::entity create(entt::registry& ecs, strb::vec2f pos, ParticleType particleType);

    private:
        static SpritesheetPropertiesComponent createSpritesheetPropertiesComponent(ParticleType particleType);

    };
}