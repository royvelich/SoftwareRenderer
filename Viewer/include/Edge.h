#pragma once

#include <vector>
#include <glm/glm.hpp>

class Edge
{
public:
	Edge(glm::ivec2 vertex1, glm::ivec2 vertex2);
	virtual ~Edge();

	bool operator<(const Edge& edge) const;
	void AddSlope();

	int GetLowX();
	int GetLowY();
	int GetHighY();

private:
	glm::ivec2 vertex1;
	glm::ivec2 vertex2;

	double lowest_vertex_x;
	int lowest_vertex_y;
	int highest_vertex_y;

	double slope;
};
