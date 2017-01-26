#pragma once
#include <string>
#include <typeindex>
#include <map>
#include <glm/glm.hpp>
#include "Systems/Graphics/Mesh.hpp"

class Component {
    public:
        virtual ~Component(){}
};

class PhysicsComponent : Component {
    glm::vec3 pos, vel, acc;
    glm::vec3 orientation;
};

class GraphicsComponent : Component {
    Mesh _mesh;
    glm::vec3 scale;
};

using EntityId = uint32_t;

class Entity {
    public:
        explicit Entity(EntityId id) : _id(id) {}

        void addComponent(const std::type_index &t, Component *c) {
            _components[t] = c;
        }

        template <typename T>
        T* getComponent() {
            auto c = _components.find(std::type_index(typeid(T)));
            return c != _components.end() ? dynamic_cast<T*>(c->second) : nullptr;
        }

        std::string getType() const {
            return _type;
        }
        
        void setType(const std::string &type) {
            _type = type;
        }

        EntityId getId() const {
            return _id;
        }

        ~Entity() {
            for(auto &c : _components) {
                delete c.second;
            }
        }

    private:
        EntityId _id;
        std::string _type;
        std::map<std::type_index, Component*> _components;
};
