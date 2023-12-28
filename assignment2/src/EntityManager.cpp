#include "EntityManager.h"

EntityManager::EntityManager()
{
    m_entities = EntityVec();
}

void EntityManager::update()
{
    for (auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->m_tag].push_back(e);
    }

    m_entitiesToAdd.clear();    

    removeDeadEntities(m_entities);

    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

bool isEntityDead(const std::shared_ptr<Entity> & e)
{
    return !e->isActive();
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    // move inactive elements to the end of the vector then deletes them 
    vec.erase(
        std::remove_if(
            vec.begin(), 
            vec.end(), 
            [](const std::shared_ptr<Entity> & e) { return !e->isActive(); }
        ), 
        vec.end()
    );
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);

    return entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    return m_entityMap[tag];
}