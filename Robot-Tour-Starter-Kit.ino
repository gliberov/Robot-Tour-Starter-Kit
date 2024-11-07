#include "SmartCar.cpp"
#include <vector>
#include <queue>
#include <cmath>
#include <map>
#include <utility>

using namespace std;

// Define grid constants
const int TILE_DISTANCE_MS = 1000; // ms to travel one tile distance
const int TURN_TIME_MS = 500;      // ms for a 90-degree turn

// Define the grid layout and object definitions
const int grid[11][11] = {
    {3, 0, 0, 4, 0, 0, 0, 0, 0, 4, 3},
    {4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
    {2, 4, 0, 0, 0, 4, 3, 4, 0, 0, 0},
    {0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4},
    {3, 4, 0, 0, 3, 4, 0, 0, 0, 4, 1},
    {0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0},
    {0, 0, 0, 4, 0, 4, 0, 0, 3, 4, 0},
    {0, 0, 0, 0, 0, 0, 4, 0, 4, 0, 0},
    {0, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Robot setup
DeviceDriverSet_Motor AppMotor;
Application_xxx Application_ConquerorCarxxx0;
MPU6050_getdata AppMPU6050getdata;

SmartCar car;

// Orientation tracker
enum Orientation { UP, RIGHT, DOWN, LEFT };
Orientation currentOrientation = UP;

// Find the positions of the start, end, and gates
pair<int, int> start, end;
vector<pair<int, int>> gates;

void find_positions() {
    for (int r = 0; r < 11; r++) {
        for (int c = 0; c < 11; c++) {
            if (grid[r][c] == 1) start = {r, c};
            else if (grid[r][c] == 2) end = {r, c};
            else if (grid[r][c] == 3) gates.push_back({r, c});
        }
    }
}

// Pathfinder function (A* or similar)
vector<pair<int, int>> find_path(const pair<int, int>& start, const pair<int, int>& goal) {
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> open_set;
    map<pair<int, int>, pair<int, int>> came_from;
    map<pair<int, int>, int> g_score;
    map<pair<int, int>, int> f_score;

    open_set.push({0, start});
    g_score[start] = 0;
    f_score[start] = abs(goal.first - start.first) + abs(goal.second - start.second);

    while (!open_set.empty()) {
        auto current = open_set.top().second;
        open_set.pop();

        if (current == goal) break;

        // Explore neighbors in four directions
        for (auto [dr, dc] : vector<pair<int, int>>{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
            pair<int, int> neighbor = {current.first + dr, current.second + dc};
            if (neighbor.first < 0 || neighbor.first >= 11 || neighbor.second < 0 || neighbor.second >= 11) continue;
            if (grid[neighbor.first][neighbor.second] == 4) continue;  // Skip barriers

            int tentative_g_score = g_score[current] + 1;
            if (tentative_g_score < g_score[neighbor] || g_score.find(neighbor) == g_score.end()) {
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = tentative_g_score + abs(goal.first - neighbor.first) + abs(goal.second - neighbor.second);
                open_set.push({f_score[neighbor], neighbor});
            }
        }
    }

    // Reconstruct path from came_from map
    vector<pair<int, int>> path;
    for (pair<int, int> step = goal; step != start; step = came_from[step]) {
        path.push_back(step);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// Function to follow the path using movement commands
void followPath(const vector<pair<int, int>>& path) {
    for (size_t i = 0; i < path.size() - 1; i++) {
        auto current = path[i];
        auto next = path[i + 1];
        int dx = next.first - current.first;
        int dy = next.second - current.second;

        // Translate direction into movement commands
        if (dx == 0 && dy == 1) {  // Moving "up"
            if (currentOrientation == RIGHT) car.turnLeft(200);
            else if (currentOrientation == LEFT) car.turnRight(200);
            else if (currentOrientation == DOWN) { car.turnRight(200); car.turnRight(200); }
            delay(TURN_TIME_MS); car.moveForwardForSeconds(200, TILE_DISTANCE_MS); currentOrientation = UP;
        } else if (dx == 1 && dy == 0) {  // Moving "right"
            if (currentOrientation == UP) car.turnRight(200);
            else if (currentOrientation == DOWN) car.turnLeft(200);
            else if (currentOrientation == LEFT) { car.turnRight(200); car.turnRight(200); }
            delay(TURN_TIME_MS); car.moveForwardForSeconds(200, TILE_DISTANCE_MS); currentOrientation = RIGHT;
        } else if (dx == 0 && dy == -1) {  // Moving "down"
            if (currentOrientation == RIGHT) car.turnRight(200);
            else if (currentOrientation == LEFT) car.turnLeft(200);
            else if (currentOrientation == UP) { car.turnRight(200); car.turnRight(200); }
            delay(TURN_TIME_MS); car.moveForwardForSeconds(200, TILE_DISTANCE_MS); currentOrientation = DOWN;
        } else if (dx == -1 && dy == 0) {  // Moving "left"
            if (currentOrientation == UP) car.turnLeft(200);
            else if (currentOrientation == DOWN) car.turnRight(200);
            else if (currentOrientation == RIGHT) { car.turnRight(200); car.turnRight(200); }
            delay(TURN_TIME_MS); car.moveForwardForSeconds(200, TILE_DISTANCE_MS); currentOrientation = LEFT;
        }
    }
    car.stop();
}

void setup() {
    Serial.begin(9600);
    car.init();
    find_positions();

    // Calculate the path from start to end
    vector<pair<int, int>> bestPath = find_path(start, end);
    followPath(bestPath);  // Execute movements along the path
}

void loop() {
    // Empty, since we're executing the path in setup
}