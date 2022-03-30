
#include "ecs.hpp"

namespace OK::ECS 
{

EntityManager::EntityManager() : mAvailableEntities{}, mSignatures{}, mActiveEntities{ 0u }
{
	for (Entity i = 0; i < MAX_ENTITIES; i++)
		mAvailableEntities.push(i);
}

Entity
EntityManager::create() {
	assert(mActiveEntities < MAX_ENTITIES && "Max number of entities reached!");

	Entity entity = mAvailableEntities.front();
	mAvailableEntities.pop();
	mActiveEntities++;
	return entity;
}

void
EntityManager::destroy(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	mSignatures[entity].reset();
	mAvailableEntities.push(entity);
	mActiveEntities--;
}

void
EntityManager::setSignature(Entity entity, Signature signature)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	mSignatures[entity] = signature;
}
Signature
EntityManager::getSignature(Entity entity)
{
	assert(entity < MAX_ENTITIES && "Entity out of range.");
	return mSignatures[entity];
}
}