#include "GameState.h"
#include "RandomGen.h"
#include "SpritesheetRegistry.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
// Prefabs
#include "Player.h"

#include <chrono>

std::mt19937 RandomGen::randEng{(unsigned int) std::chrono::system_clock::now().time_since_epoch().count()};

bool GameState::init() {
    initSystems();

    _player = prefab::Player::create(_ecs, {64, 64});
    auto pRender = _ecs.get<RenderComponent>(_player);
    _cameraSystem.setCurrentCameraOffset(pRender.renderQuad.x + pRender.renderQuad.w / 2 - getGameSize().x / 2,
        pRender.renderQuad.y + pRender.renderQuad.h / 2 - getGameSize().y / 2);
    _cameraSystem.setCameraGoal(_player);

    _level.allocateTilemap(20, 12);
    _level.setTileset(SpritesheetRegistry::getSpritesheet(SpritesheetID::DEFAULT_TILESET));
    _level.setPlayerId(_player);
    _level.setTileSize(16);

    Tile g = {TileType::SOLID, {0, 0, 16, 16}};
    Tile e = {TileType::NOVAL, {0, 0, 0, 0}};
    _level.setTilemap({
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, g, g, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, g, g, g, g, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e, e},
        {g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g},
        {g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g, g}
    });

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
    SDL_SetRenderDrawColor(getRenderer(), 0x10, 0x10, 0x10, 0xFF);
    SDL_RenderClear(getRenderer());

    _level.render(_renderOffset);

    _renderSystem.render(getRenderer(), _ecs, _renderOffset);

    // debug
    // auto collision = _ecs.get<CollisionComponent>(_player).collisionRect;
    // SDL_FRect pRect = {collision.x + _renderOffset.x, collision.y + _renderOffset.y, collision.w, collision.h};
    // SDL_SetRenderDrawColor(getRenderer(), 0x20, 0xFF, 0x20, 0xAF);
    // SDL_RenderFillRectF(getRenderer(), &pRect);

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
}