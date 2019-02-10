const char* default_vs = R"(

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform vec3 u_diffuse_material;
uniform vec3 u_ambient_material;
uniform vec3 u_specular_material;
uniform float u_shininess;
uniform mat3 u_normal_matrix;
uniform vec3 u_light_position;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	gl_Position = u_projection * u_view * u_model * position;

	vec3 eye_normal = normalize(u_normal_matrix * normal);
 	vec3 light_dir = normalize(u_light_position - (u_model * position).xyz);
 	float diff = max(0.0, dot(eye_normal, light_dir));

 	vec4 gouraud_col = vec4(diff * u_diffuse_material, 1);
 	gouraud_col.rgb += u_ambient_material;
 	vec3 reflection = normalize(reflect(-light_dir, eye_normal));
 	float spec = max(0.0, dot(eye_normal, reflection));
 	spec = pow(spec, u_shininess);
 	gouraud_col.rgb += spec * u_specular_material;
	v_gouraud_dst = gouraud_col;

	v_texcoord = texcoord;
}

)";

const char* default_fs = R"(

uniform sampler2D u_texture0;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	vec4 tex_map = texture2D(u_texture0, v_texcoord);
	vec4 tmp = v_gouraud_dst * tex_map;
	gl_FragColor = tmp;
}

)";
