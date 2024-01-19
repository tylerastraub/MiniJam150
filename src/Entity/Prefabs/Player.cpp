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

            if(physics.touchingGround) input.allowedInputs = {InputEvent::LEFT, InputEvent::RIGHT, InputEvent::JUMP};
            else input.allowedInputs = {InputEvent::LEFT, InputEvent::RIGHT};

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
        physics.jumpPower = 250.f;
        ecs.emplace<PhysicsComponent>(player, physics);

        ecs.emplace<RenderComponent>(player, RenderComponent{{0, 0, 24, 32}});

        ecs.emplace<InputComponent>(player, InputComponent{{InputEvent::LEFT, InputEvent::RIGHT, InputEvent::JUMP}});

        ecs.emplace<CollisionComponent>(player, CollisionComponent{{pos.x, pos.y, 24, 32}, {0, 0}});

        ecs.emplace<StateComponent>(player, StateComponent{EntityState::IDLE});

        ecs.emplace<DirectionComponent>(player, DirectionComponent{Direction::EAST});

        ecs.emplace<ScriptComponent>(player, ScriptComponent{std::make_shared<PlayerScript>()});

        ecs.emplace<AnimationComponent>(player, AnimationComponent{});

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

        return propsComp;
    }
}