R"(
#version 450 core

in VS_OUT {
	vec4 color;
	vec2 uv;
} fs_in;

uniform sampler2D uTexture;

out vec4 color;

void main()
{
	color = fs_in.color * texture(uTexture, fs_in.uv);
}
)"