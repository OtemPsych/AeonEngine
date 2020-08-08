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
	color = fs_in.color * vec4(1.0, 1.0, 1.0, texture(uTexture, fs_in.uv).r);
}
)"