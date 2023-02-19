#ifndef COORDINATE_H_INCLUDED
#define COORDINATE_H_INCLUDED
#endif

class Coordinate{
	private:
		glm::vec3 pos;
		float angle;
	public:
		Coordinate();
		Coordinate(float x, float y, float z);
		Coordinate(float *coordinatesVector);
		Coordinate(float *coordinatesVector, float angleParameter);
		float getX();
		float getY();
		float getZ();
		glm::vec3 getPos();
		float getAngle();
};