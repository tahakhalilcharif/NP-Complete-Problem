#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define INF 1e9
#define MAX_N 10 // Maximum size of the grid

typedef struct {
    int grid[MAX_N][MAX_N];
    int x, y; // Position of the empty tile
    int n;    // Size of the grid
} State;

typedef struct {
    State states[1000]; // To store states of the solution path
    int size;           // Number of states in the solution path
} SolutionPath;

int dx[] = {-1, 1, 0, 0}; // Row movement directions
int dy[] = {0, 0, -1, 1}; // Column movement directions

void generate_goal(int n, int goal[MAX_N][MAX_N]) {
    int value = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 1 && j == n - 1)
                goal[i][j] = 0; // Empty tile
            else
                goal[i][j] = value++;
        }
    }
}

int manhattan_distance(State *state) {
    int dist = 0;
    for (int i = 0; i < state->n; i++) {
        for (int j = 0; j < state->n; j++) {
            if (state->grid[i][j] != 0) {
                int value = state->grid[i][j] - 1;
                int goal_x = value / state->n, goal_y = value % state->n;
                dist += abs(i - goal_x) + abs(j - goal_y);
            }
        }
    }
    return dist;
}

int is_goal(State *state, int goal[MAX_N][MAX_N]) {
    for (int i = 0; i < state->n; i++) {
        for (int j = 0; j < state->n; j++) {
            if (state->grid[i][j] != goal[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void copy_state(State *dest, State *src) {
    memcpy(dest->grid, src->grid, sizeof(src->grid));
    dest->x = src->x;
    dest->y = src->y;
    dest->n = src->n;
}

int is_valid(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

int ida_search(State *state, int g, int threshold, long long int *nodes_expanded, SolutionPath *solution, int goal[MAX_N][MAX_N]) {
    (*nodes_expanded)++;
    int f = g + manhattan_distance(state);
    if (f > threshold) return f;
    if (is_goal(state, goal)) {
        solution->states[solution->size++] = *state; // Add goal state to solution path
        return -1; // Solution found
    }

    int min_threshold = INF;
    for (int i = 0; i < 4; i++) {
        int nx = state->x + dx[i];
        int ny = state->y + dy[i];
        if (is_valid(nx, ny, state->n)) {
            State neighbor;
            copy_state(&neighbor, state);
            // Swap tiles
            neighbor.grid[state->x][state->y] = neighbor.grid[nx][ny];
            neighbor.grid[nx][ny] = 0;
            neighbor.x = nx;
            neighbor.y = ny;

            int result = ida_search(&neighbor, g + 1, threshold, nodes_expanded, solution, goal);
            if (result == -1) {
                solution->states[solution->size++] = *state; // Add current state to solution path
                return -1; // Solution found
            }
            if (result < min_threshold) min_threshold = result;
        }
    }
    return min_threshold;
}

void visualize_solution(SolutionPath *solution, FILE *output_file) {
    fprintf(output_file, "Solution Path:\n");
    for (int i = solution->size - 1; i >= 0; i--) {
        fprintf(output_file, "Step %d:\n", solution->size - i);
        for (int row = 0; row < solution->states[i].n; row++) {
            for (int col = 0; col < solution->states[i].n; col++) {
                fprintf(output_file, "%2d ", solution->states[i].grid[row][col]);
            }
            fprintf(output_file, "\n");
        }
        fprintf(output_file, "\n");
    }
}

void shuffle_state(State *state) {
    int n = state->n;
    int total_moves = n * n * 10; // Arbitrary number of moves to shuffle
    for (int i = 0; i < total_moves; i++) {
        int random_direction = rand() % 4;
        int nx = state->x + dx[random_direction];
        int ny = state->y + dy[random_direction];
        if (is_valid(nx, ny, n)) {
            // Swap tiles
            state->grid[state->x][state->y] = state->grid[nx][ny];
            state->grid[nx][ny] = 0;
            state->x = nx;
            state->y = ny;
        }
    }
}

void ida_star(State *start, int goal[MAX_N][MAX_N], FILE *output_file) {
    int threshold = manhattan_distance(start);
    int iterations = 0;
    long long int total_nodes_expanded = 0;

    SolutionPath solution = { .size = 0 };

    while (1) {
        iterations++;
        long long int nodes_expanded = 0;
        clock_t start_time = clock();

        int result = ida_search(start, 0, threshold, &nodes_expanded, &solution, goal);

        clock_t end_time = clock();
        double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

        fprintf(output_file, "Iteration %d:\n", iterations);
        fprintf(output_file, "  Threshold: %d\n", threshold);
        fprintf(output_file, "  Nodes Expanded: %lld\n", nodes_expanded);
        fprintf(output_file, "  Time Taken: %.4f seconds\n\n", time_taken);

        total_nodes_expanded += nodes_expanded;

        if (result == -1) {
            fprintf(output_file, "Solution Found!\n");
            fprintf(output_file, "Total Iterations: %d\n", iterations);
            fprintf(output_file, "Total Nodes Expanded: %lld\n", total_nodes_expanded);
            fprintf(output_file, "Solution Depth: %d\n", threshold);
            visualize_solution(&solution, output_file);
            break;
        }
        if (result == INF) {
            fprintf(output_file, "No Solution!\n");
            break;
        }
        threshold = result;
    }
}

int main() {
    srand(time(NULL));

    FILE *output_file = fopen("results.txt", "a");
    if (!output_file) {
        perror("Failed to open file");
        return 1;
    }

    time_t current_time = time(NULL);
    fprintf(output_file, "Execution Date and Time: %s\n", ctime(&current_time));

    for (int n = 6; n <= 6; n++) { // Adjust range as needed
        fprintf(output_file, "Solving %dx%d Puzzle:\n", n, n);

        int goal[MAX_N][MAX_N];
        generate_goal(n, goal);

        State start = { .n = n };
        memcpy(start.grid, goal, sizeof(goal));
        start.x = n - 1;
        start.y = n - 1;

        shuffle_state(&start);
        ida_star(&start, goal, output_file);
    }

    fclose(output_file);
    return 0;
}
