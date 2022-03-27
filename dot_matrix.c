#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "dot_matrix.h"

void copy_Data(int *array, int *source){
    for(int i = 0; i < 32;i++){
        array[i] = source[i];
    }
}

void get_Num(int *array, int num){
    if(num == 0) copy_Data(array,n_0);
    else if(num == 1) copy_Data(array,n_1);
    else if(num == 2) copy_Data(array,n_2);
    else if(num == 3) copy_Data(array,n_3);
    else if(num == 4) copy_Data(array,n_4);
}

void get_Display_Data(int *array,int status){
    int data_array[32] = {};
    int temp_array[32] = {};
    if(status == 5){
        copy_Data(temp_array,n_N);
        shift_character_horizontal(temp_array,temp_array,0,9);
        merge_disp_data_or(temp_array, data_array, data_array);
    }else if(status < 5){
        copy_Data(temp_array,n_B);
        shift_character_horizontal(temp_array,temp_array,0,4);
        merge_disp_data_or(temp_array, data_array, data_array);

        int num = 4 - status;
        get_Num(temp_array,num);
        shift_character_horizontal(temp_array,temp_array,0,18);
        merge_disp_data_or(temp_array, data_array, data_array);
    }else{
        copy_Data(temp_array,n_P);
        shift_character_horizontal(temp_array,temp_array,0,4);
        merge_disp_data_or(temp_array, data_array, data_array);

        int num = status - 6;
        get_Num(temp_array,num);
        shift_character_horizontal(temp_array,temp_array,0,18);
        merge_disp_data_or(temp_array, data_array, data_array);
    }

    for(int i = 0; i < 32;i++){
        array[i] = data_array[i];
    }
}

void set_Display(int *disp_data,char negative){
    for(int data_count_i = 0; data_count_i < 32;data_count_i++){
        int data = disp_data[data_count_i];
        if(negative) data = ~data;
        char data_h = (char)(data >> 8);
        char data_l = (char)(data);
        uart_putc(uart0,data_h);
        while(!uart_is_writable(uart0));
        uart_putc(uart0,data_l);
        while(!uart_is_writable(uart0));  
    }
    uart_putc(uart0,0xFF);
    while(!uart_is_writable(uart0));
}

void clear_display(){
    for(int data_count_i = 0; data_count_i < 32;data_count_i++){
        uart_putc(uart0,0x00);
        while(!uart_is_writable(uart0));
        uart_putc(uart0,0x00);
        while(!uart_is_writable(uart0));  
    }
    uart_putc(uart0,0x00);
    while(!uart_is_writable(uart0));
}

/**
 * @brief 
 * 
 * @param data 
 * Source Data
 * 
 * @param r_data 
 * Result array
 * 
 * @param direction 
 * 0 : left
 * 1 : right
 * 
 * @param shifts 
 */
void shift_character_horizontal(int* data,int *r_data,char direction,char shifts){
    for(int i = 0; i < 16;i++){
        long one_row = (long)data[i] << 16 | data[i+16];
        if(direction == 0){
            one_row = one_row >> shifts;
            one_row = (0xFFFFFFFF >> shifts) & one_row;
        }else{
            one_row = one_row << shifts;
            one_row = (0xFFFFFFFF << shifts) & one_row;
        }
        r_data[i] = (int)(0xFFFF&(one_row >> 16));
        r_data[i+16] = (int)(0xFFFF&one_row);
    }
}

/**
 * @brief 
 * Shifts data to r_data
 * 
 * @param data 
 * Source
 * 
 * @param r_data 
 * Result
 * 
 * @param direction 
 * 0 : UP
 * 1 : DOWN
 * 
 * @param shifts 
 */
void shift_character_vertical(int* data,int *r_data,char direction,char shifts){
    int temp_data[32];
    int move_internal = shifts;
    if(direction == 1) move_internal = -move_internal;
    for(int i = 0; i < 16;i++){
        if(i+move_internal > 15 || i+move_internal < 0){
            int zero_set = (i+move_internal) % 16;
            if(direction) zero_set = 16 - ((-move_internal - i)  % 16);//int zero_set = 16 + ((move_internal + i)  % 16); //DOESNT WORK!!!
            temp_data[zero_set] = 0x00;
            temp_data[zero_set + 16] = 0x00;
            continue;
        }
        temp_data[i + move_internal] = data[i];
        temp_data[i + move_internal + 16] = data[i + 16];
    }
    for(int ix = 0;ix < 32;ix++){
        r_data[ix] = temp_data[ix];
    }
}

/**
 * @brief 
 * 
 * @param d1 
 * Source 1
 * 
 * @param d2 
 * Source 2
 * 
 * @param r_data 
 * Result
 */
void merge_disp_data_or(int* d1,int* d2,int *r_data){
    for(int i = 0;i < 32;i++){
        r_data[i] = d1[i] | d2[i];
    }
}