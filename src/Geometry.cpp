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


	shaderProgram = phongShader;
	//shaderProgram = frostShader;

	std::cout << "\nGeometry Initialized! \n " << std::endl;

}
	
void Geometry::render(std::vector<glm::mat4> sceneMatrices, glm::vec3 lightPos, glm::vec3 lightColor, float lightPower) {
    
 /*  glUseProgram(shaderTextureProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right
*/	
	mLightPos = lightPos;
 	mLightColor = lightColor;

	if(shaderProgram == phongShader) {
		renderPhong(sceneMatrices, lightPos, lightColor, lightPower);
	}
	else {
		renderFrost(sceneMatrices, lightPos, lightColor);
	}
	//std::cout << " lightColor: " << lightColor[0] << " " << lightColor[1] << " " << lightColor[2] << " " << lightColor[3] << " " << std::endl;


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


    glDisable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );

}


void Geometry::renderFrost(std::vector<glm::mat4> sceneMatrices, glm::vec3 lightPos, glm::vec3 lightColor) {
	/*


	//set names for tour uniforms, these names has to be the same as in the sahders
	MVPLoc 			= glGetUniformLocation(frostShader, "MVP");
	MVLoc 			= glGetUniformLocation(frostShader, "MV");
	MVLightLoc 		= glGetUniformLocation(frostShader, "MV_light");
	NMLoc 			= glGetUniformLocation(frostShader, "NM");
	lightPosLoc 	= glGetUniformLocation(frostShader, "lightPos");
	lightColLoc 	= glGetUniformLocation(frostShader, "lightColor");
	colorLoc 		= glGetUniformLocation(frostShader,	 "color");
	lightAmbLoc 	= glGetUniformLocation(frostShader, "lightAmb");
	lightDifLoc 	= glGetUniformLocation(frostShader, "lightDif");
	lightSpecLoc 	= glGetUniformLocation(frostShader, "lightSpec");
	specularityLoc 	= glGetUniformLocation(frostShader, "specularity");
	shinynessLoc 	= glGetUniformLocation(frostShader, "shinyness");
	timeLoc		 	= glGetUniformLocation(frostShader, "currTime");
	texID   		= glGetUniformLocation(frostShader, "renderedTexture");
	MVPLoc 			= glGetUniformLocation(frostShader, "MVP");
	colorLoc 		= glGetUniformLocation(frostShader, "color");
	 //set which shader to use
	glUseProgram(shaderProgram);
	glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
	glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV][0][0]);
	glUniformMatrix4fv(MVLightLoc, 1, GL_FALSE, &sceneMatrices[I_MV_LIGHT][0][0]);
	glUniformMatrix4fv(NMLoc, 1, GL_FALSE, &sceneMatrices[I_NM][0][0]);
	glUniform4f(lightPosLoc, mLightPos[0], mLightPos[1], mLightPos[2], 1.0f);
	glUniform4f(lightColLoc, mLightColor[0], mLightColor[1], mLightColor[2], 1.0f);
	glUniform4f(colorLoc, mMaterial.color[0], mMaterial.color[1], mMaterial.color[2], mMaterial.color[3]);
	glUniform4f(lightAmbLoc, mMaterial.ambient[0], mMaterial.ambient[1], mMaterial.ambient[2], mMaterial.ambient[3]);
	glUniform4f(lightDifLoc, mMaterial.diffuse[0], mMaterial.diffuse[1], mMaterial.diffuse[2], mMaterial.diffuse[3]);
	glUniform4f(lightSpecLoc, mMaterial.specular[0], mMaterial.specular[1], mMaterial.specular[2], mMaterial.specular[3]);
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

    glDisable( GL_BLEND );*/
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
