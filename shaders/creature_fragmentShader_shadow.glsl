// Diffuse, Specular and BumpMapping textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

// Shadow variables
//uniform sampler2D ShadowMap;
uniform sampler2DShadow ShadowMap;
uniform sampler2DShadow BackShadowMap;
varying vec4 ShadowCoord;
varying vec4 ShadowCoordBack;

// Diffuse and Ambient components

varying vec3 normal,lightDir,halfVector;

// New bumpmapping
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;

uniform vec2 textureUnitUniform ;

varying float lightDistance;

float lookup(vec4 loc, vec2 offSet)
{
   return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * 1.0/4096.0 * ShadowCoord.w, offSet.y * 1.0/3072.0 * ShadowCoord.w, 0.0, 0.0) ).s;
   //return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * textureUnitUniform.s * ShadowCoord.w, offSet.y * textureUnitUniform.t * ShadowCoord.w, 0.0, 0.0) ).s;
   //return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * 0.1, offSet.y * 0.1, 0.0, 0.0) ).s;
}


float lookupBack(vec4 loc, vec2 offSet)
{
   return shadow2DProj(BackShadowMap, ShadowCoordBack + vec4(offSet.x * 1.0/4096.0 * ShadowCoordBack.w, offSet.y * 1.0/3072.0 * ShadowCoordBack.w, 0.0, 0.0) ).s;
   //return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * textureUnitUniform.s * ShadowCoord.w, offSet.y * textureUnitUniform.t * ShadowCoord.w, 0.0, 0.0) ).s;
   //return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * 0.1, offSet.y * 0.1, 0.0, 0.0) ).s;
}

void main()
{

  // fetch normal from normal map, expand to the [-1, 1] range, and normalize
  //vec3 normal = vec4(0.0,0.0,1.0,0.0);
  vec3 normal = 2.0 * texture2D (normalTexture, gl_TexCoord[0].st).rgb - 1.0;
  normal = normalize (normal);

  // compute diffuse lighting
  float lamberFactor= max (dot (lightVec, normal), 0.0) ;
  vec4 diffuseMaterial = 0.0;
  vec4 diffuseLight  = 0.0;

  // compute specular lighting
  vec4 specularMaterial ;
  vec4 specularLight ;
  float shininess ;
  
    // compute ambient
  vec4 ambientLight = gl_LightSource[0].ambient;
  
  float shadow = 0.0;
	float bias = 0.00;
	
	
	
  if (lamberFactor > 0.0)
  {
  	 //diffuseMaterial = pow(texture2D (diffuseTexture, gl_TexCoord[0].st),2.0);
  	 diffuseMaterial = pow(texture2D (diffuseTexture, gl_TexCoord[0].st),2.0);
  	 //diffuseMaterial = vec4(0.2);
  	 diffuseLight  = gl_LightSource[0].diffuse;
  
  
  	 specularMaterial =  pow(texture2D (specularTexture, gl_TexCoord[0].st),2.0);
  	 specularLight = gl_LightSource[0].specular;
  	 shininess = pow (max (dot (halfVec, normal), 0.0), 0.01)  ;
  	 
  	 if (ShadowCoord.w > 0)
	{
	 	float x,y;
				for (y = -1.5 ; y <=1.5 ; y+=1.0)
					for (x = -1.5 ; x <=1.5 ; x+=1.0)
						shadow += lookup(ShadowCoord,vec2(x,y));
				shadow /= 16.0 ;
	}
	else
	{
	 	float x,y;
				for (y = -1.5 ; y <=1.5 ; y+=1.0)
					for (x = -1.5 ; x <=1.5 ; x+=1.0)
						shadow += lookupBack(ShadowCoord,vec2(x,y));
				shadow /= 16.0 ;
	}
	
  }
  
	    



	
			
	    // Computing final color with shadow
  
	 
  gl_FragColor =	diffuseMaterial * diffuseLight * lamberFactor ;
  gl_FragColor +=	specularMaterial * specularLight * shininess ;				
  gl_FragColor *= 		shadow;
  	
  gl_FragColor /= (gl_LightSource[0].constantAttenuation + gl_LightSource[0].linearAttenuation * lightDistance + gl_LightSource[0].quadraticAttenuation * lightDistance * lightDistance) ;

   gl_FragColor +=	ambientLight;
   
  // gl_FragColor =  pow(gl_FragColor, 1.0/2.2);
   
}
