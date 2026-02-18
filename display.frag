#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;

void main() {
    vec3 col = texture(screenTexture, TexCoord).rgb;
    // Tone mapping
    col = col / (col + vec3(1.0));
    // Gamma correction
    col = pow(col, vec3(1.0 / 2.2));
    FragColor = vec4(col, 1.0);
}
