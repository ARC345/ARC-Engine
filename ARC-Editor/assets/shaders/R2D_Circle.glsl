#type vertex
#version 450

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Sharpness;
layout(location = 5) in int a_EntityId;

uniform mat4 u_ViewProjection;

out vec3 v_LocalPosition;
out vec4 v_Color;
out float v_Thickness;
out float v_Sharpness;
out flat int v_EntityId;

void main()
{
	v_LocalPosition = a_LocalPosition; 
	v_Color = a_Color; 
	v_Thickness = a_Thickness;
	v_Sharpness = a_Sharpness;
	v_EntityId = a_EntityId;
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0f);
}

#type fragment
#version 450

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityId;

in vec3 v_LocalPosition;
in vec4 v_Color;
in float v_Thickness;
in float v_Sharpness;
in flat int v_EntityId;

void main()
{	
	float distance = 1.0 - length(v_LocalPosition);
    float circle = smoothstep(0.0, 1.0-v_Sharpness, distance);
    circle *= smoothstep(v_Thickness + 1.0-v_Sharpness, v_Thickness, distance);

    // Set output color
    o_Color = v_Color;
	o_Color.a *= circle;

	if (o_Color.a < 0.05)
		discard;

	o_EntityId = v_EntityId;

}