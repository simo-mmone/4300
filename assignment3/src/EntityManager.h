#pragma once

#include "Entity.h"

#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities; // all entities
    EntityVec m_entitiesToAdd; // antities to add next update
    EntityMap m_entityMap; // map from entity tag to vectors
    size_t m_totalEntities = 0; // total entities created

    // helper funzion to avoid repeated code
    void removeDeadEntities(EntityVec & vec);

public:
    EntityManager();
    
    void update();

    std::shared_ptr<Entity> addEntity(const std::string & tag);

    const EntityVec & getEntities();
    const EntityVec & getEntities(const std::string & tag);
	const EntityMap & getEntityMap(); // not in all projects
};