#include "Torch.h"
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
    class TorchScript : public IScript {
    public:
        TorchScript() = default;
        ~TorchScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            auto torch = ecs.get<TorchComponent>(owner);
            auto& lightComp = ecs.get<LightComponent>(owner);
            auto& propsComp = ecs.get<SpritesheetPropertiesComponent>(owner);

            if(torch.isLit) {
                lightComp.light.brightness = 0.9f;
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
    entt::entity Torch::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f});
    }

    entt::entity Torch::create(entt::registry& ecs, strb::vec2f pos) {
        entt::entity torch = ecs.create();

        ecs.emplace<TransformComponent>(torch, pos, pos);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 16, 48};
        ecs.emplace<RenderComponent>(torch, render);

        ecs.emplace<ScriptComponent>(torch, ScriptComponent{std::make_shared<TorchScript>()});
        
        ecs.emplace<HueComponent>(torch, HueComponent{});

        ecs.emplace<TorchComponent>(torch, TorchComponent{});

        Light light;
        light.pos = strb::vec2f{render.renderQuad.x + render.renderQuad.w / 2, render.renderQuad.y + render.renderQuad.h / 2} / 16;
        light.brightness = 0.f;
        light.hue = HuePreset::softWarm;
        light.falloff = 0.06f;
        light.owner = torch;
        ecs.emplace<LightComponent>(torch, LightComponent{light});

        ecs.emplace<CollisionComponent>(torch, CollisionComponent{{pos.x, pos.y, 16.f, 48.f}, {0.f, 0.f}});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(SpritesheetRegistry::getSpritesheet(SpritesheetID::TORCH));
        ecs.emplace<SpritesheetPropertiesComponent>(torch, propsComp);

        return torch;
    }

    SpritesheetPropertiesComponent Torch::createSpritesheetPropertiesComponent(Spritesheet* spritesheet) {
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