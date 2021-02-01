#include "EdgeTable.h"
#include <algorithm>

EdgeTable::EdgeTable(const std::vector<std::shared_ptr<Edge>>& edges) :
	edges(edges)
{
	if (edges.size() > 0)
	{
		low_scan_line = edges[0]->GetLowY();
		high_scan_line = edges[0]->GetHighY();
		for (int i = 0; i < edges.size(); i++)
		{
			int low_y = edges[i]->GetLowY();
			int high_y = edges[i]->GetHighY();
			scan_line_to_edge_start[low_y].push_back(edges[i]);

			if (low_y < low_scan_line)
			{
				low_scan_line = low_y;
			}

			if (high_y > high_scan_line)
			{
				high_scan_line = high_y;
			}
		}
	}
}

EdgeTable::~EdgeTable()
{

}

EdgeTable::FillLines EdgeTable::GetFillLines()
{
	FillLines fill_lines;
	for (int i = low_scan_line; i <= high_scan_line; i++)
	{
		active_edges.insert(active_edges.end(), scan_line_to_edge_start[i].begin(), scan_line_to_edge_start[i].end());
		active_edges.erase(std::remove_if(active_edges.begin(), active_edges.end(), [&i](const std::shared_ptr<Edge>& edge) { return edge->GetHighY() == i; }), active_edges.end());
		std::sort(active_edges.begin(), active_edges.end());

		for (int j = 0; j < active_edges.size(); j += 2)
		{
			int current_edge_index = j;
			int next_edge_index = j + 1;
			if (current_edge_index < active_edges.size() && next_edge_index < active_edges.size())
			{
				std::shared_ptr<Edge> current_edge = active_edges[current_edge_index];
				std::shared_ptr<Edge> next_edge = active_edges[next_edge_index];
				fill_lines.push_back(FillLine(ScanLineIntersection(current_edge->GetLowX(), i), ScanLineIntersection(next_edge->GetLowX(), i)));
			}
		}

		for (int j = 0; j < active_edges.size(); j += 1)
		{
			active_edges[j]->AddSlope();
		}
	}

	return fill_lines;
}