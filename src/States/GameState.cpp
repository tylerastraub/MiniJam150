#include "GameState.h"
#include "RandomGen.h"
#include "SpritesheetRegistry.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "MiningComponent.h"
#include "LevelParser.h"
// Prefabs
#include "Player.h"

#include <chrono>

std::mt19937 RandomGen::randEng{(unsigned int) std::chrono::system_clock::now().time_since_epoch().count()};

/**
 * @todo
 * === 1/20/24 ===
 * - Add basic enemies
 *     - Big slug (basic, slow, weak enemy)
 *     - Mimic (fake rock enemy)
 *     - Jumping enemy (frog?)
 * - Add health system
 * - Add torches
 * - Add light beacons
 * - Add rose quartz mineral
 * === 1/21/24 ===
 * - Create giant metroidvania style level but rocks/enemies are randomly generated
 *     - Light beacons/torches placed manually
 * - Add beacon progress bar (or cave light up progress bar)
 * - Add sounds
 * - Add wizard lab
 * - Add music
 * - Add more extra art :)
 * - L canceling
*/

bool GameState::init() {
    _level = LevelParser::parseLevelFromTmx(_ecs, "res/tiled/test_level.tmx", SpritesheetID::DEFAULT_TILESET);
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

    _lightSystem.update(_ecs, _level);
    _inventorySystem.update(_ecs);

    _cameraSystem.update(_ecs, timescale);
    _renderOffset = _cameraSystem.getCurrentCameraOffset();

    _renderSystem.update(_ecs, timescale);

    auto pTransform = _ecs.get<TransformComponent>(_player);
    auto pPhysics = _ecs.get<PhysicsComponent>(_player);

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