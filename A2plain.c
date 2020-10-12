/*
 *          a2plain.c
 *      Authors: Nick Murphy and Reed Kass-Mullet
 *      Assignment: Comp40 Homework 3, Locality
 *      Due: October 12
 *      Purpose:
 * 
 * 
 */
#include <string.h>

#include "uarray2.h"
//#include <uarray.h>
#include "a2plain.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include "uarray2.h"
//#include <uarray.h>
//#include <assert.h>
//#include <a2methods.h>

/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/

//#define T UArray2_T
//typedef A2Methods_UArray2 A2;
typedef A2Methods_UArray2 A2;

/*
struct T {
    int width;
    int height;
    int element_size;
    UArray_T array; // UArray of UArrays (2d Version)
};
*/

static A2Methods_UArray2 new(int width, int height, int size)
{
  return UArray2_new(width, height, size);
}

static A2Methods_UArray2 new_with_blocksize(int width, int height, int size, int blocksize)
{
  //TODO: Implement this function and remove the dummy return statement.
  (void) blocksize;
  return UArray2_new(width, height, size);
}

/* TODO: ...many more private (static) definitions follow */
static int blocksize(A2 array) 
{
  (void) array;
  return 1;
}
static int width(A2 array) 
{
  return UArray2_width(array);
}

static int height(A2 array) 
{
  return UArray2_width(array);
}

static int size(A2 array) 
{
  return UArray2_size(array);
}

static void a2free(A2 *array) 
{
  UArray2_free((UArray2_T *) array);
}

static A2Methods_Object *at(A2 array, int i, int j) 
{
  return UArray2_at(array, i, j);
}

typedef void applyfun(int i, int j, UArray2_T array, void *elem, void *cl);

static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
  UArray2_map_row_major(uarray2, (applyfun * )apply, cl);
}

static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
  UArray2_map_col_major(uarray2, (applyfun * )apply, cl);
}

struct small_closure {
  A2Methods_smallapplyfun *apply; 
  void                    *cl;
};

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
  struct small_closure *cl = vcl;
  (void)i;
  (void)j;
  (void)uarray2;
  cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_row_major(a2, apply_small, &mycl);
}

static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
  struct small_closure mycl = { apply, cl };
  UArray2_map_col_major(a2, apply_small, &mycl);
}

static struct A2Methods_T uarray2_methods_plain_struct = {
    new,
    new_with_blocksize,
    a2free,
    width,
    height,
    size,
    blocksize,
    at,
    map_row_major,
    map_col_major,
    NULL,
    map_row_major,
    small_map_row_major,
    small_map_col_major,
    NULL,
    small_map_row_major,

    //small_map_row_major,
    /* ... other functions follow in order,
   *     with NULL for those not implemented ...
   */
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
