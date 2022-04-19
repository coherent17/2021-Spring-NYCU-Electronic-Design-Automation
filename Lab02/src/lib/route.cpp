#include <queue>
#include <utility>
#include <vector>
#include <bits/stdc++.h>
#include <stdbool.h>
#include <stdlib.h>
#include "readfile.h"
#include "grid.h"
#include "route.h"

#define DISTANCE_EMPTY -1
#define DISTANCE_OCCUPIED -2
#define NO_PARENT {-1, -1}

using namespace std;
using std::queue;
using std::vector;
using std::pair;

bool isValid(int x, int y, vector<vector<int>> distance, Grid *grid){
	if(x < 0 || x >= grid->col || y < 0 || y >= grid->row){
		return false;
	}
	return true;
}

void printDistance(vector<vector<int>> distance, Grid *grid){
	for(int i = grid->row - 1; i >= 0; i--){
		for(int j = 0; j < grid->col; j++){
			if(distance[i][j] == DISTANCE_OCCUPIED) printf(" X ");
			else printf("%2d ", distance[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


vector <Point> routeOneNet(Grid *grid, Net net, bool *isValidRouting){

	//move to the neighbor (up, left, down, right)
	int dx[4] = {0, -1, 0, 1};
	int dy[4] = {1, 0, -1, 0};

	//copy the grid state to the distance, initial set the element in the distance as DISTANCE_EMPTY (-1)
	//if there exist a block or pin or other routing path, then set to DISTANCE_OCCUPIED (-2)
	vector<vector<int>> distance(grid->row,vector<int>(grid->col,DISTANCE_EMPTY));

	//the 2D array to store the parent of the coming grid
	vector<vector<Point>> Parent(grid->row, vector<Point>(grid->col, {-1, -1}));

	for(int i = 0; i < grid->row; i++){
		for(int j = 0; j < grid->col; j++){
			//if that grid is occupied by the block or the pin or the path of the routed net
			if(grid->gridState[i][j] < 0){
				distance[i][j] = DISTANCE_OCCUPIED;
			}
		}
	}

	queue <pair<int, int>> q;

	int initial_distance = 0;

	//set the distance of the source as 0
	distance[net.sourceY][net.sourceX] = 0;

	q.push({net.sourceX, net.sourceY});
	q.push({initial_distance + 1, 0});

	while(!q.empty()){

		bool canBreak = false;

		int x = q.front().first;
		int y = q.front().second;
		q.pop();

		int current_distance = q.front().first;
		q.pop();

		for(int i = 0; i <= 3; i++){
			int new_x = x + dx[i];
			int new_y = y + dy[i];

			if(!isValid(new_x, new_y, distance, grid)) continue;


			//if meet the target -> return
			if(new_x == net.targetX && new_y == net.targetY){
				*isValidRouting = true;
				Parent[new_y][new_x].x = x;
				Parent[new_y][new_x].y = y;
				canBreak = true;
				//abort();
				break;
			}

			else if(distance[new_y][new_x] == -1){
				distance[new_y][new_x] = current_distance;
				Parent[new_y][new_x].x = x;
				Parent[new_y][new_x].y = y;
				q.push({new_x, new_y});
				q.push({current_distance + 1, 0});
				//printDistance(distance, grid);
			}
		}
		if(canBreak) break;
	}

	vector <Point> Path;
	for(pair <int, int> temp = {net.targetX, net.targetY}; ;temp = {Parent[temp.second][temp.first].x, Parent[temp.second][temp.first].y}){
		if(Parent[temp.second][temp.first].x == -1 && Parent[temp.second][temp.first].y == -1) break;
		else{
			Point temp_point;
			temp_point.x = Parent[temp.second][temp.first].x;
			temp_point.y = Parent[temp.second][temp.first].y;
			Path.emplace_back(temp_point);		
		}
	}

	std::reverse(Path.begin(), Path.end());
	return Path;
}

void printPath(vector <Point> Path){
	for(auto it = Path.begin(); it != Path.end(); it++){
		printf("(%d,%d) -> ", (*it).x, (*it).y);
	}
	printf("\n");
} 

void updateGridState(vector <Point> Path, Grid *grid, Net net){
	for(auto it = Path.begin(); it != Path.end(); it++){
		if(((*it).x == net.sourceX && (*it).y == net.sourceY) || ((*it).x == net.targetX && (*it).y == net.targetY)) continue;
		grid->gridState[(*it).y][(*it).x] = NET_OCCUPIED;
	}
}