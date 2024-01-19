#include "Assets.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

Assets::Assets() {
    // Qui potresti caricare i tuoi font e le tue animazioni
}

const sf::Font & Assets::getFont(const std::string & font) const {
    // Qui dovresti restituire il font richiesto dal tuo storage di font
    // Per ora, restituirò solo un font vuoto
    static sf::Font emptyFont;
    return emptyFont;
}

const Animation & Assets::getAnimation(const std::string & name) const {
    // Qui dovresti restituire l'animazione richiesta dal tuo storage di animazioni
    // Per ora, restituirò solo un'animazione vuota
    static Animation emptyAnimation;
    return emptyAnimation;
}