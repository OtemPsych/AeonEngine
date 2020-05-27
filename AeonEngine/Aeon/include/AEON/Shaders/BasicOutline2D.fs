R"(
#version 450 core

in VS_OUT {
	vec4 color;
	vec2 uv;
} fs_in;

uniform sampler2D uTexture;
uniform float     uOutlineSize;
uniform vec4      uOutlineColor;

out vec4 color;

void main()
{
	vec4 tcol = fs_in.color * texture(uTexture, fs_in.uv);

	if (tcol.a == 0.0 &&
	    (texture(uTexture, fs_in.uv + vec2( 0.0,           uOutlineSize)).a != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2( 0.0,          -uOutlineSize)).a != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2( uOutlineSize,  0.0)).a          != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2(-uOutlineSize,  0.0)).a          != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2(-uOutlineSize,  uOutlineSize)).a != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2(-uOutlineSize, -uOutlineSize)).a != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2( uOutlineSize,  uOutlineSize)).a != 0.0 ||
	     texture(uTexture, fs_in.uv + vec2( uOutlineSize, -uOutlineSize)).a != 0.0))
	{
		tcol = uOutlineColor;
	}

	color = tcol;
}
)"