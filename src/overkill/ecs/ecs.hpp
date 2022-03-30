#pragma once

#include <memory>
#include <bitset>
#include <array>
#include <set>
#include <queue>
#include <unordered_map>
#include <cassert>

// INSPIRED BY https://austinmorlan.com/posts/entity_component_system/#the-component

namespace OK::ECS
{

	using Entity = std::uint32_t;			// MAX 4_294_967_296 ENTITIES
	using ComponentType = std::uint16_t;	// MAX 65_536 COMPONENT TYPES
	constexpr const Entity MAX_ENTITIES = 128;
	constexpr const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
	
	class System
	{
	public:
		std::set<Entity> mEntities;
	};

	namespace Detail
	{
		class IComponentList
		{
		public:
			virtual ~IComponentList() = default;
			virtual void onDestroy(Entity entity) = 0;
		};
	}

	template<typename T>
	class ComponentList final : public Detail::IComponentList
	{
	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		std::array<T, MAX_ENTITIES> mComponents;
		std::unordered_map<Entity, size_t> mEntityToIndexMap;
		std::unordered_map<size_t, Entity> mIndexToEntityMap;

		// Total size of valid entries in the array.
		size_t mSize;

		T& insert(Entity entity, T component)
		{
			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Entity already has this component!");
			
			mEntityToIndexMap[entity] = mSize;
			mIndexToEntityMap[mSize] = entity;
			mComponents[mSize] = component;
			mSize++;
			return component;
		}
		template<typename... Args>
		T& insert(Entity entity, Args&&... ctorArgs)
		{
			assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Entity already has this component!");

			mEntityToIndexMap[entity] = mSize;
			mIndexToEntityMap[mSize] = entity;
			T component = T(std::forward(ctorArgs)...);
			mComponents[mSize] = component;
			mSize++;
			return component
		}
		T& get(Entity entity)
		{
			return mComponents[mEntityToIndexMap[entity]];
		}
		void remove(Entity entity)
		{
			assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Entity does not have this component!");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
            mComponents[indexOfRemovedEntity] = mComponents[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
			mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			mEntityToIndexMap.erase(entity);
			mIndexToEntityMap.erase(indexOfLastElement);

			--mSize;
		}

		virtual void onDestroy(Entity entity) override
		{
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				// Remove the entity's component if it existed
				remove(entity);
			}
		}
	};


	class EntityManager
	{
	private:
		std::queue<Entity> mAvailableEntities;
		std::array<Signature, MAX_ENTITIES> mSignatures;
		uint32_t mActiveEntities;

	public:
		EntityManager();
		Entity create();
		void destroy(Entity entity);
		void setSignature(Entity entity, Signature signature);
		Signature getSignature(Entity entity);
	};


	class ComponentManager
	{
	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> mComponentTypes;
		std::unordered_map<const char*, std::shared_ptr<Detail::IComponentList>> mComponentLists;
		ComponentType mNextComponentType;

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentList<T>> getComponentList()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use!");
			return std::static_pointer_cast<ComponentList<T>>(mComponentLists[typeName]);
		}
	public:
		template<typename T>
		void registerComponent()
		{
			const char* typeName = typeid(T).name();
			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Component already registered!");

			mComponentTypes.insert({ typeName, mNextComponentType++ });
            mComponentLists.insert({ typeName, std::make_shared<ComponentList<T>>() });
		}

		template<typename T>
		ComponentType getType()
		{
			const char* typeName = typeid(T).name();
			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use!");
			return mComponentTypes[typeName];
		}
		template<typename T>
		T& addComponent(Entity entity, T component)
		{
			return getComponentList<T>()->insert(entity, component);
			
		}
		template<typename T, typename... Args>
		T& addComponent(Entity entity, Args&&... ctorArgs)
		{
			return getComponentList<T>()->insert(entity, ctorArgs);
		}
		template<typename T>
		void removeComponent(Entity entity) 
		{
			getComponentList<T>()->remove(entity);
		}
		template<typename T>
		T& getComponent(Entity entity)
		{
			getComponentList<T>()->get(entity);
		}

