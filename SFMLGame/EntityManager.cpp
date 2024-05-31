#include "EntityManager.h"
#include <iostream>







EntityManager::EntityManager()
{
}




void EntityManager::update()
{
	//TODO: add entities rom ,_entitiesToAdd the proper location(s)
	//-add them to thevector of all entities
	//-add them to the vector inside the map, with the tag as a key
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e); 
		m_entityMap[e->tag()].push_back(e);		
	}
	m_entitiesToAdd.clear();
	//remove dead entities from the vector of all entities
	
	removeDeadEntities(m_entities);
	// remove dead entites fron each vector in the entity map
	//c++17 way of iterating through [key,value] pairs in a map
	for (auto& [tag,entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}



void EntityManager::removeDeadEntities(EntityVec & vec)
{
	for (auto& e : vec)	//for every entity in the vector collection
	{	
		vec.erase( //vector. erase
			std::remove_if(vec.begin(), vec.end(), [](auto const& entity) //remove if (scan all item from begining to end)
				{
					return !entity->isActive();   //read item's isActive() boolean value;      symbol"!" in here is means as "Not"  ::translate{ if this entity is not Active}
				}),
			vec.end() // apply until end of the collection
		);
		break;
	}	
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}


const EntityVec& EntityManager::getEntities(const std::string & tag)
{
	return m_entityMap[tag];
}

bool EntityManager::checkEntitiesExist(const std::string& tag)
{
	for (auto e : m_entities) {
		if (e->tag() == tag) {
			return true;
		}
	}
	return false;
}






