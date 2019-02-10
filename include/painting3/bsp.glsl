const char* bsp_vs = R"(

attribute vec4 position;
attribute vec2 texcoord;
attribute vec2 texcoord_light;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

varying vec2 v_texcoord;
varying vec2 v_texcoord_light;

void main()
{
	gl_Position = u_projection * u_view * u_model * position;
	v_texcoord = texcoord;
	v_texcoord_light = texcoord_light;
}

)";

const char* bsp_fs = R"(

uniform sampler2D u_base_tex;
uniform sampler2D u_light_tex;

varying vec2 v_texcoord;
varying vec2 v_texcoord_light;

void main()
{
	vec4 base = texture2D(u_base_tex, v_texcoord);
	vec4 light = texture2D(u_light_tex, v_texcoord_light);
	gl_FragColor = base * light;
}

)";
