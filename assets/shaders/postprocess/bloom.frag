#version 330 core

out vec4 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;
in vec4 FragPos;

uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;
//vec3 Normal=(1.0,1.0,1.0);

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition=vec4(tex_coord.x,tex_coord.y,0,0); //convert the texcoord to NDC space 
    // also store the per-fragment normals into the gbuffer
   // gNormal = normalize(Normal);
    // and the diffuse per-fragment color, ignore specular
    gAlbedoSpec.rgb = vec3(0.95);
}