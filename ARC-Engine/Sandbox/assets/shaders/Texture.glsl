#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
				
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.f);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec2 u_TextureScaling = vec2(1.f, 1.f);
uniform vec4 u_Color;

in vec2 v_TexCoord;

void main()
{
	o_Color = texture(u_Texture, v_TexCoord * u_TextureScaling) * u_Color;
}	