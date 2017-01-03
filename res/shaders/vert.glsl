#version 330

layout(location = 0)in vec4 position;

uniform mat4 uprojection;
uniform mat4 umodelview;

void main()
{
    gl_Position = uprojection * umodelview * position;
}
