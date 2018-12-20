#version 330 core
out vec4 FragColor;

// #version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec2 texcoord;

// Textures
uniform sampler2D texture;

void main() {

    // vec3 materialColor = texture2D( texture, texcoord ).rgb;

    // gl_FragColor.rgb = materialColor;

    vec4 materialColor = texture2D( texture, texcoord );

    if(materialColor.a < 0.1)
            discard;

    FragColor = materialColor;
}
