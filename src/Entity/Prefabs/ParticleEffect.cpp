#include "ParticleEffect.h"
#include "SpritesheetRegistry.h"
// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ScriptComponent.h"
#include "AnimationComponent.h"
#include "HueComponent.h"

namespace {
    class ParticleEffectScript : public IScript {
    public:
        ParticleEffectScript() = default;
        ~ParticleEffectScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            auto animation = ecs.get<AnimationComponent>(owner);
        }

    private:

    };
}

namespace prefab {
    entt::entity ParticleEffect::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f}, ParticleType::NOVAL);
    }

    entt::entity ParticleEffect::create(entt::registry& ecs, strb::vec2f pos, ParticleType particleType) {
        entt::entity particleEffect = ecs.create();

        ecs.emplace<TransformComponent>(particleEffect, pos, pos);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 16, 16};
        render.renderPriority = 10;
        ecs.emplace<RenderComponent>(particleEffect, render);

        ecs.emplace<ScriptComponent>(particleEffect, ScriptComponent{std::make_shared<ParticleEffectScript>()});

        ecs.emplace<AnimationComponent>(particleEffect, AnimationComponent{});

        ecs.emplace<HueComponent>(particleEffect, HueComponent{});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(particleType);
        ecs.emplace<SpritesheetPropertiesComponent>(particleEffect, propsComp);

        return particleEffect;
    }

    SpritesheetPropertiesComponent ParticleEffect::createSpritesheetPropertiesComponent(ParticleType particleType) {
        SpritesheetPropertiesComponent propsComp;

        SpritesheetProperties props = {
            0,                      // xTileIndex
            0,                      // yTileIndex
            true,                   // isAnimated
            true,                   // isLooped
            1,                      // numOfFrames
            1,                      // msBetweenFrames
            SDL_FLIP_NONE,          // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };

        switch(particleType) {
            case ParticleType::MINING_MAGIC:
                props.numOfFrames = 7;
                props.msBetweenFrames = 100;
                propsComp.spritesheet = SpritesheetRegistry::getSpritesheet(SpritesheetID::MINING_MAGIC);
                break;
            default:
                break;
        }

        propsComp.setPrimarySpritesheetProperties(props);

        return propsComp;
    }
}