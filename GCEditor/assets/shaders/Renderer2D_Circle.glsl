#type vertex
#version 450 core
			
struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};

void main()
{	
	Output.LocalPosition = a_LocalPosition;
	Output.Color = a_Color;
	Output.Thickness = a_Thickness;
	Output.Fade = a_Fade;

	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjectionMatrix * vec4(a_WorldPosition, 1.0);
}




#type fragment
#version 450 core

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

layout(location = 0) out vec4 color1;
layout(location = 1) out int color2;

layout (binding = 0) uniform sampler2D u_Textures[32];
			
void main()
{
	float distance = 1.0 - length(Input.LocalPosition);
	float circleAlpha = smoothstep(0.0, Input.Fade, distance);
	circleAlpha *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circleAlpha == 0.0)
		discard;

	color1 = Input.Color;
	color1.a = circleAlpha;

	color2 = v_EntityID;
}