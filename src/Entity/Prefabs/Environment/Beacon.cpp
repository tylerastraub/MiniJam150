#include "Beacon.h"
#include "SpritesheetRegistry.h"
// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "ScriptComponent.h"
#include "LightComponent.h"
#include "HueComponent.h"
#include "TorchComponent.h"

namespace {
    class BeaconScript : public IScript {
    public:
        BeaconScript() = default;
        ~BeaconScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            auto beacon = ecs.get<TorchComponent>(owner);
            auto& lightComp = ecs.get<LightComponent>(owner);
            auto& propsComp = ecs.get<SpritesheetPropertiesComponent>(owner);

            if(beacon.isLit) {
                lightComp.light.brightness = 1.f;
                auto props = propsComp.getPrimarySpritesheetProperties();
                props.yTileIndex = 1;
                propsComp.setPrimarySpritesheetProperties(props);
            }
            else {
                lightComp.light.brightness = 0.f;
                auto props = propsComp.getPrimarySpritesheetProperties();
                props.yTileIndex = 0;
                propsComp.setPrimarySpritesheetProperties(props);
            }
        }

    private:

    };
}

namespace prefab {
    entt::entity Beacon::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f});
    }

    entt::entity Beacon::create(entt::registry& ecs, strb::vec2f pos) {
        entt::entity beacon = ecs.create();

        ecs.emplace<TransformComponent>(beacon, pos, pos);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 16, 64};
        ecs.emplace<RenderComponent>(beacon, render);

        ecs.emplace<ScriptComponent>(beacon, ScriptComponent{std::make_shared<BeaconScript>()});
        
        ecs.emplace<HueComponent>(beacon, HueComponent{});

        ecs.emplace<TorchComponent>(beacon, TorchComponent{false, true});

        Light light;
        light.pos = strb::vec2f{render.renderQuad.x + render.renderQuad.w / 2, render.renderQuad.y + render.renderQuad.h / 2} / 16;
        light.brightness = 0.f;
        light.hue = HuePreset::brightWarm;
        light.falloff = 0.04f;
        light.owner = beacon;
        ecs.emplace<LightComponent>(beacon, LightComponent{light});

        ecs.emplace<CollisionComponent>(beacon, CollisionComponent{{pos.x, pos.y, 16.f, 64.f}, {0.f, 0.f}});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(SpritesheetRegistry::getSpritesheet(SpritesheetID::BEACON));
        ecs.emplace<SpritesheetPropertiesComponent>(beacon, propsComp);

        return beacon;
    }

    SpritesheetPropertiesComponent Beacon::createSpritesheetPropertiesComponent(Spritesheet* spritesheet) {
        SpritesheetPropertiesComponent propsComp;
        propsComp.spritesheet = spritesheet;

        SpritesheetProperties props = {
            0,                      // xTileIndex
            0,                      // yTileIndex
            false,                  // isAnimated
            false,                  // isLooped
            1,                      // numOfFrames
            1,                      // msBetweenFrames
            SDL_FLIP_NONE,          // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.setPrimarySpritesheetProperties(props);

        return propsComp;
    }
}