		void entityDestroyed(Entity entity)
		{
			for (const auto& nameAndListPair : mComponentLists)
				nameAndListPair.second->onDestroy(entity);
		}
	};

	class SystemManager
	{
	private:
		std::unordered_map<const char*, Signature> mSignatures;
		std::unordered_map<const char*, std::shared_ptr<System>> mSystems;

	public:
		template<typename T>
		std::shared_ptr<T> registerSystem()
		{
			const char* typeName = typeid(T).name();
			assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
            mSystems.emplace(std::make_pair( typeName, system ));
			return system;
		}

		template<typename T>
		void setSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");
			// Set the signature for this system
			mSignatures.insert({ typeName, signature });
		}

		void entityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists
			// mEntities is a set so no check needed
			for (auto const& pair : mSystems)
			{
				auto const& system = pair.second;

				system->mEntities.erase(entity);
			}
		}

		void entitySignatureChanged(Entity entity, Signature entitySignature)
		{
			// Notify each system that an entity's signature changed
			for (auto const& pair : mSystems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = mSignatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->mEntities.insert(entity);
				}
				// Entity signature does not match system signature - erase from set
				else
				{
					system->mEntities.erase(entity);
				}
			}
		}
	};

    class Coordinator
    {
    public:
        void Init()
        {
            // Create pointers to each manager
            mComponentManager = std::make_unique<ComponentManager>();
            mEntityManager = std::make_unique<EntityManager>();
            mSystemManager = std::make_unique<SystemManager>();
        }


        // Entity methods
        Entity CreateEntity()
        {
            return mEntityManager->create();
        }

        void DestroyEntity(Entity entity)
        {
            mEntityManager->destroy(entity);

            mComponentManager->entityDestroyed(entity);

            mSystemManager->entityDestroyed(entity);
        }


        // Component methods
        template<typename T>
        void RegisterComponent()
        {
            mComponentManager->registerComponent<T>();
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            mComponentManager->addComponent<T>(entity, component);

            auto signature = mEntityManager->getSignature(entity);
            signature.set(mComponentManager->getType<T>(), true);
            mEntityManager->setSignature(entity, signature);

            mSystemManager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            mComponentManager->removeComponent<T>(entity);

            auto signature = mEntityManager->getSignature(entity);
            signature.set(mComponentManager->getType<T>(), false);
            mEntityManager->setSignature(entity, signature);

            mSystemManager->entitySignatureChanged(entity, signature);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return mComponentManager->getComponent<T>(entity);
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            return mComponentManager->getType<T>();
        }


        // System methods
        template<typename T>
        std::shared_ptr<T> RegisterSystem(Signature& signature)
        {
            std::cout << "a\n";
            auto registeredSystem = mSystemManager->registerSystem<T>();
            mSystemManager->setSignature<T>(signature);
            return registeredSystem;
        }
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            std::cout << "b\n";
            return mSystemManager->registerSystem<T>();
        }

        template<typename T, typename TComp, typename... TCompArgs>
        std::shared_ptr<T> RegisterSystem(Signature& signature)
        {
            std::cout << "c\n";
            signature.set(GetComponentType<TComp>());
            return RegisterSystem<T, TCompArgs...>(signature);
        }

        template<typename T, typename TComp, typename... TCompArgs>
        std::shared_ptr<T> RegisterSystem(TComp component, TCompArgs... components)
        {
            std::cout << "d\n";
            return RegisterSystem<T, TComp, TCompArgs...>(Signature());
        }

        template<typename T>
        void SetSystemSignature(Signature signature)
        {
            mSystemManager->SetSignature<T>(signature);
        }

    private:
        std::unique_ptr<ComponentManager> mComponentManager;
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<SystemManager> mSystemManager;
    };
}