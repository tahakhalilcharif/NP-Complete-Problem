#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAX_SEQUENCE_SIZE 1000

typedef struct
{
    char name[20];
    bool (*precondition)(char *state);
    char *(*action)(char *state);
} Rule;

bool dummy_precondition(char *state)
{
    return true; // Remplacez par la logique réelle
}

char *dummy_action(char *state)
{
    return state; // Remplacez par la logique réelle
}

void non_deterministic_method(char *initial_state, char *final_state, Rule rules[], int rule_count)
{
    char current_state[100] = "";
    strncpy(current_state, initial_state, 100);

    char result_sequence[MAX_SEQUENCE_SIZE] = "";

    while (strcmp(current_state, final_state) != 0)
    {
        int random_index = rand() % rule_count;
        Rule chosen_rule = rules[random_index];

        if (chosen_rule.precondition(current_state))
        {
            char *new_state = chosen_rule.action(current_state);
            if (new_state != NULL)
            {
                strncpy(current_state, new_state, 100);
                if (strlen(result_sequence) + strlen(chosen_rule.name) < MAX_SEQUENCE_SIZE)
                {
                    strcat(result_sequence, chosen_rule.name);
                }
                else
                {
                    printf("Erreur : La séquence de résultats est trop grande !\n");
                    break;
                }
            }
            else
            {
                printf("Erreur : L'action de la règle a échoué.\n");
            }
        }
    }

    printf("Résultat : %s\n", result_sequence);
}

int main()
{
    srand(time(NULL));

    Rule rules[4] = {
        {"Règle 1", dummy_precondition, dummy_action},
        {"Règle 2", dummy_precondition, dummy_action},
        {"Règle 3", dummy_precondition, dummy_action},
        {"Règle 4", dummy_precondition, dummy_action}};

    char initial_state[] = "État Initial";
    char final_state[] = "État Final";

    non_deterministic_method(initial_state, final_state, rules, 4);

    return 0;
}
