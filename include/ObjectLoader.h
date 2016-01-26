/******************************************************************************************
 *
 * A utility for loading object files (.obj)
 *
 * Based primarily on code from LIU course TNM079 lab files written by:
 * Gunnar Johansson, Ken Museth, Michael Bang Nielsen, Ola Nilsson and Andreas Söderström
 *****************************************************************************************/

#ifndef LOADOBJ_H
#define LOADOBJ_H

// Libs and headers
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <exception>
#include <unistd.h>
#include <map>
#include <glm/vec3.hpp>// glm::vec3
#include <glm/vec2.hpp>// glm::vec3

// Classes
#include "math/Vector3.h"


class ObjectLoader {

public:

	ObjectLoader() {}

	bool loadObject(std::string, std::vector<glm::vec3> &, std::vector<glm::vec2> &, std::vector<glm::vec3>&  ); 
	bool loadObject(std::string, std::vector<glm::vec3> &, std::vector<glm::vec3>&  ); 

    std::vector<std::vector<Vector3<float> > > getMeshVertexList(std::string objName);

private:

};

#endif