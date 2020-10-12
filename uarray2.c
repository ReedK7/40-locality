/*
 *     filename: uarray2.c
 *     by Reed Kass-Mullet and Jerry Wang, 10/1/20
 *     HW2 - iii
 *
 *    contains the functions and structs for uarray2
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <uarray2.h>
#include <assert.h>
#include <uarray.h>

#define T UArray2_T

struct T {
    int width;
    int height;
    int element_size;
    UArray_T columns; // UArray of UArrays (2d Version)
};

T UArray2_new(int width, int height, int size) 
{
    assert(height > 0);
    assert(width > 0);
    assert(size > 0);

    T array = malloc(sizeof(*array));
    assert(array != NULL);

    UArray_T columns = UArray_new(width, height * size);
    for (int i = 0; i < width; i++) {
        UArray_T row = UArray_new(height, size);
        *(UArray_T *) UArray_at(columns, i) = row;
    }

    array->width = width;
    array->height = height;
    array->element_size = size;
    array->columns = columns;

    return array;
}

int UArray2_width(T array) 
{
    assert(array != NULL);
    return array->width;
}

int UArray2_height(T array) 
{
    assert(array != NULL);
    return array->height;
}

int UArray2_size(T array) 
{
    assert(array != NULL);
    return array->element_size;
}

void *UArray2_at(T array, int col, int row) 
{
    assert(array != NULL);
    assert((col >= 0) && (col < array->width));
    assert((row >= 0) && (row < array->height));

    return UArray_at(*((UArray_T *) UArray_at(array->columns, col)), row);
}

void UArray2_map_col_major(T array, void (*func)(int i, int j, UArray2_T a, 
                            void *p1, void *p2), void *flex) 
{
    assert(array != NULL);
    assert(func != NULL);

    for (int i = 0; i < array->width; i++) {
        for (int j = 0; j < array->height; j++) {
            func(i, j, array, 
                 UArray_at(*(UArray_T *) UArray_at(array->columns, i), j), 
                          flex);
        }
    }
}

void UArray2_map_row_major(T array, void (*func)(int i, int j, UArray2_T a, 
                           void *p1, void *p2), void *flex) 
{
    assert(array != NULL);
    assert(func != NULL);

    for (int i = 0; i < array->height; i++) {
        for (int j = 0; j < array->width; j++) {
            func(j, i, array, 
                 UArray_at(*(UArray_T *) UArray_at(array->columns, j), i), 
                           flex);
        }
    }
}

void UArray2_free(T* array) 
{
    assert(array != NULL);

    for (int i = 0; i < (*array)->width; i++) {
        UArray_free((UArray_T *) (UArray_at((*array)->columns, i)));
    }
    UArray_free(&((*array)->columns));
    free(*array);
}
