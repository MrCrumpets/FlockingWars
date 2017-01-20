/*
 * Explosio.cpp
 *
 */

#include "Explosion.h"
#include <glm/gtc/random.hpp>

std::vector<glm::vec3> colors {
    {181, 40, 65}, 
    {255, 192, 81}, 
    {255, 137, 57}, 
    {232, 95, 77}, 
    {89, 0, 81}
};

static const int numParticles = 180;
static const float radius = 15.f;
static GLuint _vertex_array;
static GLuint _vertex_buffer;

Explosion::Explosion(Renderer *r, glm::vec3 pos)
    : Entity(r, pos), _energy(10), _numDead(0) {
    type = EntityType::Explosion;
    team = "none";

    std::vector<glm::uint32> indices;
    for(size_t i = 0; i < numParticles; ++i){
        indices.push_back(indices.size());
        indices.push_back(indices.size());
        particles.emplace_back(Particle{glm::vec3(), glm::ballRand(radius), 100.f});
        _vertices.push_back({particles.back().last_pos});
        _vertices.push_back({particles.back().pos});
    }

    _mesh = std::make_unique<Mesh>(_vertices, indices, DrawType::Lines, MeshType::Streaming);
}
/**
 * The update method updates the all the particles based on the dt since last update, and removes
 * dead particles
 *
 * @param dt
 *              The time in seconds since update was last called
 */
void Explosion::update(float dt){
    if(particles.size() == _numDead){
        dead = true;
        return;
    }

    int line_idx = 0;
    for(auto &p : particles){
        auto vel = p.pos - p.last_pos;
        p.last_pos = p.pos;
	p.pos += 0.99f * vel * dt;
        p.energy -= dt;
        if(p.energy < 0 && !dead) {
            dead = true;
            _numDead++;
        }
        _vertices[line_idx++] = { p.last_pos };
        _vertices[line_idx++] = { p.pos };
    }
    _mesh->update(_vertices);
}

void Explosion::render(){
    _mesh->draw();
}

/**
 * Destructor, frees all the memory used by Particle objects and the particles[] array
 */
Explosion::~Explosion() {
}
