
//Heavly inspiried by http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// for a simple obj reader

//Classes
#include "../include/ObjectLoader.h"

bool ObjectLoader::loadObject(std::string objName, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals) {


    std::string filePath = "assets/" + objName + ".obj";
	
	std::cout << "Loading OBJ file: " << filePath << std::endl;

    FILE *file = fopen(filePath.c_str(), "r");

    if(file == NULL) {
        std::cout << "could not open file" << std::endl;
        return false;
    }

    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    std::vector< unsigned int > vertexIndices;
	std::vector< unsigned int > uvIndices;
	std::vector< unsigned int > normalIndices;

    while(true) {

    	char lineHeader[256];

    	int res = fscanf(file, "%s", lineHeader);
    	if(res == EOF) 
    		break;
    	

    	if( strcmp( lineHeader, "v" ) == 0) {
    		glm::vec3 tmpVertex;
    		fscanf( file, "%f %f %f\n", &tmpVertex[0], &tmpVertex[1], &tmpVertex[2] );
    		temp_vertices.push_back(tmpVertex);
    	} else if( strcmp( lineHeader, "vt") == 0) {
    		glm::vec2 uv;
    		fscanf(file, "%f %f\n", &uv.x, &uv.y);
    		temp_uvs.push_back(uv);
    	} else if( strcmp( lineHeader, "vn") == 0) {
    		glm::vec3 tmpNormal;
    		fscanf( file, "%f %f %f\n", &tmpNormal[0], &tmpNormal[1], &tmpNormal[2] );
    		temp_normals.push_back(tmpNormal);
    	} else if( strcmp( lineHeader, "f") == 0) {

    		std::string vertex1, vertex2, vertex3;
    		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    		//std::string str;
    		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
    	
    		if(matches != 9) {
    			std::cout << "File can't be read by our simple parser :  Try exporting with other options\n" << std::endl;
     			return false;
    		}
				
 			vertexIndices.push_back(vertexIndex[0]);
	    	vertexIndices.push_back(vertexIndex[1]);
	     	vertexIndices.push_back(vertexIndex[2]);
	     	uvIndices    .push_back(uvIndex[0]);
	     	uvIndices    .push_back(uvIndex[1]);
	     	uvIndices    .push_back(uvIndex[2]);
	     	normalIndices.push_back(normalIndex[0]);
	     	normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
    		
    	} else {

			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
    }

    for( unsigned int i = 0; i < vertexIndices.size(); i++) {

    	unsigned int vertexIndex = vertexIndices[i];
    	unsigned int uvIndex = uvIndices[i];
    	unsigned int normalIndex = normalIndices[i];

    	glm::vec3 vertex = temp_vertices[ vertexIndex - 1 ];
    	glm::vec2 uv = temp_uvs[ uvIndex - 1 ];
    	glm::vec3 normal = temp_normals[ normalIndex - 1 ];

    	out_vertices.push_back(vertex);
    	out_uvs		.push_back(uv);
    	out_normals	.push_back(normal);
    }	



    fclose(file);

    std::cout << "OBJ Loaded!" << std::endl;
}


bool ObjectLoader::loadObject(std::string objName, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec3> &out_normals) {


    std::string filePath = "assets/" + objName + ".obj";
	
	std::cout << "Loading OBJ file: " << filePath << std::endl;

    FILE *file = fopen(filePath.c_str(), "r");

    if(file == NULL) {
        std::cout << "could not open file" << std::endl;
        return false;
    }

    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec3 > temp_normals;
    std::vector< unsigned int > vertexIndices;
	std::vector< unsigned int > normalIndices;

    while(true) {

    	char lineHeader[256];

    	int res = fscanf(file, "%s", lineHeader);
    	if(res == EOF) 
    		break;
    	

    	if( strcmp( lineHeader, "v" ) == 0) {
    		glm::vec3 tmpVertex;
    		fscanf( file, "%f %f %f\n", &tmpVertex[0], &tmpVertex[1], &tmpVertex[2] );
    		temp_vertices.push_back(tmpVertex);
    	} else if( strcmp( lineHeader, "vn") == 0) {
    		glm::vec3 tmpNormal;
    		fscanf( file, "%f %f %f\n", &tmpNormal[0], &tmpNormal[1], &tmpNormal[2] );
    		temp_normals.push_back(tmpNormal);
    	} else if( strcmp( lineHeader, "f") == 0) {

    		std::string vertex1, vertex2, vertex3;
    		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
    		//std::string str;
    		int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
    	
    		if(matches != 6) {
    			std::cout << "File can't be read by our simple parser :  Try exporting with other options\n" << std::endl;
     			return false;
    		}
				
 			vertexIndices.push_back(vertexIndex[0]);
	    	vertexIndices.push_back(vertexIndex[1]);
	     	vertexIndices.push_back(vertexIndex[2]);
	     	normalIndices.push_back(normalIndex[0]);
	     	normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
    		
    	} else {

			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
    }

    for( unsigned int i = 0; i < vertexIndices.size(); i++) {

    	unsigned int vertexIndex = vertexIndices[i];
    	unsigned int normalIndex = normalIndices[i];

    	glm::vec3 vertex = temp_vertices[ vertexIndex - 1 ];
    	glm::vec3 normal = temp_normals[ normalIndex - 1 ];

    	out_vertices.push_back(vertex);
    	out_normals	.push_back(normal);
    }	



    fclose(file);

    std::cout << "OBJ Loaded!" << std::endl;
}