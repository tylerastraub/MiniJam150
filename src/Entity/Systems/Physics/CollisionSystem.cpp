#include "CollisionSystem.h"
#include "RectUtils.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "CollisionComponent.h"
#include "MiningComponent.h"
#include "MineralComponent.h"
#include "DirectionComponent.h"
#include "PlayerComponent.h"
#include "ItemPickupComponent.h"
#include "TorchComponent.h"
#include "InventoryComponent.h"
#include "HealthComponent.h"
#include "EnemyComponent.h"

void CollisionSystem::updateLevelCollisionsOnXAxis(entt::registry& ecs, Level level) {
    auto view = ecs.view<TransformComponent, CollisionComponent, PhysicsComponent>();
    for(auto entity : view) {
        auto& transform = ecs.get<TransformComponent>(entity);
        auto& physics = ecs.get<PhysicsComponent>(entity);
        auto& collision = ecs.get<CollisionComponent>(entity);
        int ts = level.getTileSize();

        collision.collisionRect.x = transform.position.x + collision.collisionRectOffset.x;

        strb::vec2i topLeftTile = {static_cast<int>(std::floor(collision.collisionRect.x)) / ts,
            static_cast<int>(std::floor(collision.collisionRect.y)) / ts};
        strb::vec2i bottomRightTile = {static_cast<int>(std::ceil(collision.collisionRect.x + collision.collisionRect.w)) / ts,
            static_cast<int>(std::ceil(collision.collisionRect.y + collision.collisionRect.h - 1)) / ts};

        std::vector<strb::vec2i> tileCollisions;
        if(physics.velocity.x < 0) {
            collision.collidingRight = false;
            // get all tiles to the left
            for(int y = topLeftTile.y; y <= bottomRightTile.y; ++y) {
                Tile t = level.getTileAt(topLeftTile.x, y);
                if(t.type == TileType::SOLID) {
                    tileCollisions.push_back({topLeftTile.x, y});
                }
            }

            if(tileCollisions.size()) {
                // we have a hit!
                strb::vec2i tilePos = *tileCollisions.begin();
                transform.position.x = tilePos.x * ts + ts - collision.collisionRectOffset.x;
                collision.collisionRect.x = transform.position.x + collision.collisionRectOffset.x;
                physics.velocity.x = 0;
                collision.collidingLeft = true;
            }
            else {
                collision.collidingLeft = false;
            }
        }
        else if(physics.velocity.x > 0) {
            collision.collidingLeft = false;
            // get all tiles to the right
            for(int y = topLeftTile.y; y <= bottomRightTile.y; ++y) {
                Tile t = level.getTileAt(bottomRightTile.x, y);
                if(t.type == TileType::SOLID) {
                    tileCollisions.push_back({bottomRightTile.x, y});
                }
            }

            if(tileCollisions.size()) {
                // we have a hit!
                strb::vec2i tilePos = *tileCollisions.begin();
                transform.position.x = tilePos.x * ts - collision.collisionRect.w - collision.collisionRectOffset.x;
                collision.collisionRect.x = transform.position.x + collision.collisionRectOffset.x;
                physics.velocity.x = 0;
                collision.collidingRight = true;
            }
            else {
                collision.collidingRight = false;
            }
        }
    }
}

