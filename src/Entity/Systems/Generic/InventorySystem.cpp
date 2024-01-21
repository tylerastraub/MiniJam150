#include "InventorySystem.h"
#include "InventoryComponent.h"
#include "SpritesheetRegistry.h"

void InventorySystem::render(entt::registry& ecs, std::shared_ptr<Text> text) {
    auto view = ecs.view<InventoryComponent>();
    Spritesheet* ss = SpritesheetRegistry::getSpritesheet(SpritesheetID::INVENTORY_ICONS);
    for(auto ent : view) {
        auto inventory = ecs.get<InventoryComponent>(ent).inventory;
        int xPadding = 0;
        for(const auto& [itemType, quantity] : inventory) {
            ss->setTileIndex(0, getYSpritesheetIndex(itemType));
            ss->render(24 * xPadding + 1, 3, 16, 16);

            text->setString("x" + std::to_string(quantity));
            text->render(24 * xPadding + 11, 12, 0, 0, 0);
            text->render(24 * xPadding + 10, 11);

            ++xPadding;
        }
    }
}

int InventorySystem::getYSpritesheetIndex(ItemType itemType) {
    switch(itemType) {
        case ItemType::COBALT:
            return 0;
        default:
            break;
    }
    return -1;
}