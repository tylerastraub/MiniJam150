#include "Mineral.h"
#include "SpritesheetRegistry.h"
#include "ItemPickup.h"
// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "ScriptComponent.h"
#include "LightComponent.h"
#include "HueComponent.h"
#include "MiningComponent.h"

namespace {
    class MineralScript : public IScript {
    public:
        MineralScript() = default;
        ~MineralScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            auto& mineral = ecs.get<MineralComponent>(owner);
            auto& propsComp = ecs.get<SpritesheetPropertiesComponent>(owner);
            auto& lightComp = ecs.get<LightComponent>(owner);

            // spritesheet setting
            SpritesheetProperties props = propsComp.getPrimarySpritesheetProperties();
            props.xTileIndex = mineral.getMineralMinedSpritesheetXIndex();
            props.yTileIndex = mineral.getMineralSpritesheetYIndex();
            propsComp.setPrimarySpritesheetProperties(props);

            if(mineral.minedBy != entt::null) {
                lightComp.light.brightness = 0.6f;
            }
            else {
                lightComp.light.brightness = 0.f;
            }

            if(mineral.minedPercent == 1.f) {
                if(mineral.minedBy != entt::null) {
                    auto& miner = ecs.get<MiningComponent>(mineral.minedBy);
                    miner.canMine = false;
                    miner.isMining = false;
                }
                auto pos = ecs.get<TransformComponent>(owner).position;
                auto quad = ecs.get<RenderComponent>(owner).renderQuad;
                prefab::ItemPickup::create(ecs, {pos.x, pos.y - 8}, convertMineralTypeToItemType(mineral.type));
                ecs.destroy(owner);
            }
        }
        
    
        ItemType convertMineralTypeToItemType(MineralType mineralType) {
        switch(mineralType) {
            case MineralType::COBALT:
                return ItemType::COBALT;
            default:
                break;
        }

        return ItemType::NOVAL;
    }

    private:

    };
}

namespace prefab {
    entt::entity Mineral::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f}, MineralType::NOVAL);
    }

    entt::entity Mineral::create(entt::registry& ecs, strb::vec2f pos, MineralType mineralType) {
        entt::entity mineral = ecs.create();

        ecs.emplace<TransformComponent>(mineral, pos, pos);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 16, 16};
        ecs.emplace<RenderComponent>(mineral, render);

        ecs.emplace<ScriptComponent>(mineral, ScriptComponent{std::make_shared<MineralScript>()});

        ecs.emplace<MineralComponent>(mineral, MineralComponent{mineralType, {0.f, 0.f, 16.f, 16.f}, {0.f, 0.f}, getMineralMineSpeed(mineralType)});
        
        ecs.emplace<HueComponent>(mineral, HueComponent{});

        Light light;
        light.pos = strb::vec2f{render.renderQuad.x + render.renderQuad.w / 2, render.renderQuad.y + render.renderQuad.h / 2} / 16;
        light.brightness = 0.f;
        light.hue = getMineralHue(mineralType);
        light.falloff = 0.2f;
        light.owner = mineral;
        ecs.emplace<LightComponent>(mineral, LightComponent{light});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(SpritesheetRegistry::getSpritesheet(SpritesheetID::MINERALS));
        ecs.emplace<SpritesheetPropertiesComponent>(mineral, propsComp);

        return mineral;
    }

    SpritesheetPropertiesComponent Mineral::createSpritesheetPropertiesComponent(Spritesheet* spritesheet) {
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

    float Mineral::getMineralMineSpeed(MineralType mineralType) {
        switch(mineralType) {
            case MineralType::COBALT:
                return 0.01f;
            default:
                break;
        }
        return 0.f;
    }

    Hue Mineral::getMineralHue(MineralType mineralType) {
        switch(mineralType) {
            case MineralType::COBALT:
                return HuePreset::cobalt;
            default:
                break;
        }
        return {0x00, 0x00, 0x00};
    }
}