void CollisionSystem::updateLevelCollisionsOnYAxis(entt::registry& ecs, Level level) {
    auto view = ecs.view<TransformComponent, CollisionComponent, PhysicsComponent>();
    for(auto entity : view) {
        auto& transform = ecs.get<TransformComponent>(entity);
        auto& physics = ecs.get<PhysicsComponent>(entity);
        auto& collision = ecs.get<CollisionComponent>(entity);
        int ts = level.getTileSize();

        collision.collisionRect.y = transform.position.y + collision.collisionRectOffset.y;

        strb::vec2i topLeftTile = {static_cast<int>(std::floor(collision.collisionRect.x)) / ts,
            static_cast<int>(std::floor(collision.collisionRect.y)) / ts};
        strb::vec2i bottomRightTile = {static_cast<int>(std::ceil(collision.collisionRect.x + collision.collisionRect.w - 1)) / ts,
            static_cast<int>(std::ceil(collision.collisionRect.y + collision.collisionRect.h)) / ts};

        std::vector<strb::vec2i> tileCollisions;
        if(physics.velocity.y < 0) {
            collision.collidingDown = false;
            // get all tiles above us
            for(int x = topLeftTile.x; x <= bottomRightTile.x; ++x) {
                Tile t = level.getTileAt(x, topLeftTile.y);
                if(t.type == TileType::SOLID) {
                    tileCollisions.push_back({x, topLeftTile.y});
                }
            }

            if(tileCollisions.size()) {
                // we have a hit!
                strb::vec2i tilePos = *tileCollisions.begin();
                transform.position.y = tilePos.y * ts + ts - collision.collisionRectOffset.y;
                collision.collisionRect.y = transform.position.y + collision.collisionRectOffset.y;
                physics.velocity.y = 0;
                collision.collidingUp = true;
            }
            else {
                collision.collidingUp = false;
            }
        }
        else if(physics.velocity.y > 0) {
            collision.collidingUp = false;
            // get all tiles below us
            for(int x = topLeftTile.x; x <= bottomRightTile.x; ++x) {
                Tile t = level.getTileAt(x, bottomRightTile.y);
                if(t.type == TileType::SOLID) {
                    tileCollisions.push_back({x, bottomRightTile.y});
                }
            }

            if(tileCollisions.size()) {
                // we have a hit!
                strb::vec2i tilePos = *tileCollisions.begin();
                transform.position.y = tilePos.y * ts - collision.collisionRect.h - collision.collisionRectOffset.y;
                collision.collisionRect.y = transform.position.y + collision.collisionRectOffset.y;
                physics.velocity.y = 0;
                collision.collidingDown = true;
                physics.touchingGround = true;
            }
            else {
                collision.collidingDown = false;
                physics.touchingGround = false;
            }
        }
    }
}

void CollisionSystem::checkForMiningCollisions(entt::registry& ecs) {
    auto view = ecs.view<TransformComponent, MiningComponent, DirectionComponent, CollisionComponent>();
    auto minerals = ecs.view<MineralComponent>();
    for(auto entity : view) {
        auto& mining = ecs.get<MiningComponent>(entity);
        auto dir = ecs.get<DirectionComponent>(entity);
        auto collision = ecs.get<CollisionComponent>(entity);
        if(dir.direction == Direction::EAST) {
            mining.collisionRect.x = collision.collisionRect.x + collision.collisionRect.w;
        }
        else if(dir.direction == Direction::WEST) {
            mining.collisionRect.x = collision.collisionRect.x - mining.collisionRect.w;
        }
        mining.collisionRect.y = collision.collisionRect.y;
        if(mining.isMining) {
            for(auto mineral : minerals) {
                auto mineralTransform = ecs.get<TransformComponent>(mineral);
                auto& mineralComp = ecs.get<MineralComponent>(mineral);
                mineralComp.collisionRect.x = mineralTransform.position.x + mineralComp.collisionRectOffset.x;
                mineralComp.collisionRect.y = mineralTransform.position.y + mineralComp.collisionRectOffset.y;
                if(RectUtils::isIntersecting(mining.collisionRect, mineralComp.collisionRect)) {
                    mining.mineral = mineral;
                    mineralComp.minedBy = entity;
                    mineralComp.minedPercent += mineralComp.mineSpeed;
                    if(mineralComp.minedPercent > 1.f) mineralComp.minedPercent = 1.f;
                    break;
                }
                else {
                    mining.mineral = entt::null;
                    mineralComp.minedBy = entt::null;
                }
            }
        }
        else {
            for(auto mineral : minerals) {
                auto& mineralComp = ecs.get<MineralComponent>(mineral);
                if(mineralComp.minedPercent < 1.f) mineralComp.minedPercent = 0.f;
                mineralComp.minedBy = entt::null;
            }
        }
    }
}

