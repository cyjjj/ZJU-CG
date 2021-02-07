#pragma once
#ifndef GENERATEMAZE_H
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <time.h>
#include <vector>

void pushNeighbors(int u, int v, int H, int W, std::vector<std::vector<int>>& seen, std::vector<std::pair<int, int>>& edges);
void Prim(int H, int W, std::vector<std::vector<int>>& maze);
int getNewMaze(int H, int W, std::vector<std::vector<int>>& maze);

#endif // !GENERATEMAZE_H
