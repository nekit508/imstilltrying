#version 330 core

in vec3 v_realPosition;
in vec3 v_normal;

uniform struct lighting {
    vec3 ambientColor;

    vec3 sunColor;
    vec3 sunVector;
} u_light;
uniform vec3 u_playerPos;

out vec4 color;

void main(){
    vec4 textureColor = vec4(0.5, 0.5, 0.5, 1.0);

    // diffuse color
    float diffCoef = max(dot(v_normal, u_light.sunVector), 0.0);

    // specular color
    vec3 mid = normalize(u_light.sunVector + normalize(u_playerPos - v_realPosition.xyz));
    float specCoef = pow(max(dot(v_normal, mid), 0.0), 64);

    vec4 fc = textureColor * vec4(u_light.ambientColor + u_light.sunColor * (diffCoef + specCoef * diffCoef), 1.0);

    color = fc;
}