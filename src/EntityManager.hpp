#pragma once
#include <unordered_map>
#include <sol.hpp>
#include "Entity.hpp"

class EntityManager {
    public:
        EntityManager() : _entityCounter(0) {}

        Entity& createEntity() {
            auto id = _entityCounter++;
            auto pair = _entities.emplace(id, std::make_unique<Entity>(id));
            auto& entity = *(pair.first)->second;
            _lua["createHandle"](entity);
            return entity;
        }
        void removeEntity(const EntityId id) {
	    _lua["onEntityRemoved"](id);
            _entities.erase(id);
        }
    private:
        std::unordered_map<EntityId, std::unique_ptr<Entity>> _entities;
        EntityId _entityCounter;
        sol::state _lua;
};
