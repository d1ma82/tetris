#version 330 core

precision mediump float;
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D camera;

void main() {
    
    FragColor=texture(camera, TexCoord);
}