#pragma once
#include <unordered_map>
#include <sol.hpp>
#include "Entity.hpp"

class EntityManager {
    public:
        EntityManager(sol::state &lua) : _lua(lua), _entityCounter(0) {
                _lua.open_libraries();
                _lua.new_usertype<Entity>("Entity",
                        "getName", &Entity::getType,
                        "setName", &Entity::setType,
                        "getId", &Entity::getId);

                _lua.do_file("config/safe_ref.lua");
        }

        Entity& createEntity() {
	    auto id = _entityCounter;
	    ++_entityCounter;
	    auto inserted = _entities.emplace(id, std::make_unique<Entity>(id));
	    auto it = inserted.first; // iterator to created id/Entity pair
	    auto& e = *it->second; // created entity
	    _lua["createHandle"](e);
	    return e;
	}
        void removeEntity(const EntityId id) {
	    _lua["onEntityRemoved"](id);
            _entities.erase(id);
        }
    private:
        std::unordered_map<EntityId, std::unique_ptr<Entity>> _entities;
        EntityId _entityCounter;
        sol::state &_lua;
};
