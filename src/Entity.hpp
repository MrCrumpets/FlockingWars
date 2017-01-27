#pragma once
#include <string>
#include <typeindex>
#include <map>
#include <glm/glm.hpp>
#include "Systems/Graphics/Mesh.hpp"
#include "Systems/Graphics/Renderer.h"

using EntityId = uint32_t;

class Component {
    public:
        virtual ~Component(){}
};

class System {
    public:
        virtual void update(float dt) = 0;
        virtual Component* getComponent(EntityId = -1) = 0;
        virtual ~System() {}
};


template <typename T>
class SystemImpl : public System{
    public:
        virtual void update(float dt) = 0;
        Component* getComponent(EntityId = -1) {
            if(_avail_component >= _components.size()) {
                _components.emplace_back();
                ++_avail_component;
            }
            return &_components[_avail_component++];
        }
        virtual ~SystemImpl() {}
    protected:
        size_t _avail_component;
        std::vector<T> _components;
};

class PhysicsComponent : public Component {
    friend class PhysicsSystem;
    glm::vec3 pos, vel, acc;
    glm::vec3 orientation;
    float mass; // kg
};

class PhysicsSystem : public SystemImpl<PhysicsComponent> {
    static constexpr float decay = 0.9f;
    static constexpr float max_velocity = 200.f; // m/s
    public:
        void update(float dt) {
            for(auto &c : _components) {
              c.vel += c.acc * dt;
              c.pos += c.vel * dt;

              c.vel *= decay;
              c.acc *= decay;

              if(glm::length(c.vel) > max_velocity){
                  c.vel = glm::normalize(c.vel) * max_velocity;
              }
            }
        }
};

class GraphicsComponent : public Component {
    friend class GraphicsSystem;
    Mesh _mesh;
    glm::vec4 color = { 129.f / 255.f, 204.f / 255.f, 60.f / 255.f, 1.f };
};

class GraphicsSystem : public SystemImpl<GraphicsComponent> {
    public:
        GraphicsSystem(int width, int height) : _r(std::make_unique<Renderer>(width, height)) {
            _r->setCamera(0.5f, 0.5f, 10.f);
        }

        void update(float dt) {
            glClearColor(0.f,0.f,0.f,1.f);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glClear(GL_COLOR_BUFFER_BIT);

            _r->setCamera(0.5f, 0.5f, 10.f);
            for(auto &c : _components) {
                _r->pushMatrix();
                    _r->translate( {0.5f, 0.5f, 0.f} );
                    _r->scale( {4.f, 4.f, 4.f} );
                    _r->setColor(c.color);
                    c._mesh.draw();
                _r->popMatrix();
            }
        }

    private:
        std::unique_ptr<Renderer> _r;
};

class Entity {
    public:
        explicit Entity(EntityId id) : _id(id) {}

        template <typename T>
        void addComponent(Component *c) {
            _components[std::type_index(typeid(T))] = c;
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
        }

    private:
        EntityId _id;
        std::string _type;
        std::map<std::type_index, Component*> _components;
};
