#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>

/*
    roll - A simple Diceroller in C for RPG and CLI enthusiasts
    by João Victor Guimarães (jvlsg)
*/

bool DICE_POOL_MODE = false;
bool VERBOSE_MODE = false; 
int64_t DEFAULT_TARGET_NUM = 0;
char* TOKEN_DIE = "d";

char* VERSION_STR="v1.0\n";
char* HELP_STR = 
"\nUsage\n"
"roll [options] XdY[+A-B#Z] ...\n"
"\tX is Number of Rolls\n"
"\tY is the type of die\n" 
"\tA and B are optional increments and decrements, respectively\n" 
"\tZ is a Target Number. A roll will be successful if Result >= TN \n"
"\nPlease note that roll fails quietly - Incorrect input will simply be ignored\n"

"\nOPTIONS\n"
"\t-p\tDice Pool mode: Each die of a roll is independent of the others\n"
"\t-v\tVerbose mode\n"
"\t-t<NUM>\tDefault Target Number: All rolls will be compared against it.An explicit target number declaration will be"
"used instead for that roll.\n"
"\t-V\tPrints out the program's version\n"

"\nEXAMPLES\n"
"\troll 1d20+2-4#14 2d8#4\n"
"\troll -p 6d6#5\n"

"\nAUTHOR:\tJoão Victor Guimarães\nBugs and feedback should be sent to https://github.com/jvlsg/roll/issues \n"
"\n";

/*
Parse a string for modifiers: Increment, Decrement and target numbers.
Ignores trash input by default.

@str            String including the modifiers to be parsed
@mod_inc        Reference to store Increment modifier
@mod_dec        Reference to store Decrement modifier
@target_num     Reference to store Target Number
*/
void parse_modifiers(char * str, int64_t *mod_inc, int64_t *mod_dec, int64_t *target_num){
    int64_t aux = 0;
    for (uint32_t i = 0; str[i] != '\0'; i++){
        switch (str[i]){
            case '+':
                aux = strtoll( str+i ,NULL,10);
                *mod_inc+=aux;
                break;
            case '-':
                aux = strtoll( str+i ,NULL,10);
                *mod_dec-=aux;
                break;
            case '#':
                aux = strtoll( str+i+1 ,NULL,10);
                *target_num=aux;
                break;
            default:
                break;
        }
    }
}

/*
return random integer in a range of [min,max]

Thanks to the fine folks at yale
http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html

@min    Lower boundry of the range - inclusive
@max    Upper boundry of the range - inclusive

@return Random number in the boundry
*/
int64_t random_in_range(int64_t min, int64_t max){
    int64_t r = 0;
    //
    int64_t limit = RAND_MAX - (RAND_MAX % max);
    while ( (r = rand() ) >= limit );
    return (r % max)+1;
}

/*
Applies modifiers and prints them according to the verbosity level
@mod_inc        Increment modifier
@mod_dec        Decrement modifier
@roll_sum       Reference to the overall sum of the roll
*/
void apply_modifiers(int64_t mod_inc, int64_t mod_dec, int64_t *roll_sum){
    
    *(roll_sum) +=  mod_inc - mod_dec; 
    
    if(VERBOSE_MODE)
        printf("+%ld-%ld=%ld",mod_inc, mod_dec, *(roll_sum) );
    else
        printf(" -> %ld", *(roll_sum));
    return;
}


