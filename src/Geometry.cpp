#include "../include/Geometry.h"

Geometry::Geometry() {

    mMaterial.diffuseColor      = glm::vec3(0.8f, 0.8f, 0.8f);
    mMaterial.ambientColor      = mMaterial.diffuseColor * 0.1f;
    mMaterial.specularColor     = glm::vec3(1.0f, 1.0f, 1.0f);
    mMaterial.alpha				= 1.0f;
    mMaterial.specularity   	= 40.0f;
    mMaterial.shinyness    	 	= 0.9f;
}

Geometry::~Geometry() {

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteProgram(shaderProgram);

    mVerts.clear();
    mVerts.shrink_to_fit();
}

void Geometry::initialize(glm::vec3 lightPos, glm::vec3 lightColor, glm::vec3 cameraPos) {

	mLightPos = lightPos;
	mLightColor = lightColor;

	std::cout << "\nInitializing Geometry... \n " << std::endl;

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	phongShader = LoadShaders( "shaders/PhongVertexShader.glsl", "shaders/PhongFragmentShader.glsl");
	//frostShader = LoadShaders( "shaders/FrostVertexShader.glsl", "shaders/FrostFragmentShader.glsl");
		//set names for tour uniforms, these names has to be the same as in the sahders
	MVPLoc 			= glGetUniformLocation(phongShader, "MVP");
	MVLoc 			= glGetUniformLocation(phongShader, "MV");
	MLoc 			= glGetUniformLocation(phongShader, "M");
	VLoc 			= glGetUniformLocation(phongShader, "V");
 	MVLightLoc 		= glGetUniformLocation(phongShader, "MV_light");
	NMLoc 			= glGetUniformLocation(phongShader, "NM");
	cameraPosLoc	= glGetUniformLocation(phongShader, "cameraPosition");
	lightPosLoc 	= glGetUniformLocation(phongShader, "lightPosition");
	lightColLoc 	= glGetUniformLocation(phongShader, "lightColor");
	lightPowerLoc	= glGetUniformLocation(phongShader, "lightPower");
	ambienceLoc 	= glGetUniformLocation(phongShader, "ambientColor");
	diffuseLoc 		= glGetUniformLocation(phongShader, "diffuseColor");
	specularLoc 	= glGetUniformLocation(phongShader, "specularColor");
	alphaLoc		= glGetUniformLocation(phongShader, "alpha");
	specularityLoc 	= glGetUniformLocation(phongShader, "specularity");
	shinynessLoc 	= glGetUniformLocation(phongShader, "shinyness");
	timeLoc		 	= glGetUniformLocation(phongShader, "currTime");
		
	glUniform3f(cameraPosLoc, cameraPos[0], cameraPos[1], cameraPos[2]);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(glm::vec3), &mVerts[0], GL_STATIC_DRAW);
	
	//1st attrib buffed : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,								// attribute 0, I.e layout 0 in the shader
		3,								// size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		reinterpret_cast<void*>(0)		// array buffer offset
	);


	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);
	
	//2nd attrib buffer: normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,								// attribute 1, I.e layout 1 in the shader
		3,								// size
		GL_FLOAT,						// type
		GL_FALSE,						// normalized?
		0,								// stride
		reinterpret_cast<void*>(0)		// array buffer offset
	);

	///------------ texture specific ---------- //// 
	/*//Gen Texture!
	glGenFramebuffers(1, &frostTextureBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frostTextureBuffer);

	// the texture we're going to render to
	glGenTextures(1, &frostTexture);
	//bind the newly created texture
	glBindTexture(GL_TEXTURE_2D, frostTexture);

	//give an empty image to opengl (the last "0");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	//poor filtering needed
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glGenRenderbuffers(1, &depthrenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frostTexture, 0);	

	 // Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
 	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

 	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	// The fullscreen quad's FBO
	static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
	quad_programID = LoadShaders( "shaders/Passthrough.vertexshader", "shaders/WobblyTexture.fragmentshader" );
	texID = glGetUniformLocation(quad_programID, "renderedTexture");
	timeID = glGetUniformLocation(quad_programID, "time");
    */

	shaderProgram = phongShader;
	//shaderProgram = frostShader;

	std::cout << "\nGeometry Initialized! \n " << std::endl;

}
	
