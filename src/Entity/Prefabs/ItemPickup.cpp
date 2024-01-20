#include "ItemPickup.h"
#include "SpritesheetRegistry.h"
#include "RandomGen.h"
// Components
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "ScriptComponent.h"
#include "HueComponent.h"
#include "PhysicsComponent.h"

namespace {
    class ItemPickupScript : public IScript {
    public:
        ItemPickupScript() = default;
        ~ItemPickupScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            
        }

    private:

    };
}

namespace prefab {
    entt::entity ItemPickup::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f}, ItemType::NOVAL);
    }

    entt::entity ItemPickup::create(entt::registry& ecs, strb::vec2f pos, ItemType itemType) {
        entt::entity item = ecs.create();

        ecs.emplace<TransformComponent>(item, pos, pos);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 16, 16};
        ecs.emplace<RenderComponent>(item, render);

        ecs.emplace<ScriptComponent>(item, ScriptComponent{std::make_shared<ItemPickupScript>()});
        
        ecs.emplace<HueComponent>(item, HueComponent{});

        PhysicsComponent physics;
        physics.gravity = 15.f;
        physics.velocity.y = static_cast<float>(RandomGen::getRandomInt(-150, -100));
        physics.velocity.x = static_cast<float>(RandomGen::getRandomInt(-50, 50));
        physics.airFrictionCoefficient = 0.f;
        physics.frictionCoefficient = 100.f;
        physics.touchingGround = false;
        physics.maxVelocity = {50.f, 400.f};
        ecs.emplace<PhysicsComponent>(item, physics);

        ecs.emplace<CollisionComponent>(item, CollisionComponent{{pos.x, pos.y + 6, 12, 10}, {0, 6}});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(itemType);
        ecs.emplace<SpritesheetPropertiesComponent>(item, propsComp);

        return item;
    }

    SpritesheetPropertiesComponent ItemPickup::createSpritesheetPropertiesComponent(ItemType itemType) {
        SpritesheetPropertiesComponent propsComp;

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

        switch(itemType) {
            case ItemType::COBALT:
                props.xTileIndex = 4;
                props.yTileIndex = 0;
                propsComp.spritesheet = SpritesheetRegistry::getSpritesheet(SpritesheetID::MINERALS);
                break;
            default:
                break;
        }

        propsComp.setPrimarySpritesheetProperties(props);

        return propsComp;
    }
}