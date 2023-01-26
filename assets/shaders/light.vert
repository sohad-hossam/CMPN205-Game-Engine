#version 330 core

//attributes needed from mesh buffer:
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;  //our surface direction [surface looking at what direction]



//our unifroms:
//we want to divide the transformation matrix as we want to apply the light in the world space  not in view space
uniform mat4 object_to_world;  //from local to world
uniform mat4 object_to_world_inv_transpose;
uniform mat4 view_projection;  //now we want the world space to the homogeneous clipping space using the view projection [canoncial 2D with z]
uniform vec3 camera_position;  //direction out of the object to the eye [view direction]



//our varyings to be sent to frag shader:
//we send to frag the world position as we need to calculate light in WORLD space first
//we want to calculate light in frag shader pixels by pixel [by phong shading model] 
//in phong shading model we calculate everything [phong light model] but for every pixel
out Varyings {
   vec4 color;
   vec2 tex_coord;
   //new things needed to calculate light:
   vec3 world;  //obj direction in the world
   vec3 view;   //direction from object to camera
   vec3 normal; //normal of the surface 
} vsout;




void main() {
   vec3 temp_world = (object_to_world * vec4(position, 1.0f)).xyz; 
   vsout.world = temp_world;   //we convert the position local to the world position - we dont need W as w=1
   vsout.view = camera_position - temp_world;    //vector from point to camera
   //we normalize as if non-uniforms scalling occured  we should calculate Nworld=(M^-1^T)*Nlocal to avoid incorrect reflections and normals
   vsout.normal = normalize((object_to_world_inv_transpose * vec4(normal, 0.0)).xyz);      //normal 0 as vector not point
   
   //normal code as before: ---we complete rest of trasnformation and set color and text-ccoord to be sent to shader
   gl_Position = view_projection * vec4(temp_world, 1.0);  //gettint the position by multiplying view projection
   vsout.color = color;
   vsout.tex_coord = tex_coord;
}