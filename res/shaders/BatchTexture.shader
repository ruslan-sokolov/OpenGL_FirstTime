#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TextureId;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec2 v_TexCoord;
out float v_TextureId;

void main()
{
	v_TexCoord = a_TexCoord;
	v_TextureId = a_TextureId;
	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in float v_TextureId;

uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
	int index = int(v_TextureId);
	o_Color = texture(u_Textures[index], v_TexCoord) * u_Color;
};
