#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


int prchr(char *s){ 
     while(*s!='\0'){ 
        printf("%c",*s++); 
    } 
    return 0; 
} 
 
int prchr2(char s[]){
    for (int i=0; s[i] != '\0'; i++){
        printf("%c",s[i]); 
    }
    return 0; 
}

int main(int argc, char **argv){
    
    //while not EOL
    //Switch-case to handle arg?
    // -h for help
    // xdy - roll a dy x times, print a result for each line
    // xdy[z] - the same but set a "success" treshold of z, print "pass/failed"

    printf("ARGC - %d\n",argc);
    
    //Isso itera cada um dos argumentos, ao menos da o primeiro char
    for(int i=1; i < argc; i++){
        printf("%s",argv[i]);
        printf("\n");
    }
    return EXIT_SUCCESS;
}
