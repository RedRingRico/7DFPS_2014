#version 150

uniform sampler2D u_Texture;

in vec2 f_TexCoord;

out vec4 o_FinalColour;

void main( )
{
	o_FinalColour = texture( u_Texture, f_TexCoord );
}

