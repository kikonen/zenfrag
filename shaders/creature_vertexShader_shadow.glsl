// Used for shadow lookup
varying vec4 ShadowCoord;
varying vec4 ShadowCoordBack;

// Diffuse and Ambient components
varying vec3 normal,lightDir,halfVector;


attribute vec3 tangent;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;

varying float lightDistance;

void main()
{

		// This is the only shadow part in the Vertex Shader
     	ShadowCoord= gl_TextureMatrix[0] * gl_Vertex;
     	ShadowCoordBack = gl_TextureMatrix[1] * gl_Vertex;
     	
	
		
		gl_TexCoord[0] =  gl_MultiTexCoord0;
		
		// Building the matrix Eye Space -> Tangent Space
		vec3 n = normalize (gl_NormalMatrix * gl_Normal);
  		vec3 t = normalize (gl_NormalMatrix * tangent);
  		vec3 b = cross (n, t);
  		
  		vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
  		vec3 lightDir = normalize(gl_LightSource[0].position.xyz - vertexPosition);
  		
  		
  		 // transform light and half angle vectors by tangent basis
		  vec3 v;
		  v.x = dot (lightDir, t);
		  v.y = dot (lightDir, b);
		  v.z = dot (lightDir, n);
		  lightVec = normalize (v);
		
		  
		  v.x = dot (vertexPosition, t);
		  v.y = dot (vertexPosition, b);
		  v.z = dot (vertexPosition, n);
		  eyeVec = normalize (v);
  
		  lightDistance = length(vertexPosition - gl_LightSource[0].position.xyz);
		  
		  vertexPosition = normalize(vertexPosition);
		
		  // No need to divide by two, the result is normalized anyway.
	      // vec3 halfVector = normalize((vertexPosition + lightDir) / 2.0); 
	      vec3 halfVector = normalize(vertexPosition + lightDir);

		  v.x = dot (halfVector, t);
		  v.y = dot (halfVector, b);
		  v.z = dot (halfVector, n);
		  
		  // No need to normalize, t,b,n and halfVector are normal vectors.
		  //normalize (v);
	      halfVec = v ; 
	  
	
		  
		  
		gl_Position = ftransform();

		

}
