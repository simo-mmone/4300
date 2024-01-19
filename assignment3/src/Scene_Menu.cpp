#include "Scene_Menu.h"
#include "GameEngine.h"
#include "Action.h"
#include <SFML/Graphics.hpp>

Scene_Menu::Scene_Menu(GameEngine * gameEngine) : Scene(gameEngine) {
    init();
}

void Scene_Menu::init() {
    // Qui dovresti inizializzare il tuo menu
}

void Scene_Menu::update() {
    // Qui dovresti aggiornare lo stato del tuo menu
}

void Scene_Menu::onEnd() {
    // Qui dovresti gestire la fine del tuo menu
}

void Scene_Menu::sDoAction(const Action & action) {
    // Qui dovresti gestire le azioni del tuo menu
}

void Scene_Menu::sRender() {
    // Qui dovresti renderizzare il tuo menu
}