#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform vec2 u_TextureScaling = {1.f, 1.f};
uniform sampler2D u_Texture;

void main()
{
	// o_Color = texture(u_Texture, v_TexCoord * u_TextureScaling) * u_Color;
	o_Color = v_Color;
}