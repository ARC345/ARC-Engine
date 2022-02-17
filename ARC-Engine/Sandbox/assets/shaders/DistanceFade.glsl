#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
				
uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec2 v_ScreenPos;
out vec4 v_Color;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.f);
	v_ScreenPos = gl_Position.xy;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Texture;
uniform vec2 u_TextureScaling = vec2(1.f, 1.f);
uniform vec4 u_Color;

in vec2 v_TexCoord;
in vec2 v_ScreenPos;

void main()
{
	float dist = 1.0f - distance(v_ScreenPos * 0.8f, vec2(0.0f));
	dist = clamp(dist, 0.0f, 1.0f);
	dist = sqrt(dist);
	o_Color = texture(u_Texture, v_TexCoord * u_TextureScaling) * u_Color * dist;
}	