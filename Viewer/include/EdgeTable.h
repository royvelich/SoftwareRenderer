#pragma once

#include <memory>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Edge.h"

class EdgeTable
{
public:
	typedef std::pair<int, int> ScanLineIntersection;
	typedef std::pair<ScanLineIntersection, ScanLineIntersection> FillLine;
	typedef std::vector<FillLine> FillLines;

public:
	EdgeTable(const std::vector<std::shared_ptr<Edge>>& edges);
	virtual ~EdgeTable();

	FillLines GetFillLines();

private:
	std::vector<std::shared_ptr<Edge>> edges;
	std::map<int, std::vector<std::shared_ptr<Edge>>> scan_line_to_edge_start;
	std::vector<std::shared_ptr<Edge>> active_edges;

	int low_scan_line;
	int high_scan_line;
};
