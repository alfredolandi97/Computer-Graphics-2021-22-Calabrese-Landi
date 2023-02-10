#ifndef COORDINATE_H_INCLUDED
#define COORDINATE_H_INCLUDED
#endif

class Coordinate{
	private:
		glm::vec3 pos;
	public:
		Coordinate();
		Coordinate(float x, float y, float z);
		float getX();
		float getY();
		float getZ();
		glm::vec3 getPos();
};