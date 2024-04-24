#vertex
#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 color;

out vec2 fragUV;
out vec3 fragNormal;
out vec3 Color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 pos = projection * view * model * vec4(position, 1.0);
    gl_Position = pos;
    fragUV = uv;
    fragNormal = (model * vec4(normal, 0.0)).xyz;
    Color = color;
}

#fragment
#version 400
in vec2 fragUV;
in vec3 fragNormal;
in vec3 Color;
out vec4 fragColor;

void main() {
    vec3 color = vec3(1.0,0.0,1.0);
    vec3 normal = normalize(fragNormal);
    float intensity = dot(normal, vec3(0.0, 1.0, 0.0)) + 0.2;
    fragColor = vec4(fragUV.xy,1.0, 1.0);
}