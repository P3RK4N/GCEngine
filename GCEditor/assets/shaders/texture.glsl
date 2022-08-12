#type vertex
#version 330 core
			
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_TransformMatrix;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TextureScale;
layout(location = 5) in int a_EntityID;

out vec4 v_Color;
out vec2 v_UV;
flat out float v_TexIndex;
flat out int v_EntityID;

void main()
{	
	v_UV = a_UV * a_TextureScale;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}




#type fragment
#version 330 core
			
uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_UV;
flat in float v_TexIndex;
flat in int v_EntityID;

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;
			
void main()
{
color = texture(u_Textures[int(v_TexIndex)], v_UV) * v_Color;
color2 = 1;
}