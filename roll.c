#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
/*
roll - A simple diceroller for RPG and CLI enthusiasts

roll xdy    rolls a dy x times and sums their results
    $: roll 2d10 3d4 1d123
    [13] [6] [88]

roll -p xdy     rolls xdy as a dice pool, gives individual results for each die
    $: roll -p 2d10 3d4 1d123
    [5,8] [4,2] [88]
    
roll xdy+a-b    rolls a dy x times and sums their results, modifies final sum with modifiers    
    $: roll 1d20+4-6 3d6-20+3
    [12 -> 10] [9 -> -8]
    
    $: roll -p 3d6+2-3
    [2->1, 3->2, 4->3]
    
    
roll xdy>z   rolls a dy x times and returning success if results are Greater or Equal than a Target Number
    $: roll 2d10>15 3d4>3 1d123>1409
    [13 - Failed] [6 - Success] [88 - Invalid TN]
    
    $: roll -p 3d6+2-3>3
    [2->1, 3->2, 4->3 - 1 Success]
    
    $: roll -p 3d6+2-3>2000
    [2->1, 3->2, 4->3 - 0 Success]
*/
bool DICE_POOL_MODE = false;
char* TOKEN_DIE = "d";
void parse_roll( char *roll ){
/*
parses rolls and calls the roll_dice function
>Detect number of rolls and ice types
    |-> xdy is *must* be followed
>Detect modifiers
>Detect target Number

*/ 
    
    uint64_t  num_rolls, die_type, modifiers, target_num;
    //Break string, get num_rolls
    char *curr_token;
    curr_token = strtok(roll,TOKEN_DIE);
    printf("%s",curr_token);    
    
    //convert to integer
        
    

    return;
}




void roll_dice(){
//actually runs randm
}

int main(int argc, char **argv){
    int c;

    //Parse Options 
    while( (c = getopt(argc, argv, "ph")) != -1  ){ 
        switch(c){
            case 'p':
                printf("menos p");
                DICE_POOL_MODE = true;
                break;
            case 'h':
                printf("menos aga");
            default:
                printf("%c\n",c);
                break;
        }
    }
     
    printf("\n_____________\n");
    
    //optiond -> special external variable of getopt
    //
    for(int i=optind; i < argc; i++){
        parse_roll(argv[i]);
        //printf("%s",argv[i]);
        //printf("\n");
    }
    return EXIT_SUCCESS;
}