void Geometry::render(std::vector<glm::mat4> sceneMatrices, glm::vec3 lightPos, glm::vec3 lightColor, float lightPower) {
    
	mLightPos = lightPos;
 	mLightColor = lightColor;

 	glBindFramebuffer(GL_FRAMEBUFFER, frostTextureBuffer);
 	glViewport(0,0,1024,768);

	renderPhong(sceneMatrices, lightPos, lightColor, lightPower);

	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 1024, 768);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(quad_programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frostTexture);

	glUniform1i(texID, 0);
	glUniform1f(timeID, float(glfwGetTime()*10.0f) );

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
*/


	
}

void Geometry::renderPhong(std::vector<glm::mat4> sceneMatrices, glm::vec3 lightPos, glm::vec3 lightColor, float lightPower) {

	 //set which shader to use
	glUseProgram(shaderProgram);
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
	glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV][0][0]);
	glUniformMatrix4fv(MLoc, 1, GL_FALSE, &sceneMatrices[I_M][0][0]);
	glUniformMatrix4fv(VLoc, 1, GL_FALSE, &sceneMatrices[I_V][0][0]);
	glUniformMatrix4fv(MVLightLoc, 1, GL_FALSE, &sceneMatrices[I_MV_LIGHT][0][0]);
	glUniformMatrix4fv(NMLoc, 1, GL_FALSE, &sceneMatrices[I_NM][0][0]);
	glUniform3f(lightPosLoc, mLightPos[0], mLightPos[1], mLightPos[2]);
	glUniform3f(lightColLoc, mLightColor[0], mLightColor[1], mLightColor[2]);
	glUniform1f(lightPowerLoc, lightPower);
	glUniform3f(ambienceLoc, mMaterial.ambientColor[0], mMaterial.ambientColor[1], mMaterial.ambientColor[2]);
	glUniform3f(diffuseLoc, mMaterial.diffuseColor[0], mMaterial.diffuseColor[1], mMaterial.diffuseColor[2]);
	glUniform3f(specularLoc, mMaterial.specularColor[0], mMaterial.specularColor[1], mMaterial.specularColor[2]);
	glUniform1f(alphaLoc, mMaterial.alpha);
	glUniform1f(specularityLoc, mMaterial.specularity);
	glUniform1f(shinynessLoc, mMaterial.shinyness);
	glUniform1f(timeLoc, glfwGetTime());

		//rebind buffer data
	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(glm::vec3), &mVerts[0], GL_STATIC_DRAW);

  	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
   	glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, mVerts.size());

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);


    glDisable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );

}

void Geometry::loadObject(std::string objPath) {
	bool res = mObjectLoader->loadObject(objPath, mVerts, mNormals);
	//bool res = mObjectLoader->loadObject(objPath, mVerts, mUvs, mNormals);
}

void Geometry::translate(glm::vec3 p) {
	 
	 glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), p);

	 glm::vec4 tmpVec;

	 for(unsigned int i = 0; i < mVerts.size(); i++) {

	 	tmpVec = glm::vec4(mVerts[i], 1.0f);
	 	tmpVec = translationMatrix * tmpVec;

	 	mVerts[i] = glm::vec3(tmpVec.x, tmpVec.y, tmpVec.z);
	 }
}

// Scale the Mesh
void Geometry::scale(glm::vec3 s){
 
    // Compute the scaling matrix
   	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), s);
 	
 	glm::vec4 tmpVec;

    for(unsigned int i = 0; i < mVerts.size(); i++) {
        // Apply the scaling matrix
    	tmpVec = glm::vec4(mVerts[i], 1.0f);
        tmpVec = scalingMatrix * tmpVec;
        mVerts[i] = glm::vec3(tmpVec.x, tmpVec.y, tmpVec.z);
    }
}
