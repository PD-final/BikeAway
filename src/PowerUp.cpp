#include "PowerUp.h"

void PowerUp::apply(Character& player) {
    switch (type) {
        case PowerUpType::Coffee:
            player.speed *= 1.5f;
            break;

        case PowerUpType::Beer:
            player.speed *= 0.7f;
            break;

        case PowerUpType::Shield:
            player.hearts += 1;
            break;

        case PowerUpType::StudentID:
            // TODO: unlock shortcut
            break;
    }
}
