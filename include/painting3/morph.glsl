const char* morph_vs = R"(

attribute vec4 position;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 u_projection;
uniform mat4 u_modelview;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_gouraud_dst = vec4(normal * 0.001, 0);
	v_texcoord = texcoord;
}

)";

const char* morph_fs = R"(

uniform sampler2D u_texture0;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	gl_FragColor = texture2D(u_texture0, v_texcoord) + v_gouraud_dst * 0.0001;
}

)";