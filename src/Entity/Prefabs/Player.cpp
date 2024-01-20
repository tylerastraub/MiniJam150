#include "Player.h"
#include "SpritesheetRegistry.h"
// Components
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "InputComponent.h"
#include "CollisionComponent.h"
#include "StateComponent.h"
#include "DirectionComponent.h"
#include "ScriptComponent.h"
#include "AnimationComponent.h"
#include "LightComponent.h"
#include "MiningComponent.h"
#include "HueComponent.h"

namespace {
    class PlayerScript : public IScript {
    public:
        PlayerScript() = default;
        ~PlayerScript() = default;

        void update(entt::registry& ecs, entt::entity owner, float timescale, std::shared_ptr<Audio> audio) override {
            auto& state = ecs.get<StateComponent>(owner);
            auto& dir = ecs.get<DirectionComponent>(owner);
            auto& input = ecs.get<InputComponent>(owner);
            auto physics = ecs.get<PhysicsComponent>(owner);
            auto mining = ecs.get<MiningComponent>(owner);

            if(mining.isMining) {
                input.allowedInputs = {InputEvent::ACTION};
            }
            else if(physics.offGroundCount <= physics.coyoteTime) {
                input.allowedInputs = {InputEvent::LEFT, InputEvent::RIGHT, InputEvent::JUMP};
                if(mining.canMine) input.allowedInputs.push_back(InputEvent::ACTION);
            }
            else {
                input.allowedInputs = {InputEvent::LEFT, InputEvent::RIGHT};
            }

            // state setting
            if(physics.velocity.x < 0) {
                dir.direction = Direction::WEST;
            }
            else if(physics.velocity.x > 0) {
                dir.direction = Direction::EAST;
            }

            if(!physics.touchingGround) {
                state.state = EntityState::JUMPING;
            }
            else if(physics.velocity.x != 0) {
                state.state = EntityState::MOVING;
            }
            else if(mining.isMining) {
                state.state = EntityState::MINING;
            }
            else {
                state.state = EntityState::IDLE;
            }
        }

    private:

    };
}

namespace prefab {
    entt::entity Player::create(entt::registry& ecs) {
        return create(ecs, {0.f, 0.f});
    }

    entt::entity Player::create(entt::registry& ecs, strb::vec2f pos) {
        entt::entity player = ecs.create();

        ecs.emplace<TransformComponent>(player, pos, pos);

        PhysicsComponent physics;
        physics.maxVelocity = {50.f, 400.f};
        physics.frictionCoefficient = {10.f};
        physics.acceleration = {20.f, 20.f};
        physics.jumpPower = 180.f;
        ecs.emplace<PhysicsComponent>(player, physics);

        RenderComponent render;
        render.renderQuad = {pos.x, pos.y, 24, 32};
        ecs.emplace<RenderComponent>(player, render);

        ecs.emplace<InputComponent>(player, InputComponent{{InputEvent::LEFT, InputEvent::RIGHT, InputEvent::JUMP, InputEvent::ACTION}});

        ecs.emplace<CollisionComponent>(player, CollisionComponent{{pos.x + 6, pos.y, 12, 32}, {6, 0}});

        ecs.emplace<StateComponent>(player, StateComponent{EntityState::IDLE});

        ecs.emplace<DirectionComponent>(player, DirectionComponent{Direction::EAST});

        ecs.emplace<ScriptComponent>(player, ScriptComponent{std::make_shared<PlayerScript>()});

        ecs.emplace<AnimationComponent>(player, AnimationComponent{});

        ecs.emplace<MiningComponent>(player, MiningComponent{});

        ecs.emplace<HueComponent>(player, HueComponent{});

        Light light;
        light.pos = strb::vec2f{render.renderQuad.x + render.renderQuad.w / 2, render.renderQuad.y + render.renderQuad.h / 2} / 16;
        light.brightness = 1.f;
        light.hue = HuePreset::cool;
        light.falloff = 0.1f;
        ecs.emplace<LightComponent>(player, LightComponent{light});

        SpritesheetPropertiesComponent propsComp = createSpritesheetPropertiesComponent(SpritesheetRegistry::getSpritesheet(SpritesheetID::PLAYER));
        ecs.emplace<SpritesheetPropertiesComponent>(player, propsComp);

        return player;
    }

