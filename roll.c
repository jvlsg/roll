#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


int main(int argc, char *argv[]){
    
    //while not EOL
    //Switch-case to handle arg?
    // -h for help
    // xdy - roll a dy x times, print a result for each line
    // xdy[z] - the same but set a "success" treshold of z, print "pass/failed"

    printf("%d\n",argc);
    
    //Isso itera cada um dos argumentos, ao menos da o primeiro char
    for(int i=1; i < argc; i++){
        //dar um jeito de iterar por cima dos args
        printf("%c\n",*(argv[i]));
    }
    //printf("%c\n",*(argv[1]+1));
    return EXIT_SUCCESS;
}
