#include "GameState.h"
#include "RandomGen.h"
#include "SpritesheetRegistry.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "MiningComponent.h"
#include "LevelParser.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "LightComponent.h"
// Prefabs
#include "Player.h"

#include <chrono>

std::mt19937 RandomGen::randEng{(unsigned int) std::chrono::system_clock::now().time_since_epoch().count()};

/**
 * @todo
 * ==== ESSENTIALS ====
 * - Add sounds
 * 
 * ==== NICE TO HAVES ====
 * - Add way to help tell where you are
 *     - Easiest: depth counter/coordinate system
 *     - Medium: Add flags/checkpoints throughout cave
 *     - Hard: Add live minimap
 * - Add jumping enemy (frog?)
 * - Make enemies avoid light (so that torches have more reason to be lit up)
 * - Add rose quartz and emerald mineral
 * - Add wizard lab
 * - Add music
 * - Add more extra art :)
 * - L canceling
*/

bool GameState::init() {
    _level = LevelParser::parseLevelFromTmx(_ecs, "res/tiled/main_level.tmx", SpritesheetID::DEFAULT_TILESET);
    _player = _level.getPlayerId();
    _lMap = std::make_shared<FloatingPointLightMap>();
    _lMap->allocate(_level.getTilemapWidth(), _level.getTilemapHeight());
    _level.setLightMap(_lMap);

    initSystems();

    auto pRender = _ecs.get<RenderComponent>(_player);
    _cameraSystem.setCurrentCameraOffset(pRender.renderQuad.x + pRender.renderQuad.w / 2 - getGameSize().x / 2,
        pRender.renderQuad.y + pRender.renderQuad.h / 2 - getGameSize().y / 2);
    _cameraSystem.setCameraGoal(_player);

    return true;
}

void GameState::tick(float timescale) {
    // Logic goes here BEFORE input updates!
    _scriptSystem.update(_ecs, getAudioPlayer(), timescale);

    _inputSystem.update(_ecs);

    _physicsSystem.updateX(_ecs, timescale);
    _collisionSystem.updateLevelCollisionsOnXAxis(_ecs, _level);
    _physicsSystem.updateY(_ecs, timescale);
    _collisionSystem.updateLevelCollisionsOnYAxis(_ecs, _level);

    _collisionSystem.checkForMiningCollisions(_ecs);
    _collisionSystem.checkForItemPickupCollisions(_ecs, timescale, getAudioPlayer());
    _collisionSystem.checkForTorchAndBeaconCollisions(_ecs);
    _collisionSystem.checkForPlayerAndEnemyCollisions(_ecs, timescale, getAudioPlayer());

    _lightSystem.update(_ecs, _level);
    _inventorySystem.update(_ecs);

    _cameraSystem.update(_ecs, timescale);
    _renderOffset = _cameraSystem.getCurrentCameraOffset();

    _renderSystem.update(_ecs, timescale);

    respawnUpdate();

    auto& playerComp = _ecs.get<PlayerComponent>(_player);
    if(_beaconTextDisplayTimer < _beaconTextDisplayDuration && playerComp.beaconsLit < _level.getNumOfBeacons()) {
        _beaconTextDisplayTimer += timescale * 1000.f;
    }

    if(playerComp.beaconRecentlyLit) {
        playerComp.beaconRecentlyLit = false;
        _beaconTextDisplayTimer = 0;
    }

    // Input updates
    getKeyboard()->updateInputs();
    getController()->updateInputs();
}

void GameState::render() {
    SDL_SetRenderDrawColor(getRenderer(), 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(getRenderer());

    _level.render(_renderOffset);

    _renderSystem.render(getRenderer(), _ecs, _renderOffset);

    _inventorySystem.render(_ecs, getText(TextSize::TINY));

    // respawn text
    int playerHealth = _ecs.get<HealthComponent>(_player).health;
    if(playerHealth <= 0) {
        std::shared_ptr<Text> smallText = getText(TextSize::SMALL);
        smallText->setString("Press '" + getSettings()->getStringKeyboardControlForInputEvent(InputEvent::RESPAWN) + "' to respawn");
        smallText->render(getGameSize().x / 2 - smallText->getWidth() / 2 + 1, getGameSize().y / 2 - smallText->getHeight() / 2 + 1, 0, 0, 0);
        smallText->render(getGameSize().x / 2 - smallText->getWidth() / 2, getGameSize().y / 2 - smallText->getHeight() / 2);
    }
    
    // health bar
    Spritesheet* heart = SpritesheetRegistry::getSpritesheet(SpritesheetID::HEART);
    for(int i = 0; i < 3; ++i) {
        int ySpritesheetIndex = (i < playerHealth) ? 1 : 0;
        heart->setTileIndex(0, ySpritesheetIndex);
        heart->render(getGameSize().x - (heart->getWidth() + 2) * (3 - i), 2, 16, 16);
    }

    // beacon progress text
    if(_beaconTextDisplayTimer < _beaconTextDisplayDuration) {
        std::shared_ptr<Text> smallText = getText(TextSize::SMALL);
        auto numBeaconsLit = _ecs.get<PlayerComponent>(_player).beaconsLit;
        if(numBeaconsLit == _level.getNumOfBeacons()) {
            smallText->setString("All beacons lit! Congratulations.");
        }
        else {
            smallText->setString(std::to_string(numBeaconsLit) + " out of " + std::to_string(_level.getNumOfBeacons()) + " beacons lit");
        }
        smallText->render(getGameSize().x / 2 - smallText->getWidth() / 2 + 1, getGameSize().y - smallText->getHeight() - 2, 0, 0, 0);
        smallText->render(getGameSize().x / 2 - smallText->getWidth() / 2, getGameSize().y - smallText->getHeight() - 3);
    }

    // debug
    if(_debug) {
        auto view = _ecs.view<CollisionComponent>();
        SDL_SetRenderDrawColor(getRenderer(), 0x20, 0xFF, 0x20, 0xAF);
        for(auto ent : view) {
            auto collision = _ecs.get<CollisionComponent>(ent).collisionRect;
            SDL_FRect rect = {collision.x + _renderOffset.x, collision.y + _renderOffset.y, collision.w, collision.h};
            SDL_RenderFillRectF(getRenderer(), &rect);
        }
    }

    SDL_RenderPresent(getRenderer());
}

void GameState::handleControllerButtonInput(SDL_Event e) {
    getController()->updateButtonInputs(e);
}

void GameState::handleControllerAxisInput(SDL_Event e) {
    getController()->updateAxisInputs(e);
}

void GameState::handleMouseInput(SDL_Event e) {
    getMouse()->updateInput(e, _renderOffset.x, _renderOffset.y);
}

void GameState::initSystems() {
    _inputSystem.init(getKeyboard(), getController(), getSettings());
    
    _renderSystem.setRenderBounds(getGameSize());
    
    _cameraSystem.setGameSize(getGameSize().x, getGameSize().y);
    _cameraSystem.setLevelSize(_level.getTilemapWidth() * _level.getTileSize(), _level.getTilemapHeight() * _level.getTileSize());

    _lightSystem.updateLightSources(_ecs, _level);
}

void GameState::respawnUpdate() {
    auto& playerComp = _ecs.get<PlayerComponent>(_player);
    if(playerComp.requestsRespawn) {
        playerComp.requestsRespawn = false;
        auto& health = _ecs.get<HealthComponent>(_player);
        if(health.health <= 0) {
            health.health = 3;
            auto& transform = _ecs.get<TransformComponent>(_player);
            transform.position = _level.getPlayerSpawn();
            _lightSystem.update(_ecs, _level);
        }
    }
}