#type vertex
#version 450 core
			
struct VertexOutput
{
	vec4 Color;
	vec2 UV;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TextureScale;
layout(location = 5) in int a_EntityID;

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat float v_TexIndex;
layout (location = 3) out flat int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};

//uniform mat4 u_ViewProjectionMatrix;
//uniform mat4 u_TransformMatrix;


void main()
{	
	Output.UV = a_UV * a_TextureScale;
	Output.Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}




#type fragment
#version 450 core

struct VertexOutput
{
	vec4 Color;
	vec2 UV;
};

layout (location = 0) in VertexOutput Input;
layout (location = 2) in flat float v_TexIndex;
layout (location = 3) in flat int v_EntityID;

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

layout (binding = 0) uniform sampler2D u_Textures[32];
			
void main()
{
	vec4 texColor = Input.Color;

	switch(int(v_TexIndex))
	{
		case  0: texColor *= texture(u_Textures[ 0], Input.UV); break;
		case  1: texColor *= texture(u_Textures[ 1], Input.UV); break;
		case  2: texColor *= texture(u_Textures[ 2], Input.UV); break;
		case  3: texColor *= texture(u_Textures[ 3], Input.UV); break;
		case  4: texColor *= texture(u_Textures[ 4], Input.UV); break;
		case  5: texColor *= texture(u_Textures[ 5], Input.UV); break;
		case  6: texColor *= texture(u_Textures[ 6], Input.UV); break;
		case  7: texColor *= texture(u_Textures[ 7], Input.UV); break;
		case  8: texColor *= texture(u_Textures[ 8], Input.UV); break;
		case  9: texColor *= texture(u_Textures[ 9], Input.UV); break;
		case 10: texColor *= texture(u_Textures[10], Input.UV); break;
		case 11: texColor *= texture(u_Textures[11], Input.UV); break;
		case 12: texColor *= texture(u_Textures[12], Input.UV); break;
		case 13: texColor *= texture(u_Textures[13], Input.UV); break;
		case 14: texColor *= texture(u_Textures[14], Input.UV); break;
		case 15: texColor *= texture(u_Textures[15], Input.UV); break;
		case 16: texColor *= texture(u_Textures[16], Input.UV); break;
		case 17: texColor *= texture(u_Textures[17], Input.UV); break;
		case 18: texColor *= texture(u_Textures[18], Input.UV); break;
		case 19: texColor *= texture(u_Textures[19], Input.UV); break;
		case 20: texColor *= texture(u_Textures[20], Input.UV); break;
		case 21: texColor *= texture(u_Textures[21], Input.UV); break;
		case 22: texColor *= texture(u_Textures[22], Input.UV); break;
		case 23: texColor *= texture(u_Textures[23], Input.UV); break;
		case 24: texColor *= texture(u_Textures[24], Input.UV); break;
		case 25: texColor *= texture(u_Textures[25], Input.UV); break;
		case 26: texColor *= texture(u_Textures[26], Input.UV); break;
		case 27: texColor *= texture(u_Textures[27], Input.UV); break;
		case 28: texColor *= texture(u_Textures[28], Input.UV); break;
		case 29: texColor *= texture(u_Textures[29], Input.UV); break;
		case 30: texColor *= texture(u_Textures[30], Input.UV); break;
		case 31: texColor *= texture(u_Textures[31], Input.UV); break;
	}

	color = texColor;
	color2 = v_EntityID;
}