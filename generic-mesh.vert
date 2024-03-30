#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_proj;
uniform mat4 u_view;

out vec3 v_realPosition;
out vec3 v_normal;

void main(){
    v_normal = a_normal;

    v_realPosition = a_position;

    gl_Position = u_proj * u_view * vec4(v_realPosition, 1.0);
}