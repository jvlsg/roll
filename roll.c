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
    
    
roll xdy#z   rolls a dy x times and returning success if results are Greater or Equal than a Target Number
    $: roll 2d10#15 3d4#3 1d123#1409
    [13 - Failed] [6 - Success] [88 - Invalid TN]
    
    $: roll -p 3d6+2-3#3
    [2->1, 3->2, 4->3 - 1 Success]
    
    $: roll -p 3d6+2-3#2000
    [2->1, 3->2, 4->3 - 0 Success]

roll xdy -t z   Sets a global target number of Z
    $: roll -t 5 -p 3d6
    [2, 3, 4 - 0 Success]
*/
bool DICE_POOL_MODE = false;
uint64_t GLOBAL_TARGET_NUM = 0;
char* TOKEN_DIE = "d";



void parse_modifiers(char * str, uint64_t *mod_inc, uint64_t *mod_dec, uint64_t *target_num){
    //printf("to parse %s\n", str);
    uint64_t aux = 0;

    for (uint32_t i = 0; str[i] != '\0'; i++){
        printf("> %c\n", str[i] );
        switch (str[i]){
            case '+':
                aux = strtoll( str+i ,NULL,10);
                //printf("\t+%d\n",aux);
                *mod_inc+=aux;
                break;
            case '-':
                aux = strtoll( str+i ,NULL,10);
                //printf("\t-%d\n",aux);
                *mod_dec+=aux;
                break;
            case '#':
                aux = strtoll( str+i+1 ,NULL,10);
                //printf("\ttn %d\n",aux);
                *target_num=aux;
                break;
            default:
                break;
        }
    }
}

/*
    Parses each roll, calls roll_dice function with proper arguments
*/
void parse_roll( char *roll ){
    
    uint64_t  num_rolls=0, die_type=0, mod_inc=0, mod_dec=0, target_num=0;

    //Break string at 'd' charcter, get num_rolls
    char *aux_token = strtok(roll,TOKEN_DIE);    
    /*
    strtoll - (
            const char *nptr, 
            char **endptr: to indicate where the conversion stopped. 
                    Can be NULL -> convert till ya can`t, 
            int base: Octal, Decimal
            )

    convert to integer - err returns 0
    */
    num_rolls = strtoll(aux_token,NULL , 10);
    printf("N rolls %lld\n", num_rolls);
    
    if (num_rolls == 0)
        return;
        
    aux_token = strtok(NULL,TOKEN_DIE);
    if(aux_token==NULL) //ERROR HANDLING - NO DIE
        return;
    die_type = strtoll(aux_token, NULL, 10);
    if(die_type == 0) //ERROR - INVALID INPUT
        return;

    printf("die d%lld\n", die_type);


    //All after 'd' -> PARSE MODIFIERS & TN
    printf("%d\t%d\t%d\n",mod_inc,mod_dec,target_num);
    parse_modifiers(aux_token, &mod_inc, &mod_dec, &target_num);   
    printf("%d\t%d\t%d\n",mod_inc,mod_dec,target_num);
    
    if(GLOBAL_TARGET_NUM > 0)
        target_num = GLOBAL_TARGET_NUM;

   // roll_dice(num_rolls,die_type,mod_inc,mod_dec,target_num);
    
    return;
}



void roll_dice(uint64_t num_rolls, uint64_t die_type, uint64_t mod_inc, uint64_t mod_dec, uint64_t target_num){
    
//actually runs randm
}

int main(int argc, char **argv){
    int c;

    //Parse Options 
    while( (c = getopt(argc, argv, "ph")) != -1  ){ 
        switch(c){
            case 'p':
                printf("Dice Pool mode\n");
                DICE_POOL_MODE = true;
                break;
            case 't':
                printf("Global Target Number\n");
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
