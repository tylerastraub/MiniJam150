#include "InventorySystem.h"
#include "InventoryComponent.h"
#include "SpritesheetRegistry.h"
#include "PowerupComponent.h"

void InventorySystem::update(entt::registry& ecs) {
    auto view = ecs.view<InventoryComponent, PowerupComponent>();
    for(auto ent : view) {
        auto inventory = ecs.get<InventoryComponent>(ent).inventory;
        auto& powerup = ecs.get<PowerupComponent>(ent);
        int numCobalt = inventory[ItemType::COBALT];
        int numTopaz = inventory[ItemType::TOPAZ];
        powerup.brightnessBoost = COBALT_BRIGHTNESS_BOOST * numCobalt + TOPAZ_BRIGHTNESS_BOOST * numTopaz;
        powerup.falloffBoost = COBALT_FALLOFF_BOOST * numCobalt + TOPAZ_FALLOFF_BOOST * numTopaz;
    }
}

void InventorySystem::render(entt::registry& ecs, std::shared_ptr<Text> text) {
    auto view = ecs.view<InventoryComponent>();
    Spritesheet* ss = SpritesheetRegistry::getSpritesheet(SpritesheetID::INVENTORY_ICONS);
    for(auto ent : view) {
        auto inventory = ecs.get<InventoryComponent>(ent).inventory;
        int xPadding = 0;
        for(const auto& [itemType, quantity] : inventory) {
            ss->setTileIndex(0, getYSpritesheetIndex(itemType));
            ss->render(20 * xPadding + 1, 3, 16, 16);

            text->setString("x" + std::to_string(quantity));
            text->render(20 * xPadding + 11, 13, 0, 0, 0);
            text->render(20 * xPadding + 10, 12);

            ++xPadding;
        }
    }
}

int InventorySystem::getYSpritesheetIndex(ItemType itemType) {
    switch(itemType) {
        case ItemType::COBALT:
            return 0;
        case ItemType::TOPAZ:
            return 1;
        default:
            break;
    }
    return -1;
}