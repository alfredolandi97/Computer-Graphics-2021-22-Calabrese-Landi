#include "Coordinate.h"

Coordinate::Coordinate(){
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

Coordinate::Coordinate(float x, float y, float z){
	pos = glm::vec3(x, y, z);
}

Coordinate::Coordinate(float *coordinatesVector){
	pos.x = coordinatesVector[0];
	pos.y = coordinatesVector[1];
	pos.z = coordinatesVector[2];
}

float Coordinate::getX(){
	return pos.x;
}

float Coordinate::getY(){
	return pos.y;
}

float Coordinate::getZ(){
	return pos.z;
}

glm::vec3 Coordinate::getPos(){
	return pos;
}