#pragma once

#include "Animation.h"
#include <SFML/Graphics.hpp>

class Assets {
public:
    Assets();

    void loadFromFile(const std::string & path);
    const sf::Font & getFont(const std::string & font) const;
    const Animation & getAnimation(const std::string & name) const;

};