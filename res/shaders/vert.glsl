#version 330

layout(location = 0)in vec4 vert;

uniform mat4 uprojection;
uniform mat4 umodelview;
uniform vec2 uoffset;

void main()
{
    gl_Position = uprojection * umodelview * vert;
}
