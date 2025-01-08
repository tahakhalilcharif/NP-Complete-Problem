#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h> 

#define MAX_VARS 100
#define MAX_CLAUSES 100
#define UNASSIGNED -1

typedef struct {
    int clauses[MAX_CLAUSES][MAX_VARS + 1]; // CNF clauses (+1 for ending 0)
    int num_clauses;
    int num_vars;
} SATProblem;

// Helper function: Add a clause to the SAT problem
void add_clause(SATProblem *problem, int *clause) {
    int clause_index = problem->num_clauses++;
    int i = 0;
    while (clause[i] != 0) {
        problem->clauses[clause_index][i] = clause[i];
        i++;
    }
    problem->clauses[clause_index][i] = 0; // End with 0
}

// Helper function: Count inversions in the puzzle
int count_inversions(int puzzle[], int size) {
    int inversions = 0;
    for (int i = 0; i < size; i++) {
        if (puzzle[i] == 0) continue; // Skip the blank tile
        for (int j = i + 1; j < size; j++) {
            if (puzzle[j] != 0 && puzzle[i] > puzzle[j]) {
                inversions++;
            }
        }
    }
    return inversions;
}

// Helper function: Find the blank tile's row from the bottom
int find_blank_row_from_bottom(int puzzle[], int size) {
    int n = (int)sqrt(size);
    for (int i = size - 1; i >= 0; i--) {
        if (puzzle[i] == 0) {
            int row = i / n;          // Row index from top
            return n - row;          // Row index from the bottom
        }
    }
    return -1; // Should not happen
}

// DPLL Solver: Check if a clause is satisfied
bool is_clause_satisfied(int clause[], int assignments[]) {
    for (int i = 0; clause[i] != 0; i++) {
        int var = abs(clause[i]);
        int sign = (clause[i] > 0) ? 1 : -1;
        if (assignments[var] == sign) {
            return true; // Clause is satisfied
        }
    }
    return false; // Clause is not satisfied
}

// DPLL Solver: Check if a clause is unsatisfied
bool is_clause_unsatisfied(int clause[], int assignments[]) {
    for (int i = 0; clause[i] != 0; i++) {
        int var = abs(clause[i]);
        if (assignments[var] == UNASSIGNED) {
            return false; // Clause is not yet unsatisfied
        }
    }
    return true; // Clause is unsatisfied
}

// DPLL Recursive Function
bool dpll(SATProblem *problem, int assignments[]) {
    // Check if all clauses are satisfied
    bool all_satisfied = true;
    for (int i = 0; i < problem->num_clauses; i++) {
        if (!is_clause_satisfied(problem->clauses[i], assignments)) {
            all_satisfied = false;
            break;
        }
    }
    if (all_satisfied) return true;

    // Check if any clause is unsatisfied
    for (int i = 0; i < problem->num_clauses; i++) {
        if (is_clause_unsatisfied(problem->clauses[i], assignments)) {
            return false; // Conflict detected
        }
    }

    // Choose the next unassigned variable
    for (int var = 1; var <= problem->num_vars; var++) {
        if (assignments[var] == UNASSIGNED) {
            // Try assigning true
            assignments[var] = 1;
            if (dpll(problem, assignments)) return true;

            // Backtrack and try assigning false
            assignments[var] = -1;
            if (dpll(problem, assignments)) return true;

            // Reset assignment
            assignments[var] = UNASSIGNED;
            break;
        }
    }
    return false;
}

// Function to check solvability using SAT
bool check_sliding_puzzle_solvability(int puzzle[], int size) {
    SATProblem problem = {0};
    problem.num_vars = size; // One variable per tile placement

    // Count inversions
    int inversions = count_inversions(puzzle, size);

    // Find blank tile's row from the bottom
    int blank_row_from_bottom = find_blank_row_from_bottom(puzzle, size);

    // Encode parity constraints
    int n = (int)sqrt(size);
    if (n % 2 == 1) {
        // Odd grid: Solvable if inversions are even
        if (inversions % 2 != 0) {
            int clause[] = {-1, 0}; // UNSAT clause
            add_clause(&problem, clause);
        }
    } else {
        // Even grid: Solvable if blank_row + inversions have opposite parity
        if ((blank_row_from_bottom % 2 == 0) == (inversions % 2 == 0)) {
            int clause[] = {-1, 0}; // UNSAT clause
            add_clause(&problem, clause);
        }
    }

    // Solve SAT
    int assignments[MAX_VARS + 1] = {UNASSIGNED};
    return dpll(&problem, assignments);
}

// Main function

int main() {
    // File to store results
    FILE *output_file = fopen("timing_results.txt", "w");
    if (!output_file) {
        printf("Error: Could not open file for writing.\n");
        return 1;
    }

    fprintf(output_file, "Dimension,Average Time (ms)\n");

    int max_dimension = 100;  // Maximum grid size (10x10)
    int trials = 10;        // Number of trials per dimension

    for (int dim = 3; dim <= max_dimension; dim++) {
        int size = dim * dim;
        int puzzle[size];
        double total_time = 0.0;

        printf("Testing %dx%d puzzles...\n", dim, dim);

        for (int trial = 0; trial < trials; trial++) {
            // Generate a random puzzle
            for (int i = 0; i < size; i++) {
                puzzle[i] = i;
            }
            // Shuffle the puzzle
            for (int i = size - 1; i > 0; i--) {
                int j = rand() % (i + 1);
                int temp = puzzle[i];
                puzzle[i] = puzzle[j];
                puzzle[j] = temp;
            }

            // Start timing
            clock_t start_time = clock();
            check_sliding_puzzle_solvability(puzzle, size);
            clock_t end_time = clock();

            // Calculate elapsed time in milliseconds
            double elapsed_time = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;
            total_time += elapsed_time;
        }

        // Calculate average time for the current dimension
        double average_time = total_time / trials;
        fprintf(output_file, "%d,%.2f\n", dim, average_time);

        printf("Dimension: %dx%d, Average Time: %.2f ms\n", dim, dim, average_time);
    }

    fclose(output_file);

    printf("Timing results written to timing_results.txt\n");
    return 0;
}