#pragma once

#include "Entity.h"

#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
    EntityVec m_entities; // all entities
    EntityVec m_entitiesToAdd; // antities to add next update
    std::map<std::string, EntityVec> m_entityMap; // map from entity tag to vectors
    size_t m_totalEntities = 0; // total entities created

    // helper funzion to avoid repeated code
    void removeDeadEntities(EntityVec & vec);

public:
    EntityManager();
    
    void update();
};