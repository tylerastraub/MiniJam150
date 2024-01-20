#pragma once

#include "Tile.h"
#include "Level.h"
#include "SpritesheetID.h"
#include "PrefabType.h"
#include "Mineral.h"

#include <vector>
#include <string>

class LevelParser {
public:
    LevelParser() = default;
    ~LevelParser() = default;

    static Level parseLevelFromTmx(entt::registry& ecs, std::string filePath, SpritesheetID spritesheetId);

private:
    static PrefabType convertStringToPrefabType(std::string prefabTypeString);
    static MineralType convertStringToMineralType(std::string mineralTypeString);

};