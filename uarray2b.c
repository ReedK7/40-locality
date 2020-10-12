/*
 *          uarray2b.c
 *      Authors: Nick Murphy and Reed Kass-Mullet
 *      Assignment: Comp40 Homework 3, Locality
 *      Due: October 12
 *      Purpose:
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
//#include "uarray2.h"
#include "uarray.h"
#include <assert.h>
#include "uarray2b.h"
#include "uarray2.h"
#include <math.h>

#define T UArray2b_T

const int MAX_SIZE = 64000;

struct T {
    int width;
    int height;
    int element_size;
    int block_size;
    UArray_T blocks; // UArray of UArrays (2d Version)
};

extern T UArray2b_new(int width, int height, int size, int blocksize)
{   
    //printf("new\n");
    assert(height > 0);
    assert(width > 0);
    assert(size > 0);
    assert(blocksize > 0);
    //printf("wid th & height ");
    //printf("%d %d\n", width, height);
    //find number of blocks needed
    int num_block_width = (width + blocksize - 1) / blocksize;
    int num_block_height = (height + blocksize - 1) / blocksize;
    //printf("%d \n", num_block_height*num_block_width);
    //printf("new2\n");
    //printf("array size ");
    //printf("%d\n", num_block_width * num_block_height * blocksize);
    UArray_T blocks = UArray_new(blocksize * blocksize * num_block_width * num_block_height, size);
    //printf("new3\n");
    T arrayb = malloc(sizeof(*arrayb));
    assert(arrayb != NULL);
    //printf("new4\n");
    arrayb->width = width;
    arrayb->height = height;
    arrayb->element_size = size;
    arrayb->block_size = blocksize;
    arrayb->blocks = blocks;
    //printf("new5\n");
    return arrayb;
}


extern T UArray2b_new_64K_block(int width, int height, int size)
{
    assert(height > 0);
    assert(width > 0);
    assert(size > 0);
    
    int block_size = 0;
    int capacity = MAX_SIZE / size;
    
    /* If size > Max_size: */
    if(capacity == 0) {
        block_size = 1;
    } else {
        /* Calculate the max block size through int math*/
        block_size = (int) floor (sqrt ((double) block_size));
    }

    T array2b = UArray2b_new(width, height, size, block_size);
    return array2b;

}
extern void UArray2b_free(T *array2b)
{
    //printf("free\n");
    assert(array2b != NULL);
    assert(*array2b != NULL);
    UArray_free(&((*array2b)->blocks));
    free(*array2b);
}
extern int UArray2b_width(T array2b)
{
    //printf("width\n");
    assert(array2b != NULL);
    return array2b->width;
}
extern int UArray2b_height(T array2b)
{
    //printf("height\n");
    assert(array2b != NULL);
    return array2b->height;
}
extern int UArray2b_size(T array2b)
{
    //printf("size\n");
    assert(array2b != NULL);
    return array2b->element_size;
}
extern int UArray2b_blocksize(T array2b)
{
    //printf("blacksize\n");
    assert(array2b != NULL);
    return array2b->block_size;
}

extern void *UArray2b_at(T array2b, int column, int row)
{
    //printf("at\n");
    //printf("%s %d %s %d\n", "col ", column, "row ", row);
    assert(array2b != NULL);
    assert((column >= 0) && (column < array2b->width));
    assert((row >= 0) && (row < array2b->height));
    /* Calculate index */
    int w = array2b->width;
    //int h = array2b->height;
    int bs = array2b->block_size;
    //printf("BS == %d\n", bs);
    int bc = column/bs;
    int br = row/bs;

    int bn = (w + bs - 1)/bs;
    //printf("BN == %d\n", bn);

    int bloc = bc + (bn * br);
    //printf("BLOC == %d\n", bloc);

    int c_block = column % bs;
    int r_block = row % bs;

    int inside_in = c_block + (r_block * bs);
    int index = inside_in + (bloc * bs * bs);
    /* Return pointer to cell at index */
    //printf("index ");
    //printf("%d\n", index);
    return UArray_at(array2b->blocks, index);
}

/* visits every cell in one block before moving to another block */
extern void UArray2b_map(T array2b,
                         void apply(int col, int row, T array2b,
                                    void *elem, void *cl),
                         void *cl)
{
    printf("map\n");
    //assert(height > 0 && height < array2b->height);
    //assert(width > 0 && width < array2b->width);
    //assert(size > 0);
    assert(array2b != NULL);
    assert(apply != NULL);
    for(int i = 0; i < UArray_length((array2b -> blocks)); ++i) {
        int c = calc_Col(i, array2b);
        int r = calc_Row(i, array2b);
        if(c < array2b->width && r < array2b->height)
        {
            apply(c, r, array2b, UArray_at(array2b->blocks, i), cl);
        }
    }
}                            

extern int calc_Col(int index, T array2b)
{
    int bs = array2b->block_size; 
    int w = array2b->width;
    int bloc = index / (bs * bs);
    int inner_index = index - (bloc * bs * bs);
    int inner_Col = inner_index % bs;
    int bn = (w + bs - 1)/bs;
    int bloc_Col = bloc % bn;
    int index_Col = (bloc_Col * bs) + inner_Col;
    return index_Col;
}

extern int calc_Row(int index, T array2b)
{
    int bs = array2b->block_size;
    int h = array2b->height;
    int bloc = index / (bs * bs);
    int inner_index = index - (bloc * bs * bs);
    int inner_Row = inner_index % bs;
    int bn = (h + bs - 1) / bs;
    int bloc_Row = bloc % bn;
    int index_Row = (bloc_Row * bs) + inner_Row;
    return index_Row;
}

#undef T
