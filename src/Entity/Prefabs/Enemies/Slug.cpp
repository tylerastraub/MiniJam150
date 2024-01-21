#include "Slug.h"
#include "SpritesheetRegistry.h"
#include "RandomGen.h"
// Components
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "StateComponent.h"
#include "DirectionComponent.h"
#include "ScriptComponent.h"
#include "AnimationComponent.h"
#include "LightComponent.h"
#include "HueComponent.h"
#include "EnemyComponent.h"

namespace {
    class SlugScript : public IScript {
    public:
        SlugScript() {
            _nextActionTime = RandomGen::getRandomInt(_actionMin, _actionMax);
        }
        ~SlugScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            auto& state = ecs.get<StateComponent>(owner);
            auto& dir = ecs.get<DirectionComponent>(owner);
            auto& physics = ecs.get<PhysicsComponent>(owner);

            if(_activeActionTimer > _activeActionTime) {
                _currentActionTimer += timescale * 1000.f;
                if(_currentActionTimer >= _nextActionTime) {
                    _currentActionTimer = 0;
                    _nextActionTime = RandomGen::getRandomInt(_actionMin, _actionMax);
                    _activeActionTimer = 0;
                    _activeActionTime = RandomGen::getRandomInt(_actionMin, _actionMax);
                    dir.direction = (RandomGen::getRandomInt(0, 1)) ? Direction::EAST : Direction::WEST;
                }
            }
            else {
                _activeActionTimer += timescale * 1000.f;
                int coefficient = (dir.direction == Direction::EAST) ? 1.f : -1.f;
                physics.velocity.x = physics.maxVelocity.x * coefficient;
            }

            // state setting
            if(physics.velocity.x < 0) {
                dir.direction = Direction::WEST;
            }
            else if(physics.velocity.x > 0) {
                dir.direction = Direction::EAST;
            }
        }

    private:
        int _currentActionTimer = 0;
        int _nextActionTime = 0;
        int _activeActionTimer = 0;
        int _activeActionTime = 0;
        int _actionMin = 3000;
        int _actionMax = 10000;
        int _actionDurationMin = 3000;
        int _actionDurationMax = 6000;

    };
}

namespace prefab {
    entt::entity Slug::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f});
    }

    entt::entity Slug::create(entt::registry& ecs, strb::vec2f pos) {
        entt::entity slug = ecs.create();

        ecs.emplace<TransformComponent>(slug, pos, pos);

        PhysicsComponent physics;
        physics.maxVelocity = {10.f, 400.f};
        physics.frictionCoefficient = {5.f};
        physics.acceleration = {10.f, 10.f};
        physics.gravity = 5.f;
        ecs.emplace<PhysicsComponent>(slug, physics);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 36, 20};
        ecs.emplace<RenderComponent>(slug, render);

        ecs.emplace<CollisionComponent>(slug, CollisionComponent{{pos.x + 2, pos.y, 32, 10}, {2, 10}});

        ecs.emplace<StateComponent>(slug, StateComponent{EntityState::IDLE});

        ecs.emplace<DirectionComponent>(slug, DirectionComponent{Direction::EAST});

        ecs.emplace<ScriptComponent>(slug, ScriptComponent{std::make_shared<SlugScript>()});

        ecs.emplace<AnimationComponent>(slug, AnimationComponent{});

        ecs.emplace<HueComponent>(slug, HueComponent{});

        ecs.emplace<EnemyComponent>(slug, EnemyComponent{});

        // Light light;
        // light.pos = strb::vec2f{render.renderQuad.x + render.renderQuad.w / 2, render.renderQuad.y + render.renderQuad.h / 2} / 16;
        // light.brightness = 0.8f;
        // light.hue = HuePreset::cool;
        // light.falloff = 0.15f;
        // light.owner = slug;
        // ecs.emplace<LightComponent>(slug, LightComponent{light});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(SpritesheetRegistry::getSpritesheet(SpritesheetID::SLUG));
        ecs.emplace<SpritesheetPropertiesComponent>(slug, propsComp);

        return slug;
    }

    SpritesheetPropertiesComponent Slug::createSpritesheetPropertiesComponent(Spritesheet* spritesheet) {
        SpritesheetPropertiesComponent propsComp;
        propsComp.spritesheet = spritesheet;

        SpritesheetProperties idleEast = {
            0,                      // xTileIndex
            0,                      // yTileIndex
            true,                   // isAnimated
            true,                   // isLooped
            NUM_OF_IDLE_FRAMES,     // numOfFrames
            MS_BETWEEN_IDLE_FRAMES, // msBetweenFrames
            SDL_FLIP_HORIZONTAL,    // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::IDLE, Direction::EAST, idleEast);

        SpritesheetProperties idleWest = {
            0,                      // xTileIndex
            0,                      // yTileIndex
            true,                   // isAnimated
            true,                   // isLooped
            NUM_OF_IDLE_FRAMES,     // numOfFrames
            MS_BETWEEN_IDLE_FRAMES, // msBetweenFrames
            SDL_FLIP_NONE,          // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::IDLE, Direction::WEST, idleWest);

        return propsComp;
    }
}