    SpritesheetPropertiesComponent Player::createSpritesheetPropertiesComponent(Spritesheet* spritesheet) {
        SpritesheetPropertiesComponent propsComp;
        propsComp.spritesheet = spritesheet;

        SpritesheetProperties idleEast = {
            0,                      // xTileIndex
            0,                      // yTileIndex
            true,                   // isAnimated
            true,                  // isLooped
            NUM_OF_IDLE_FRAMES,     // numOfFrames
            MS_BETWEEN_IDLE_FRAMES, // msBetweenFrames
            SDL_FLIP_NONE,          // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::IDLE, Direction::EAST, idleEast);

        SpritesheetProperties idleWest = {
            0,                      // xTileIndex
            0,                      // yTileIndex
            true,                   // isAnimated
            true,                  // isLooped
            NUM_OF_IDLE_FRAMES,     // numOfFrames
            MS_BETWEEN_IDLE_FRAMES, // msBetweenFrames
            SDL_FLIP_HORIZONTAL,    // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::IDLE, Direction::WEST, idleWest);

        SpritesheetProperties movingEast = {
            0,                        // xTileIndex
            1,                        // yTileIndex
            true,                     // isAnimated
            true,                    // isLooped
            NUM_OF_MOVING_FRAMES,     // numOfFrames
            MS_BETWEEN_MOVING_FRAMES, // msBetweenFrames
            SDL_FLIP_NONE,            // flip
            0.0,                      // angle
            {-1.f, -1.f}              // center
        };
        propsComp.addSpritesheetProperties(EntityState::MOVING, Direction::EAST, movingEast);

        SpritesheetProperties movingWest = {
            0,                        // xTileIndex
            1,                        // yTileIndex
            true,                     // isAnimated
            true,                    // isLooped
            NUM_OF_MOVING_FRAMES,     // numOfFrames
            MS_BETWEEN_MOVING_FRAMES, // msBetweenFrames
            SDL_FLIP_HORIZONTAL,      // flip
            0.0,                      // angle
            {-1.f, -1.f}              // center
        };
        propsComp.addSpritesheetProperties(EntityState::MOVING, Direction::WEST, movingWest);

        SpritesheetProperties jumpingEast = {
            0,                      // xTileIndex
            2,                      // yTileIndex
            false,                  // isAnimated
            false,                  // isLooped
            1,                      // numOfFrames
            1,                      // msBetweenFrames
            SDL_FLIP_NONE,          // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::JUMPING, Direction::EAST, jumpingEast);

        SpritesheetProperties jumpingWest = {
            0,                      // xTileIndex
            2,                      // yTileIndex
            false,                  // isAnimated
            false,                  // isLooped
            1,                      // numOfFrames
            1,                      // msBetweenFrames
            SDL_FLIP_HORIZONTAL,    // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::JUMPING, Direction::WEST, jumpingWest);

        SpritesheetProperties miningEast = {
            0,                      // xTileIndex
            3,                      // yTileIndex
            false,                  // isAnimated
            false,                  // isLooped
            1,                      // numOfFrames
            1,                      // msBetweenFrames
            SDL_FLIP_NONE,          // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::MINING, Direction::EAST, miningEast);

        SpritesheetProperties miningWest = {
            0,                      // xTileIndex
            3,                      // yTileIndex
            false,                  // isAnimated
            false,                  // isLooped
            1,                      // numOfFrames
            1,                      // msBetweenFrames
            SDL_FLIP_HORIZONTAL,    // flip
            0.0,                    // angle
            {-1.f, -1.f}            // center
        };
        propsComp.addSpritesheetProperties(EntityState::MINING, Direction::WEST, miningWest);

        return propsComp;
    }
}