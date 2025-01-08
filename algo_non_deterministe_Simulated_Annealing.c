#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_SIZE 100              // Taille maximale du taquin (réduite pour les tests)
#define MAX_ITERATIONS_LARGE 500  // Nombre d'itérations pour les grandes tailles
#define MAX_ITERATIONS_SMALL 1000 // Nombre d'itérations pour les petites tailles

// Représente une position dans la matrice
typedef struct
{
    int x, y;
} Position;

// Calcul de l'énergie : Distance de Manhattan
int calculateEnergy(int state[MAX_SIZE][MAX_SIZE], int final_state[MAX_SIZE][MAX_SIZE], int N)
{
    int energy = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int value = state[i][j];
            if (value != 0)
            {
                for (int k = 0; k < N; k++)
                {
                    for (int l = 0; l < N; l++)
                    {
                        if (final_state[k][l] == value)
                        {
                            energy += abs(i - k) + abs(j - l);
                        }
                    }
                }
            }
        }
    }
    return energy;
}

// Vérifie si une position est valide dans la matrice
int isValidMove(int x, int y, int N)
{
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// Applique un mouvement (haut, bas, gauche, droite)
void applyMove(int state[MAX_SIZE][MAX_SIZE], Position zero_pos, Position move, Position *new_zero_pos)
{
    int temp = state[move.x][move.y];
    state[move.x][move.y] = state[zero_pos.x][zero_pos.y];
    state[zero_pos.x][zero_pos.y] = temp;
    new_zero_pos->x = move.x;
    new_zero_pos->y = move.y;
}

// Génère un état initial aléatoire
void generateRandomState(int state[MAX_SIZE][MAX_SIZE], int N)
{
    int numbers[MAX_SIZE * MAX_SIZE];
    for (int i = 0; i < N * N - 1; i++)
    {
        numbers[i] = i + 1;
    }
    numbers[N * N - 1] = 0; // Position du vide (0)

    // Mélange les nombres pour générer un état initial aléatoire
    for (int i = 0; i < N * N; i++)
    {
        int j = rand() % (N * N);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }

    int idx = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            state[i][j] = numbers[idx++];
        }
    }
}

// Résolution du taquin avec Simulated Annealing
void solveTaquin(int initial_state[MAX_SIZE][MAX_SIZE], int final_state[MAX_SIZE][MAX_SIZE], double T_initial, double alpha, int max_iterations, int N, FILE *file)
{
    int current_state[MAX_SIZE][MAX_SIZE];
    Position zero_pos;

    // Initialiser l'état courant et trouver la position initiale du 0
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            current_state[i][j] = initial_state[i][j];
            if (current_state[i][j] == 0)
            {
                zero_pos.x = i;
                zero_pos.y = j;
            }
        }
    }

    double T = T_initial;
    clock_t start_time, end_time;

    // Démarrer le chronomètre pour mesurer le temps total de résolution
    start_time = clock();

    // Répéter le processus de Simulated Annealing
    for (int iteration = 0; iteration < max_iterations; iteration++)
    {
        // Vérifie si l'état courant est final
        if (calculateEnergy(current_state, final_state, N) == 0)
        {
            end_time = clock(); // Fin du chronomètre
            double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            fprintf(file, "%d %.6f\n", N, time_taken); // Format souhaité : taille et temps
            return;
        }

        // Génération aléatoire d'un mouvement
        Position moves[4] = {
            {zero_pos.x - 1, zero_pos.y}, // Haut
            {zero_pos.x + 1, zero_pos.y}, // Bas
            {zero_pos.x, zero_pos.y - 1}, // Gauche
            {zero_pos.x, zero_pos.y + 1}  // Droite
        };
        int random_index = rand() % 4;

        // Vérifie si le mouvement est valide
        if (isValidMove(moves[random_index].x, moves[random_index].y, N))
        {
            int new_state[MAX_SIZE][MAX_SIZE];
            Position new_zero_pos;

            // Copie l'état courant dans un nouvel état
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    new_state[i][j] = current_state[i][j];
                }
            }

            // Applique le mouvement
            applyMove(new_state, zero_pos, moves[random_index], &new_zero_pos);

            // Calcul des énergies
            int energy_current = calculateEnergy(current_state, final_state, N);
            int energy_new = calculateEnergy(new_state, final_state, N);

            // Décision d'acceptation
            int delta_energy = energy_new - energy_current;
            if (delta_energy < 0 || exp(-delta_energy / T) > ((double)rand() / RAND_MAX))
            {
                // Accepte le nouvel état
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++)
                    {
                        current_state[i][j] = new_state[i][j];
                    }
                }
                zero_pos = new_zero_pos;
            }
        }

        // Réduction de la température
        T *= alpha;
    }

    end_time = clock(); // Fin du chronomètre si le taquin n'est pas résolu
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    fprintf(file, "%d %.6f\n", N, time_taken); // Format souhaité : taille et temps
}

int main()
{
    srand(time(NULL)); // Initialisation de la graine aléatoire

    // Paramètres de l'algorithme
    double T_initial = 1000.0;
    double alpha = 0.99;
    int max_iterations = 1000;                                                   // Nombre d'itérations pour les petites tailles
    int max_iterations_large = MAX_ITERATIONS_LARGE;                             // Réduit les itérations pour les grandes tailles
    int sizes[] = {2, 3, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000}; // Tailles des taquins

    // Ouvrir le fichier pour sauvegarder les résultats
    FILE *file = fopen("execution_times.txt", "w");
    if (file == NULL)
    {
        printf("Erreur d'ouverture du fichier.\n");
        return 1;
    }

    // Boucle pour résoudre les taquins de différentes tailles
    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
    {
        int N = sizes[i];
        int initial_state[MAX_SIZE][MAX_SIZE];
        int final_state[MAX_SIZE][MAX_SIZE];

        // Générer l'état final pour chaque taille
        int num = 1;
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                if (j == N - 1 && k == N - 1)
                    final_state[j][k] = 0;
                else
                    final_state[j][k] = num++;
            }
        }

        // Générer un état initial aléatoire
        generateRandomState(initial_state, N);

        // Modifier les itérations en fonction de la taille
        if (N <= 100)
            solveTaquin(initial_state, final_state, T_initial, alpha, max_iterations, N, file);
        else
            solveTaquin(initial_state, final_state, T_initial, alpha, max_iterations_large, N, file);
    }

    fclose(file); // Fermer le fichier

    printf("Les temps d'exécution ont été sauvegardés dans 'execution_times.txt'.\n");

    return 0;
}
