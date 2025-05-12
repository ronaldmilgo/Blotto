#include <stdio.h>
#include <stdlib.h>
#include "entry.h"
#include "gmap.h"
#include "string_key.h"

#define SIZE 1000
double get_totalscore(int *value1, int *value2, int *battlefield, size_t battlefieldcount);
void free_value(const void *key, void *value, void *arg);

int main(int argc, char **argv){
    if (argc < 2){
        fprintf(stderr, "Error: Invalid command-line argument\n");
        return 1;
    }

    size_t battlefields_count = argc - 1;
    // printf("battlefield_count: %zu\n", battlefields_count);
    //create an array to store the battefield values
    int *battlefields = malloc(sizeof(int) * battlefields_count);
    if (battlefields ==  NULL){
        fprintf(stderr, "Memory allocation for battlefields failed!\n");
        return 1;
    }
    for (size_t i = 0; i < battlefields_count; i++){
        battlefields[i] = atoi(argv[i + 1]);
    }

    // printf("Battlefields: ");
    // for (size_t i = 0; i < battlefields_count; i++){
    //     printf("%d ", battlefields[i]);
    // }
    // printf("\n");
    

    // create the gmap
    gmap *map = gmap_create(duplicate, compare_keys, hash29, free);
    if(map == NULL){
        fprintf(stderr, "Error: Could not create map\n");
        free(battlefields);
        return 1;
    }

    //read the input and populate the gmap
    while(1){
        entry input = entry_read(stdin, SIZE, battlefields_count);
        if (input.id == NULL || input.id[0] == '\0'){
            free(input.id);
            break;
        }
        gmap_put(map, input.id, input.distribution);
        free(input.id);
        // entry_destroy(&input);
    }

    //processing matchups
    char player_1[SIZE], player_2[SIZE]; 
    while(scanf("%s %s", player_1, player_2) == 2){
        int *player1_values = gmap_get(map, player_1);
        int *player2_values = gmap_get(map, player_2);

        // printf("player1_values: ");
        // for(size_t i = 0; i < battlefields_count; i++){
        //      printf("%d ", player1_values[i]);
        // }
        // printf("\n");


        // printf("player2_values: ");
        // for(size_t i = 0; i < battlefields_count; i++){
        //      printf("%d ", player2_values[i]);
        // }
        // printf("\n");
       

        if (player1_values == NULL || player2_values == NULL){
            fprintf(stderr, "Error: One or both players not found\n");
            continue;
        }

        //player 1 score
        double player1_score = get_totalscore(player1_values, player2_values, battlefields, battlefields_count);
        //player 2 score
        double player2_score = get_totalscore(player2_values, player1_values, battlefields, battlefields_count);
        
        if (player2_score > player1_score){
            printf("%s %.1lf - %s %.1lf\n", player_2, player2_score, player_1, player1_score);
        }else{
        printf("%s %.1lf - %s %.1lf\n", player_1, player1_score, player_2, player2_score);
        }
    }

    //free the value
    gmap_for_each(map, free_value, NULL);
    gmap_destroy(map);
 
    free(battlefields);

    return 0;
    
}

double get_totalscore(int *value1, int *value2, int *battlefield, size_t battlefieldcount){
    double score = 0.0;
    for(size_t i = 0; i < battlefieldcount; i++){
        if (value1[i] > value2[i]){
            score += battlefield[i];
        } else if(value1[i] == value2[i]){
            score += (battlefield[i]/2.0);
        }
    }
    return score;
}

void free_value(const void *key, void *value, void *arg){
    free(value);
}