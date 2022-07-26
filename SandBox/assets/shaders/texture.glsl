#type vertex
#version 330 core
			
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_TransformMatrix;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TextureScale;

out vec4 v_Color;
out vec2 v_UV;
out float v_TexIndex;

void main()
{	
	v_UV = a_UV * a_TextureScale;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}




#type fragment
#version 330 core
			
uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_UV;
in float v_TexIndex;

layout(location = 0) out vec4 color;
			
void main()
{
color = texture(u_Textures[int(v_TexIndex)], v_UV) * v_Color;
}