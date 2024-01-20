#pragma once

#include "vec2.h"

#include <list>
#include <vector>
#include <cstdint>

#include "Hue.h"
#include "Light.h"

/**
 * @brief 2D lightmap used for creating static lighting map.
 * Lighting is calculated using a floating point position.
 */
class FloatingPointLightMap {
public:
    FloatingPointLightMap() = default;
    ~FloatingPointLightMap() = default;

    void allocate(int width, int depth);

    uint16_t addLightSource(Light light);
    void removeLightSource(uint16_t lightId);

    Hue calculateEntityHue(strb::vec2f pos);

    Hue getHue(strb::vec2f pos);
    int getBrightness(strb::vec2f pos);

    bool hasLight(uint16_t lightId);

private:
    bool isLightInBounds(strb::vec2f pos);
    void updateLightMap(Light light);
    void addLightToLightMap(strb::vec2f pos, float brightness, Hue hue);

    uint16_t _currentLightId = 0;

    std::list<Light> _lightSources;
    // std::pair<brightness, hue>
    std::vector<std::vector<std::pair<int, Hue>>> _lightMap;

    int _lightMapWidth = 0;
    int _lightMapDepth = 0;
};