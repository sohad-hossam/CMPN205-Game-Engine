#version 330 core

//our varyings:
in Varyings {
   vec4 color;
   vec2 tex_coord;
   vec3 world;
   vec3 view;    
   vec3 normal;  //normal of the light
} fsin;
out vec4 frag_color;




//we have 3 types Directional, Point and spot:
#define TYPE_DIRECTIONAL    0
#define TYPE_POINT          1
#define TYPE_SPOT           2


//our Structs:    ====>we need light struct, material struct and textures struct

//We are using Phong lightening model where ====> Diffuse + Specular + Ambient  = Phong Reflection
//diffuse is lights scattering in all directions equally [gotten from single ray] 
//specular is when lights reflexts in almost one single direction  
struct Light {
   int type;                         //to know which type of the 3 above
   //we could put those 3 in only one thing ==> color 
   vec3 diffuse;                     //for directional light, point light, spot light
   vec3 specular;                    //for directional light, point light , spot light
   vec3 position, direction;         //directional light doesnt have position(direction only)
   vec3 ambient;
   //while position for point light and spot light ======>THEY DONT HAVE DIRECTION LIGHT
   //Attenuation factor: its for point light as whenever dis increase photons strength decrease 
   //  intensity=1/(c+ld+qd^2)   =>general rule as if we needed light with effects not in real lufe but normally it should be 1/d^2 only
   float attenuation_constant;       //for point light
   float attenuation_linear;         //for point light
   float attenuation_quadratic;      //for point light


   float inner_angle, outer_angle;   //for spot light
};



//we have material properties that define diffuse, specular, ambient in addition to its shineness
//alpha is shineness [0-inf] when alpha increase shinennes increase
//The more rough the surface, the less shiny it is. Roughness is from 0 to 1 but shininess is from 0 to ∞
//ambient is when light comes from all directions and scatters equally in all directions
struct TexturedMaterial {
   sampler2D albedo_map;
   sampler2D specular_map;
   sampler2D roughness_map;
   sampler2D ambient_occlusion_map;
   sampler2D emission_map;
};

struct Material {

   vec3 diffuse;
   vec3 specular;
   vec3 ambient;
   vec3 emissive;
   float shininess;
   float roughness;

};

//our uniforms:
#define MAX_LIGHT_COUNT 16
uniform Light lights[MAX_LIGHT_COUNT];   //all the lights we have either directuional,point or spot
uniform int light_count;
uniform TexturedMaterial tex_mat;
//our output

void main() {

    //defining sampled material [which one?] to gt material we will use
      Material sampled;
   //--------------------------------------------------------//
      sampled.diffuse = texture(tex_mat.albedo_map, fsin.tex_coord).rgb;
      sampled.specular = texture(tex_mat.specular_map, fsin.tex_coord).rgb;
      sampled.emissive = texture(tex_mat.emission_map, fsin.tex_coord).rgb;
      sampled.ambient = sampled.diffuse*texture(tex_mat.ambient_occlusion_map, fsin.tex_coord).rgb;
      sampled.roughness = texture(tex_mat.roughness_map, fsin.tex_coord).r;
      sampled.shininess = 2.0/pow(clamp(sampled.roughness, 0.001, 0.999), 4.0) - 2.0;
   //-------------------------------------------------------//
   //for all
   vec3 normal = normalize(fsin.normal);  //normalize to avoid anuthing happening from interpolation
   vec3 view = normalize(fsin.view);    //must normalize view after interpolation

   //for the accumulated light:
   int count = min(light_count, MAX_LIGHT_COUNT);
   vec3 accumulated_light = vec3(0.0, 0.0, 0.0);
   frag_color = vec4(0.0,0.0,0.0,1.0); //to be able to accumulate on the frag color
   //looping around lights
   for(int index = 0; index < count; index++)
   {
      Light light = lights[index];
      vec3 light_direction;

      float attenuation = 1;

      if(light.type == 0)
         light_direction = -light.direction;
      else {
         light_direction = normalize(light.position - fsin.world );
         float new_distance = distance(light.position, fsin.world);
         attenuation *= 1.0f / (light.attenuation_constant +
         light.attenuation_linear * new_distance +
         light.attenuation_quadratic * new_distance * new_distance);
         if(light.type == TYPE_SPOT){
            float angle = acos(dot(-light_direction, light.direction));
            attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);
         }
      }
         vec3 reflected = reflect(-light_direction, normal);
         float lambert = max(0.0f, dot(normal, light_direction));
         float phong = pow(max(0.0f, dot(view, reflected)), sampled.shininess);
         vec3 diffuse =  light.diffuse * sampled.diffuse  * lambert;
         vec3 specular = light.specular * sampled.specular * phong;
         vec3 ambient = light.ambient * sampled.ambient;
         accumulated_light += (diffuse + specular) * attenuation + sampled.ambient;
         //accumulated_light += (diffuse + specular) * attenuation ;
         //accumulated_light += diffuse;  
   }
   //frag_color = fsin.color * vec4(accumulated_light, 1.0f);//try gowa
   frag_color = fsin.color * vec4(accumulated_light, 1.0);
   //frag_color= vec4(accumulated_light,0.0);
}