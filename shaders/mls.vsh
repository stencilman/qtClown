// minimal vertex shader
// www.lighthouse3d.com

varying vec2 texture_coordinate; 

void main()
{	

	// the following three lines provide the same result

//	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position = ftransform();
	
	// Transforming The Vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	// Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
	texture_coordinate = vec2(gl_MultiTexCoord0);

}
