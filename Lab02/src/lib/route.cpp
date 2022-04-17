#include <queue>
#include <utility>
#include "grid.h"
#include "readfile.h"
#include "route.h"

using namespace std;
using std::queue;

void bfs(Grid *grid, Net net){

	//go up left down right
	int dx[] = {0, -1,  0, 1};
	int dy[] = {1,  0, -1, 0};

	queue <pair<int, int>> q;	//store the location (x, y) and distance(currentDistance, 0) repectively

	int distance = 0;

	//push the entrance into the queue
	grid->gridState[net.sourceX][net.sourceY] = 0;
	q.push({net.sourceX, net.sourceY});
	q.push({distance + 1, 0});

	while(!q.empty()){
		int x = q.front().first, y = q.front().second;
		q.pop();
		int current_distance = q.front().first;
		q.pop();
		for(int i = 0; i <= 3 ; i++){
			int new_x = x + dx[i];
			int new_y = y + dy[i];

			//if the seed spread to the target -> break the stop wave propagation
			if(new_x == net.targetX && new_y == net.targetY) return;

			if(new_x < 0 || new_x >= grid->col || new_y < 0 || new_y >= grid->row || grid->gridState[new_x][new_y] == BLOCK_OCCUPIED || grid->gridState[new_x][new_y] == NET_OCCUPIED || grid->gridState[new_x][new_y] >= 0) continue;
			else{
				grid->gridState[new_x][new_y] = current_distance;
				q.push({new_x, new_y});
				q.push({current_distance + 1, 0});
				printGrid(grid);
			}
		}
	}
}