#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in int a_EntityId;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out int v_EntityId;

void main()
{
	v_Color = a_Color; 
	v_EntityId = a_EntityId;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
}

#type fragment
#version 450

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityId;

in vec4 v_Color;
in flat int v_EntityId;

void main()
{
	if (v_Color.a < 0.05) discard;
	o_Color = v_Color;
	o_EntityId = v_EntityId;
}