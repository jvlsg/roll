#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<sys/time.h>
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
int64_t GLOBAL_TARGET_NUM = 0;
char* TOKEN_DIE = "d";


void parse_modifiers(char * str, int64_t *mod_inc, int64_t *mod_dec, int64_t *target_num){
    //printf("to parse %s\n", str);
    int64_t aux = 0;

    for (uint32_t i = 0; str[i] != '\0'; i++){
        switch (str[i]){
            case '+':
                aux = strtoull( str+i ,NULL,10);
                //printf("\tinc %lld\n",aux);
                *mod_inc+=aux;
                break;
            case '-':
                aux = strtoull( str+i ,NULL,10);
                //printf("\tdec %lld\n",aux);
                *mod_dec+=aux;
                break;
            case '#':
                aux = strtoull( str+i+1 ,NULL,10);
                //printf("\ttn %d\n",aux);
                *target_num=aux;
                break;
            default:
                break;
        }
    }
}

void roll_dice(int64_t num_rolls, int64_t die_type, int64_t mod_inc, int64_t mod_dec, int64_t target_num){
 
    struct timeval tv;

    gettimeofday(&tv, NULL);

   
    int64_t roll_sum = 0;
    int64_t success_count = 0;
    bool has_tn = (target_num > 0) ? true : false;
    bool has_mod = (mod_inc > 0 || mod_dec >0) ? true : false;
    
    printf("\n[");
    
    unsigned long long millisecondsSinceEpoch =  (unsigned long long)(tv.tv_sec) * 1000 +
    (unsigned long long)(tv.tv_usec) / 1000;
    srand(millisecondsSinceEpoch);

    for(int64_t i = 0; i<num_rolls; i++){
        //roll_sum += random_at_most(die_type); //ROLL THE FUCKING DICE;
        printf("%lld", roll_sum); 
        
        if(has_mod){
            roll_sum +=  mod_inc - mod_dec; 
            printf("+%lld %lld ->%lld ",mod_inc, mod_dec, roll_sum);
        }

        if (DICE_POOL_MODE){
            if (roll_sum >= target_num)
                success_count++;
            roll_sum = 0;
        }
    }
    
    //SUCCES OR FAIL ?
    if (has_tn){
        if(DICE_POOL_MODE){
            printf("%lld Successes", success_count);
            return;
        }
        (roll_sum >= target_num) ? printf("Success") : printf("Fail");
    }
    printf("]\n");
}


/*
    Parses each roll, calls roll_dice function with proper arguments
*/
void parse_roll( char *roll ){
    
    int64_t  num_rolls=0, die_type=0, mod_inc=0, mod_dec=0, target_num=0;

    //Break string at 'd' charcter, get num_rolls
    char *aux_token = strtok(roll,TOKEN_DIE);    
    if(aux_token==NULL ) //ERROR HANDLING - NO NUM ROLLS
        return;
    /*
    strtoull - (
            const char *nptr, 
            char **endptr: to indicate where the conversion stopped. 
                    Can be NULL -> convert till ya can`t, 
            int base: Octal, Decimal
            )

    convert to integer - err returns 0
    */
    num_rolls = strtoull(aux_token,NULL , 10);
    printf("#rolls %lld\t",num_rolls); 
    if (num_rolls == 0)
        return;
        
    aux_token = strtok(NULL,TOKEN_DIE);
    if(aux_token==NULL ) //ERROR HANDLING - NO DIE
        return;
    
    die_type = strtoull(aux_token, NULL, 10);
    if(die_type <= 0) //ERROR - INVALID INPUT
        return;

    printf("die type %lld\t",die_type); 
    
    //All after 'd' -> PARSE MODIFIERS & TN
    parse_modifiers(aux_token, &mod_inc, &mod_dec, &target_num);   
    printf("inc %d\t dec %d\t tn %d\n",mod_inc,mod_dec,target_num);
    
    if(GLOBAL_TARGET_NUM > 0)
        target_num = GLOBAL_TARGET_NUM;

    roll_dice(num_rolls,die_type,mod_inc,mod_dec,target_num);
    
    return;
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
     
    //optiond -> special external variable of getopt
    for(int i=optind; i < argc; i++){
        parse_roll(argv[i]);
    }
    return EXIT_SUCCESS;
}
