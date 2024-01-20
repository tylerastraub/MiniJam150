#include "LightSystem.h"
#include "LightComponent.h"
#include "TransformComponent.h"

void LightSystem::update(entt::registry& ecs, Level& level) {
    auto view = ecs.view<LightComponent, TransformComponent>();
    for(auto entity : view) {
        auto transform = ecs.get<TransformComponent>(entity);
        if(transform.position != transform.lastPosition) {
            auto& lightComp = ecs.get<LightComponent>(entity);
            if(level.getLightMap()->hasLight(lightComp.light.id)) removeLightSource(level, lightComp.light);
            lightComp.light.pos += (transform.position - transform.lastPosition) / level.getTileSize();
            lightComp.light.id = addLightSource(level, lightComp.light);
        }
    }
}

void LightSystem::updateLightSources(entt::registry& ecs, Level& level) {
    auto view = ecs.view<LightComponent>();
    for(auto entity : view) {
        auto& lightComp = ecs.get<LightComponent>(entity);
        if(!level.getLightMap()->hasLight(lightComp.light.id)) {
            lightComp.light.id = level.getLightMap()->addLightSource(lightComp.light);
        }
    }
}

uint16_t LightSystem::addLightSource(Level& level, Light light) {
    return level.getLightMap()->addLightSource(light);
}

void LightSystem::removeLightSource(Level& level, Light light) {
    level.getLightMap()->removeLightSource(light.id);
}