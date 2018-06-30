const char* morph_vs = R"(

attribute vec4 pose1_vert;
attribute vec3 pose1_normal;
attribute vec4 pose2_vert;
attribute vec3 pose2_normal;
attribute vec2 texcoord;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform float u_blend;

varying vec4 v_gouraud_dst;
varying vec2 v_texcoord;

void main()
{
	vec4 vert = pose1_vert + (pose2_vert - pose1_vert) * u_blend;
	gl_Position = u_projection * u_modelview * vert;
	v_gouraud_dst = vec4(pose1_normal * 0.001, 0);
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