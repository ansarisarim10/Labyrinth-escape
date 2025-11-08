#version 330 core
in vec3 vN;
out vec4 FragColor;

uniform vec3 uColor;

void main() {
    vec3 L = normalize(vec3(0.3, 0.7, 0.5));
    float diff = max(dot(normalize(vN), L), 0.0);
    vec3 col = uColor * (0.25 + 0.75 * diff);
    FragColor = vec4(col, 1.0);
}
