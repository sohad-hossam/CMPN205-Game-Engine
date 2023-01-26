#version 330 core

uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

    // Calculate noise and sample texture
float noiseScale = 2000.0;

//-----------------------------
// Noise function
//Using a vector of two constants, taking the dot product of that and the supplied vector,
// the sine of that then the fract of THAT and we have a pseudo-random number.

float rand(vec2 n) { 
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p){
    vec2 ip = floor(p);
    vec2 u = fract(p);
    u = u*u*(3.0-2.0*u);

float res = mix(
        mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
        mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
    return res*res;
}
//------------------------------------------

void main() {

     //Get noise value
    float n = noise(tex_coord*noiseScale);

    //sample from the texture
    vec4 color = texture(tex, tex_coord);

    // Multiply the RGB color of your main texture with the noise
    // Add 0.5 so we at least retain more bright colors
    color.rgb *=n+0.5;
    frag_color = color;
}