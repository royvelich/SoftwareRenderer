#include "Edge.h"

Edge::Edge(glm::ivec2 vertex1, glm::ivec2 vertex2) :
	vertex1(vertex1),
	vertex2(vertex2)
{
	int delta_x = vertex1.x - vertex2.x;
	if (delta_x != 0)
	{
		slope = static_cast<double>(vertex1.y - vertex2.y) / static_cast<double>(delta_x);
	}
	else
	{
		slope = 0;
	}

	if (vertex1.y < vertex2.y)
	{
		lowest_vertex_x = vertex1.x;
		lowest_vertex_y = vertex1.y;
		highest_vertex_y = vertex2.y;
	}
	else
	{
		lowest_vertex_x = vertex2.x;
		lowest_vertex_y = vertex2.y;
		highest_vertex_y = vertex1.y;
	}
}

Edge::~Edge()
{

}

bool Edge::operator<(const Edge& edge) const
{
	return this->lowest_vertex_x < edge.lowest_vertex_x;
}

void Edge::AddSlope()
{
	lowest_vertex_x += slope;
}

int Edge::GetLowX()
{
	return lowest_vertex_x;
}

int Edge::GetLowY()
{
	return lowest_vertex_y;
}

int Edge::GetHighY()
{
	return highest_vertex_y;
}