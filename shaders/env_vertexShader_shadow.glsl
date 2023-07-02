// Used for shadow lookup
varying vec4 ShadowCoord;


void main()
{
		gl_Position = ftransform();
		
		ShadowCoord= gl_TextureMatrix[0] * gl_Vertex;
    	
}
