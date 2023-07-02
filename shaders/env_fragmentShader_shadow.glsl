// Shadow variables
//uniform sampler2D ShadowMap;
uniform sampler2DShadow ShadowMap;
varying vec4 ShadowCoord;
uniform vec2 textureUnitUniform ;



float lookup(vec4 loc, vec2 offSet)
{
  // return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * 1.0/4096.0 * ShadowCoord.w, offSet.y * 1.0/3072.0 * ShadowCoord.w, 0.0, 0.0) ).s;
   return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * textureUnitUniform.s * ShadowCoord.w, offSet.y * textureUnitUniform.t * ShadowCoord.w, 0.0, 0.0) ).s;
   //return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * 0.1, offSet.y * 0.1, 0.0, 0.0) ).s;
}


void main()
{

		
		
		vec4 color = gl_LightSource[0].ambient;
		
		float shadow = 0.0;

	
		

			
			if (ShadowCoord.z > 0.0)
			{
				float x,y;
				for (y = -1.5 ; y <=1.5 ; y+=1.0)
					for (x = -1.5 ; x <=1.5 ; x+=1.0)
						shadow += lookup(ShadowCoord,vec2(x,y));
				shadow /= 16.0 ;
			}
		
	
	    
	
	
    gl_FragColor =    vec4(shadow) *  gl_LightSource[0].ambient;
}
