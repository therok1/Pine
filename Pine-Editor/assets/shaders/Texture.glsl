#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat float v_TexIndex;
out float v_TilingFactor;
			
void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;
in float v_TilingFactor;

const int textureNum = 32;
uniform sampler2D u_Textures[textureNum];
			
void main()
{
	vec4 texColor = v_Color;
	int texIndex = int(v_TexIndex) % textureNum;
	texColor *= texture(u_Textures[texIndex], v_TexCoord * v_TilingFactor);
	color = texColor;
}
