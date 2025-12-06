#include "PowerUp.h"

#include "Character.h"

void PowerUp::apply(Character& player) {
    switch (type) {
        case PowerUpType::Coffee:
            player.speed *= 1.5f;
            break;

        case PowerUpType::Beer:
            player.speed *= 0.7f;
            break;

        case PowerUpType::Shield:
            // TODO: player.hasShield = true;
            break;

        case PowerUpType::StudentID:
            // TODO: unlock shortcut
            break;
    }
}
