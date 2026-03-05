#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct lever_struct{
    char* name;
    bool state;
    bool springReturn;
}lever_struct;

typedef struct adapter_struct{
    char* name;
    bool state;
}adapter_struct;

typedef struct adapter_list{
    adapter_struct* list;
    size_t list_size;
}adapter_list;

typedef struct lever_list{
    lever_struct* list;
    size_t list_size;
} lever_list;

// Main functions

void start_tim_instance(const char* ip_addr, unsigned short port);

lever_list get_lever_list();

lever_struct get_lever(const char* name);

adapter_list get_adapter_list();

adapter_struct get_adapter(const char* name);

void enable_lever(const char* name);

void disable_lever(const char* name);

void change_lever_color_rgb(const char* name, uint8_t r, uint8_t g, uint8_t b);

//Clean up functions

void clean_up_tim_instance();

void destroy_lever_struct(lever_struct* ls);

void destroy_adapter_struct(adapter_struct* as);

void destroy_lever_list(lever_list* ll);

void destroy_adapter_list(adapter_list* al);

#endif // !BASE_H