void CollisionSystem::checkForItemPickupCollisions(entt::registry& ecs, float timescale, std::shared_ptr<Audio> audio) {
    auto pView = ecs.view<PlayerComponent>();
    auto pickupView = ecs.view<ItemPickupComponent>();
    for(auto player : pView) {
        auto& health = ecs.get<HealthComponent>(player);
        if(health.health <= 0) return;
        auto pCollision = ecs.get<CollisionComponent>(player).collisionRect;
        for(auto item : pickupView) {
            auto iCollision = ecs.get<CollisionComponent>(item).collisionRect;
            if(RectUtils::isIntersecting(pCollision, iCollision)) {
                auto& itemPickupComp = ecs.get<ItemPickupComponent>(item);
                itemPickupComp.pickedUpBy = player;
                itemPickupComp.onPickupScript->update(ecs, item, timescale, audio);
                ecs.destroy(item);
            }
        }
    }
}

void CollisionSystem::checkForTorchAndBeaconCollisions(entt::registry& ecs, std::shared_ptr<Audio> audio) {
    entt::entity player = *ecs.view<PlayerComponent>().begin();
    auto& health = ecs.get<HealthComponent>(player);
    if(health.health <= 0) return;
    auto& playerComp = ecs.get<PlayerComponent>(player);
    if(playerComp.requestsLight) {
        playerComp.requestsLight = false;
        auto pCollision = ecs.get<CollisionComponent>(player).collisionRect;
        auto& inventory = ecs.get<InventoryComponent>(player);
        auto torchAndBeaconView = ecs.view<TorchComponent>();
        for(auto torch : torchAndBeaconView) {
            auto& torchComp = ecs.get<TorchComponent>(torch);
            auto tCollision = ecs.get<CollisionComponent>(torch).collisionRect;
            if(RectUtils::isIntersecting(pCollision, tCollision)) {
                if(torchComp.isBeacon && inventory.inventory[ItemType::TOPAZ] > 0) {
                    inventory.inventory[ItemType::TOPAZ]--;
                    torchComp.isLit = true;
                    playerComp.beaconsLit++;
                    playerComp.beaconRecentlyLit = true;
                    audio->playAudio(entt::null, AudioSound::BEACON_LIT, 1.f);
                    return;
                }
                else if(!torchComp.isBeacon && inventory.inventory[ItemType::COBALT] > 0) {
                    inventory.inventory[ItemType::COBALT]--;
                    torchComp.isLit = true;
                    audio->playAudio(entt::null, AudioSound::TORCH_LIT, 1.f);
                    return;
                }
            }
        }
    }
}

void CollisionSystem::checkForPlayerAndEnemyCollisions(entt::registry& ecs, float timescale, std::shared_ptr<Audio> audio) {
    entt::entity player = *ecs.view<PlayerComponent, CollisionComponent, HealthComponent>().begin();
    auto& health = ecs.get<HealthComponent>(player);
    if(health.currentInvulnTimer >= health.invulnTime && health.health > 0) {
        auto enemyView = ecs.view<EnemyComponent, CollisionComponent>();
        auto pCollision = ecs.get<CollisionComponent>(player).collisionRect;
        for(auto enemy : enemyView) {
            auto eCollision = ecs.get<CollisionComponent>(enemy).collisionRect;
            if(RectUtils::isIntersecting(pCollision, eCollision)) {
                health.health--;
                health.currentInvulnTimer = 0;
                auto pPos = ecs.get<TransformComponent>(player).position;
                auto ePos = ecs.get<TransformComponent>(enemy).position;
                auto& physics = ecs.get<PhysicsComponent>(player);
                float coefficient = (pPos.x - ePos.x < 0.f) ? -1.f : 1.f;
                physics.velocity = {120.f * coefficient, -80.f};
                audio->playAudio(player, AudioSound::PLAYER_HURT, 1.f);
                return;
            }
        }
    }
}