#version 150

uniform sampler2D u_Texture;

in vec2 f_TexCoord;

out vec4 o_WorldPosition;
out vec4 o_Albedo;
out vec4 o_TexCoord;

void main( )
{
	o_WorldPosition = vec4( 1.0, 0.0, 0.0, 1.0 );
	o_Albedo = vec4( 0.0, 1.0, 0.0, 1.0 );
	o_TexCoord = vec4( f_TexCoord, 1.0, 1.0 );
}

