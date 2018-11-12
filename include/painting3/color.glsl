const char* color_vs = R"(

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;
attribute vec4 color;

uniform mat4 u_projection;
uniform mat4 u_modelview;

uniform mat3 u_normal_matrix;
uniform vec3 u_light_position;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	gl_Position = u_projection * u_modelview * position;

	vec3 eye_normal = normalize(u_normal_matrix * normal);
 	vec4 pos4 = u_modelview * position;
 	vec3 pos3 = pos4.xyz / pos4.w;
 	vec3 light_dir = normalize(u_light_position - pos3);
 	float diff = max(0.0, dot(eye_normal, light_dir));

 	v_gouraud_dst = vec4(diff * color.rgb, 1);

	v_texcoord = texcoord;
}

)";

const char* color_fs = R"(

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
