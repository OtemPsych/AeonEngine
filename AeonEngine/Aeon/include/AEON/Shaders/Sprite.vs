R"(
#version 450 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

layout (shared) uniform uTransformBlock {
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	mat4 mvp;
} uTransform;

out VS_OUT {
	vec4 color;
	vec2 uv;
} vs_out;

void main()
{
	vs_out.color = aColor;
	vs_out.uv = aUV;
	gl_Position = uTransform.viewProjection * vec4(aPosition, 0.0, 1.0);
}
)"