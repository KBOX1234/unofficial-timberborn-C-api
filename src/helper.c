#include "../include/helper.h"

#include <stdlib.h>
#include <string.h>

char* parse_white_space(const char* str){
    //get whitespace count
    int wc = 0;

    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == ' ') wc++;
    }

    size_t new_size = strlen(str) + 1;
    new_size = new_size - wc;

    //its 3 because %20 is 3 long
    new_size = new_size + (wc * 3);

    char* new_char = malloc(new_size);

    int j = 0;
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] != ' '){
            new_char[j] = str[i];
            j++;
        }
        else{
            new_char[j + 0] = '%';
            new_char[j + 1] = '2';
            new_char[j + 2] = '0';

            j = j + 3;
        }
    }
    new_char[new_size - 1] = '\0';

    return new_char;
}