/*
Rolls dice given a die type, number of rolls, increment and decrement   

If in DICE_POOL_MODE, all dice are treated independently and each have their
roll result modified by the mod_* variables, and compared to the target number
if the TN is different than 0

If not in the DICE_POOL_MODE, all
rolls have their results summed and ONLY the final result is modified by mod_*
and compared to the target number, if the TN is different than 0.

@num_rolls      Number of rolls, or loop iterations to run
@die_type       Upper boundry of the random range
@mod_inc        Increment modifier
@mod_dec        Decrement modifier
@target_num     Target Number
*/
void roll_dice(int64_t num_rolls, int64_t die_type, int64_t mod_inc, int64_t mod_dec, int64_t target_num){
    
    srand(time(0));
    int64_t roll_sum = 0;
    int64_t success_count = 0;
    bool has_tn = (target_num > 0) ? true : false;
    bool has_mod = (mod_inc > 0 || mod_dec >0) ? true : false;
    
    printf("[");
    
    
    if(DICE_POOL_MODE){

        for(int64_t i = 0; i<num_rolls; i++){
            roll_sum += random_in_range(1,die_type); 
            printf("%ld", roll_sum);

            if(has_mod)
                apply_modifiers(mod_inc, mod_dec, &roll_sum);

            if (roll_sum >= target_num)
                success_count++;

            if(i+1<num_rolls)
                printf(" , "); 

            roll_sum = 0;
        }
        if(has_tn)
            printf(" | %ld Successes", success_count);
    }

    else{
        for(int64_t i = 0; i<num_rolls; i++)
            roll_sum += random_in_range(1,die_type); 

        printf("%ld", roll_sum);
        if(has_mod)
            apply_modifiers(mod_inc,mod_dec, &roll_sum);
        if(has_tn) 
            (roll_sum >= target_num) ? printf(" | Success") : printf(" | Fail");
    }
    
    printf("]\t");
    if(VERBOSE_MODE)
        printf("\n");
    
    return;
}


/*
Parses a single 'roll', calls roll_dice function with proper arguments

The roll is expected to be in the form of 'xdy+a-b#z' where x,y,z,a,b are ints
Using strtoll it ignores any non numerical input

"strtoull - (
        const char *nptr, 
        char **endptr: to indicate where the conversion stopped. 
                Can be NULL -> convert till ya can`t, 
        int base: Octal, Decimal
        )
convert to integer - err returns 0"

@roll   String containing the roll

@return EXIT_SUCCESS if good roll, EXIT_FAILURE otherwise
*/
int parse_roll( char *roll ){
    
    int64_t  num_rolls=0, die_type=0, mod_inc=0, mod_dec=0, target_num=0;

    //Break string at 'd' charcter, get num_rolls
    char *aux_token = strtok(roll,TOKEN_DIE);    
    if(aux_token == NULL ) //ERROR HANDLING - NO NUM ROLLS
        return EXIT_FAILURE;
    num_rolls = strtoll(aux_token,NULL , 10);
    if (num_rolls == 0)
        return EXIT_FAILURE;
        
    aux_token = strtok(NULL,TOKEN_DIE);
    if(aux_token == NULL ) //ERROR HANDLING - NO DIE
        return EXIT_FAILURE;
    
    die_type = strtoll(aux_token, NULL, 10);
    if(die_type <= 0) //ERROR - INVALID INPUT
        return EXIT_FAILURE;

    //All after 'd' -> PARSE MODIFIERS & TN
    parse_modifiers(aux_token, &mod_inc, &mod_dec, &target_num);   
    
    if(target_num <= 0)
        target_num = DEFAULT_TARGET_NUM;

    if(VERBOSE_MODE)
        printf("#rolls %ld \td%ld\tinc %ld\t dec %ld\t tn %ld\n",num_rolls,die_type,mod_inc,mod_dec,target_num);
    
    roll_dice(num_rolls,die_type,mod_inc,mod_dec,target_num);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv){
    int c;

    //Parse Options 
    while( (c = getopt(argc, argv, "pVvt:h")) != -1  ){ 
        switch(c){
            case 'p':
                DICE_POOL_MODE = true;
                break;
            case 'v':
                VERBOSE_MODE = true;
                break;
            case 't':
                DEFAULT_TARGET_NUM = strtoll(optarg,NULL,10);
                if(VERBOSE_MODE)
                    printf("Default Target Number %ld\n",DEFAULT_TARGET_NUM);
                break;
            case 'h':
                printf("%s",HELP_STR);
                return EXIT_SUCCESS;
            case 'V':
                printf("%s",VERSION_STR);
                return EXIT_SUCCESS;
            default:
                break;
        }
    }
     
    //optiond -> special external variable of getopt
    if (optind==argc)
        printf("%s",HELP_STR);

    for(int i=optind; i < argc; i++){
        parse_roll(argv[i]);
    }
    if (!VERBOSE_MODE)
        printf("\n");
    return EXIT_SUCCESS;
}
