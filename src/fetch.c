#include "../include/timberborn.h"
#include "../include/helper.h"
#include "../include/cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <strings.h>
#include <time.h>

char* g_ip_addr;

uint16_t g_port;

CURL *curl;

CURLcode res;
    
long response_code;

typedef struct {
    char *data;
    size_t size;
} Buffer;

Buffer global_callback_buffer = {0};

size_t write_callback_curl(void *ptr, size_t size, size_t nmemb, void *userdata)
{

    size_t total = size * nmemb;
    Buffer *buf = (Buffer *)userdata;

    char *new_data = realloc(buf->data, buf->size + total + 1);
    if (!new_data)
        return 0;

    buf->data = new_data;
    memcpy(buf->data + buf->size, ptr, total);

    buf->size += total;
    buf->data[buf->size] = '\0';

    return total;
}

void start_tim_instance(const char* ip_addr, unsigned short port){

    curl = curl_easy_init();

    size_t size = strlen(ip_addr) + 1;

    g_ip_addr = malloc(size);

    memcpy(g_ip_addr, ip_addr, size);
    
    g_port = port;

    char* http_url = NULL;

    asprintf(&http_url, "http://%s:%ui/api/levers", ip_addr, port);

    curl_easy_setopt(curl, CURLOPT_URL, ip_addr);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_curl);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &global_callback_buffer);

    res = curl_easy_perform(curl);

    response_code = 404;

    if(res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    }

}

lever_list get_lever_list(){

    lever_list rtv = {NULL, 0};

    char* http_url = NULL;

    asprintf(&http_url, "http://%s:%u/api/levers", g_ip_addr, g_port);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    if(!global_callback_buffer.data){
        printf("falled to get lever: processing GET request\nURL: %s\n", http_url);
        return rtv;
    }

    cJSON* response_json = cJSON_Parse(global_callback_buffer.data);

    if(!response_json){
        printf("faled to get lever: invalid json\nURL: %s\n", global_callback_buffer.data);
        return rtv;
    }

    rtv.list_size = cJSON_GetArraySize(response_json);

    rtv.list = malloc(rtv.list_size * sizeof(lever_struct));

    for(size_t i = 0; i < rtv.list_size; i++){
        lever_struct* ls = &rtv.list[i];
        cJSON* index_j = cJSON_GetArrayItem(response_json, i);

        if(!index_j) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(index_j, "name");

        cJSON* state = cJSON_GetObjectItemCaseSensitive(index_j, "state");

        cJSON* springReturn = cJSON_GetObjectItemCaseSensitive(index_j, "springReturn");

        if(!name || !state || !springReturn){
            printf("json did not contain valid data");
            continue;
        }

        ls->name = name->valuestring;
        
        if(cJSON_IsTrue(state)) ls->state = true;
        else ls->state = false;

        if(cJSON_IsTrue(springReturn)) ls->springReturn = true;
        else ls->springReturn = false;
    }
    
    return rtv;
    
}

lever_struct get_lever(const char *name_str){

    lever_struct rtv = {NULL, false, false};

    char* http_url = NULL;

    char* name_without_whitespace = parse_white_space(name_str);

    asprintf(&http_url, "http://%s:%u/api/levers/%s", g_ip_addr, g_port, name_without_whitespace);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    if(!global_callback_buffer.data){
        printf("falled to get lever: processing GET request\nurl: %s\n", http_url);
        return rtv;
    }

    cJSON* index_j = cJSON_Parse(global_callback_buffer.data);

    if(!index_j){
        printf("failed to parse json\n");
        return rtv;
    }

    cJSON* name = cJSON_GetObjectItemCaseSensitive(index_j, "name");

    cJSON* state = cJSON_GetObjectItemCaseSensitive(index_j, "state");

    cJSON* springReturn = cJSON_GetObjectItemCaseSensitive(index_j, "springReturn");

    if(!name || !state || !springReturn){
        printf("json did not contain valid data\n");
        return rtv;
    }

    rtv.name = name->valuestring;
        
    if(cJSON_IsTrue(state)) rtv.state = true;
    else rtv.state = false;

    if(cJSON_IsTrue(springReturn)) rtv.springReturn = true;
    else rtv.springReturn = false;

    return rtv;
}

