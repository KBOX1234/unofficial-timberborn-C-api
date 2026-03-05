#include <stdio.h>

#include "../include/timberborn.h"

int main(){
    printf("Hello world!\n");

    start_tim_instance("localhost", 8080);

    lever_list ll = get_lever_list();

    for(size_t i = 0; i < ll.list_size; i++){
        printf("%s\n", ll.list[i].name);
    }

    if(ll.list_size > 0){
        printf("turning on lever: %s\n", ll.list[0].name);

        enable_lever(ll.list[0].name);

        change_lever_color_rgb(ll.list[0].name, 128, 1, 1);
    }
    return 0;
}
