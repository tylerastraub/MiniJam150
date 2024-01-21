#pragma once

#include "Tile.h"
#include "Spritesheet.h"
#include "FloatingPointLightMap.h"

#include <vector>
#include <cstdint>
#include <memory>
#include <entt/entity/registry.hpp>

class Level {
public:
    Level() = default;
    ~Level() = default;

    void allocateTilemap(int width, int height);
    void render(strb::vec2f renderOffset = {0.f, 0.f});

    void setTilemap(std::vector<std::vector<Tile>> tilemap);
    void setLightMap(std::shared_ptr<FloatingPointLightMap> lightMap);
    void setTileSize(int tileSize);
    void setTileAt(int x, int y, Tile tile);
    void setTileset(Spritesheet* tileset);
    void setPlayerId(entt::entity player);
    void setPlayerSpawn(strb::vec2f spawn);

    Tile getTileAt(int x, int y);
    int getTileSize();
    int getTilemapWidth();
    int getTilemapHeight();
    entt::entity getPlayerId();
    std::shared_ptr<FloatingPointLightMap> getLightMap();
    strb::vec2f getPlayerSpawn();

private:
    std::shared_ptr<FloatingPointLightMap> _lMap = nullptr;
    std::vector<std::vector<Tile>> _tilemap;
    int _tilemapWidth = 0;
    int _tilemapHeight = 0;
    int _tileSize = 16;
    Spritesheet* _tileset = nullptr;

    entt::entity _playerId;
    strb::vec2f _playerSpawn = {-1.f, -1.f};

};