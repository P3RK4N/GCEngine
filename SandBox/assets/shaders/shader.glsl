#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_UV;
			
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_TransformMatrix;

out vec3 v_Position;
out vec3 v_Color;
out vec2 v_UV;

void main()
{
	v_Position = a_Position;
	v_Color = a_Color;
	v_UV = a_UV;
	gl_Position = u_ViewProjectionMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core
			
uniform vec3 u_Color;
uniform sampler2D u_Texture;
			
in vec3 v_Position;
in vec3 v_Color;
in vec2 v_UV;

layout(location = 0) out vec4 color;
			
void main()
{
color = texture(u_Texture, v_UV) * vec4(u_Color, 1.0);
}