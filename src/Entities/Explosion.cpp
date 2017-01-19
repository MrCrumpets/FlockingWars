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

    for(size_t i = 0; i < numParticles; ++i){
        particles.emplace_back(Particle{glm::vec3(), glm::ballRand(radius)});
    }

    glGenVertexArrays(1, &_vertex_array);
    glBindVertexArray(_vertex_array);

    // Copy Vertex Buffer Data
    glGenBuffers(1, &_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
            particles.size() * sizeof(Particle),
            nullptr, GL_STREAM_DRAW);

    // Set Shader Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *) 0);
    glEnableVertexAttribArray(0); // Vertex Positions
}
/**
 * The update method updates the all the particles based on the dt since last update, and removes
 * dead particles
 *
 * @param dt
 *              The time in seconds since update was last called
 */
void Explosion::update(float dt){
    return;
    if(particles.size() == _numDead){
        dead = true;
        return;
    }

    for(auto &p : particles){
        auto vel = p.pos - p.last_pos;
        p.last_pos = p.pos;
	p.pos += 0.99f * vel * dt;
        _energy -= dt;
        if(_energy < 0) {
            dead = true;
            return;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Particle), &particles[0]);

}

void Explosion::render(){
    glBindVertexArray(_vertex_array);
    glDrawArrays(GL_LINES, 0, 2 * particles.size());
}

/**
 * Destructor, frees all the memory used by Particle objects and the particles[] array
 */
Explosion::~Explosion() {
}