adapter_list get_adapters(){
    adapter_list rtv = {NULL, 0};

    char* http_url = NULL;

    asprintf(&http_url, "http://%s:%u/api/adapters", g_ip_addr, g_port);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    if(!global_callback_buffer.data){
        printf("falled to get lever: processing GET request\n");
        return rtv;
    }

    cJSON* response_json = cJSON_Parse(global_callback_buffer.data);

    if(!response_json){
        printf("faled to get lever: invalid json\n");
        return rtv;
    }

    rtv.list_size = cJSON_GetArraySize(response_json);

    rtv.list = malloc(rtv.list_size * sizeof(adapter_struct));

    for(size_t i = 0; i < rtv.list_size; i++){
        adapter_struct* ls = &rtv.list[i];
        cJSON* index_j = cJSON_GetArrayItem(response_json, i);

        if(!index_j) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(index_j, "name");

        cJSON* state = cJSON_GetObjectItemCaseSensitive(index_j, "state");

        if(!name || !state){
            printf("json did not contain valid data");
            continue;
        }

        ls->name = name->valuestring;
        
        if(cJSON_IsTrue(state)) ls->state = true;
        else ls->state = false;

    }
    
    return rtv;
}

adapter_struct get_adapter(const char *name_str){
    adapter_struct rtv = {NULL, false};

    char* http_url = NULL;

    char* name_without_whitespace = parse_white_space(name_str);

    asprintf(&http_url, "http://%s:%u/api/adapters/%s", g_ip_addr, g_port, name_without_whitespace);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    if(!global_callback_buffer.data){
        printf("falled to get lever: processing GET request\n");
        return rtv;
    }

    cJSON* index_j = cJSON_Parse(global_callback_buffer.data);

    if(!index_j){
        printf("failed to parse json\n");
        return rtv;
    }

    cJSON* name = cJSON_GetObjectItemCaseSensitive(index_j, "name");

    cJSON* state = cJSON_GetObjectItemCaseSensitive(index_j, "state");

    if(!name || !state){
        printf("json did not contain valid data\n");
        return rtv;
    }

    rtv.name = name->valuestring;
        
    if(cJSON_IsTrue(state)) rtv.state = true;
    else rtv.state = false;

    return rtv;
}

void enable_lever(const char* name){
    char* http_url = NULL;

    char* no_ws_name = parse_white_space(name);

    asprintf(&http_url, "http://%s:%u/api/switch-on/%s", g_ip_addr, g_port, no_ws_name);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    printf("%s\n", global_callback_buffer.data);
}

void disable_lever(const char* name){
    char* http_url = NULL;

    char* no_ws_name = parse_white_space(name);

    asprintf(&http_url, "http://%s:%u/api/switch-off/%s", g_ip_addr, g_port, no_ws_name);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    printf("%s\n", global_callback_buffer.data);
}

void change_lever_color_rgb(const char* name, uint8_t r, uint8_t g, uint8_t b){
    char* http_url = NULL;

    char* no_ws_name = parse_white_space(name);

    asprintf(&http_url, "http://%s:%u/api/color/%s/%02X%02X%02X", g_ip_addr, g_port, no_ws_name, r, g, b);

    curl_easy_setopt(curl, CURLOPT_URL, http_url);

    curl_easy_perform(curl);

    printf("%s\n", global_callback_buffer.data);
    printf("\nOffending URL: %s\n", http_url);

}

void clean_up_tim_instance(){
    free(global_callback_buffer.data);

    curl_easy_cleanup(curl);

}
