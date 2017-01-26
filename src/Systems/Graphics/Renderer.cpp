/*
 * Renderer.cpp
 *
 *  Created on: 2012-12-02
 *      Author: Nathan
 */

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Renderer.h"
#include "Shaders/shaders.h"

Shader* shader;
glm::mat4 projection;

Renderer::Renderer(int width, int height)
    : width(width), height(height) {

    // Load shader
    loadShader("");
    useShader(0);

    uniforms.projection = setUniform("uprojection");
    uniforms.camera = setUniform("ucamera");
    uniforms.model = setUniform("umodel");
    uniforms.color = setUniform("ucolor");

    matrixStack.push_back(glm::mat4(1.0f));
    setProjection(width, height);
    setCamera(0, 0, 0);
}

void Renderer::loadShader(const std::string &name){
    shader = new Shader("res/shaders/vert.glsl", "res/shaders/frag.glsl", width, height);
}

glm::vec3 Renderer::unProject(int x, int y, double z){
    glm::ivec4 viewport;
    glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport));
    return glm::unProject(glm::vec3(x, y, z), currentMatrix(), projection, viewport);
}

void Renderer::setColor(const glm::vec4 &color){
    glUniform4fv(uniforms.color, 1, glm::value_ptr(color));
}

void Renderer::setColor(float r, float g, float b, float a){
    glUniform4f(uniforms.color, r, g, b, a);
}

glm::vec3 Renderer::getCursorPos(int x, int y){
    pushMatrix();
    glm::vec3 p0 = unProject(x, y, 0.0);
    glm::vec3 p1 = unProject(x, y, 1.0);
    float u = -p0.z/(p1.z - p0.z);
    popMatrix();
    return p0 + (p1 - p0)*u;
}

void Renderer::loadFont(const std::string &name){
    _font.reset(new Font(name));
}

void Renderer::bindTexture(int texture){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uniforms.texture, 0);
}

void Renderer::renderString(glm::vec3 pos, const std::string &text){
    pushMatrix();
    for(unsigned i = 0; i < text.size(); ++i){
        Glyph g = _font->getGlyph(text[i]);
        float x = pos.x + g.left;
        float y = pos.y + g.top;
        float w = g.w;
        float h = g.h;
        glEnable(GL_TEXTURE);
        bindTexture(g.texture);
        glBegin(GL_QUADS);
        glVertex3f(x - w, y - h, 0);
        glVertex3f(x + w, y - h, 0);
        glVertex3f(x - w, y + h, 0);
        glVertex3f(x + w, y + h, 0);
        glEnd();
        //          glBufferData(GL_ARRAY_BUFFER, sizeof (box), box, GL_DYNAMIC_DRAW);
        //          glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        pos.x += (g.adv);
    }
    popMatrix();
}

#define NEAR_PLANE 0.0625f
#define FAR_PLANE 256.0f

glm::mat4 Renderer::setProjection(int width, int height){
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projection = glm::perspective(glm::radians(45.f), aspectRatio, NEAR_PLANE, FAR_PLANE);
    glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));
    return projection;
}

void Renderer::uploadModelView(){
    glUniformMatrix4fv(uniforms.model, 1, GL_FALSE, glm::value_ptr(currentMatrix()));
}

void Renderer::setCamera(float x, float y, float z){
    camera = glm::translate(glm::mat4(1.f), -glm::vec3(x, y, z));
    glUniformMatrix4fv(uniforms.camera, 1, GL_FALSE, glm::value_ptr(camera));
}

void Renderer::pushMatrix(){
    matrixStack.push_back(matrixStack.back());
    uploadModelView();
}

void Renderer::popMatrix(){
    if(matrixStack.size() > 1) {
        matrixStack.pop_back();
        uploadModelView();
    }
}

glm::mat4 Renderer::translate(const glm::vec3 &pos){
    glm::mat4 t = glm::translate(glm::mat4(1.f), pos);
    matrixStack.back() = matrixStack.back() * t;
    uploadModelView();
    return matrixStack.back();
}

glm::mat4 Renderer::rotate(const glm::vec3 &from, const glm::vec3 &to){
    auto q = glm::quat( from, to );
    auto m = glm::toMat4( q );
    matrixStack.back() = matrixStack.back() * m;
    uploadModelView();
    return matrixStack.back();
}

glm::mat4 Renderer::rotate(float angle, float x, float y, float z){
    glm::mat4 t = glm::rotate(glm::mat4(1.f), angle, glm::vec3(x, y, z));
    matrixStack.back() = matrixStack.back() * t;
    uploadModelView();
    return matrixStack.back();
}

glm::mat4 Renderer::scale(const glm::vec3 &scale){
    glm::mat4 t = glm::scale(
            glm::mat4(1.0f),
            scale);
    matrixStack.back() = matrixStack.back() * t;
    uploadModelView();
    return matrixStack.back();
}

glm::mat4& Renderer::currentMatrix(){
    return matrixStack.back();
}

void Renderer::loadIdentity(){

}

GLuint Renderer::setUniform(const std::string &name){
    return shader->setUniform(name);
}

void Renderer::useShader(int id){
    shader->enable();
}

void Renderer::disableShader(){
    shader->disable();
}


Renderer::~Renderer() {}
