#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 velocity = { 0.0, 0.0 };
    float angle = 0;

    /// @brief constructor
    /// @param p position
    /// @param v velocity
    /// @param a angle
    CTransform(const Vec2 & p, const Vec2 & v, float a)
        : pos(p), velocity(v), angle(a) {}
};

class CShape
{
public:
    sf::CircleShape circle;

    /// @brief constructor
    /// @param radius radius
    /// @param points number of points
    /// @param fill fill color
    /// @param outline outline color
    /// @param thickness outline thickness
    CShape(float radius, int points, const sf::Color & fill, const sf::Color & outline, float thickness)
        : circle(radius, points)
    {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
        circle.setRadius(radius);
        circle.setPointCount(points);
    }
};

class CCollision
{
public:
    float radius = 0;

    /// @brief constructor
    /// @param r radius
    CCollision(float r)
        : radius(r) {}
};

class CScore
{
public:
    int score = 0;

    /// @brief constructor
    /// @param s score
    CScore(int s)
        : score(s) {}
};

class CLifespan
{
public:
    /// @brief remaining lifespan
    int remaining = 0;
    /// @brief total lifespan
    int total = 0;

    /// @brief constructor
    /// @param t total
    CLifespan(int t)
        : remaining(t), total(t) {}
};

class CInput
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool special = false;

    CInput() {}
};