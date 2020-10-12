*
 *     filename: uarray2.h
 *     by Reed Kass-Mullet and Jerry Wang, 10/01/20
 *     HW2 - iii
 *
 *     Contains the header for our UArray2 implementation
 */ 

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define T UArray2_T
typedef struct T *T;
/*
 *               UArray2_new
 *
 *   Purpose:
 *
 *     Allocates a UArray2 on the heap initialized with given width,
 *   height and size. 
 *
 *   Parameters:
 * 
 *     DIM1 (WIDTH):   Integer that specifies UArray2 width
 *     DIM2 (HEIGHT):   Integer that specifies UArray2 height
 *     ELEMENT_SIZE (SIZE): Integer that specifies the size of memory 
 *           allocated for each piece of data
 * 
 *  Returns: A pointer to a Uarray2_T 
 */
T UArray2_new(int width, int height, int size);

/* 
 *               UArray2_width
 *
 *   Purpose:
 *
 *     Returns the width of the 2D array 
 *
 *   Parameters:
 *
 *     ARRAY: Pointer to Uarray2_T whose width is to be returned
 *     (Assert ARRAY is not NULL)
 * 
 *  Returns: Integer equal to the width of the Uarray2_T which was passed in
 */ 
int UArray2_width(T array);

/*
 *               UArray2_height
 *
 *   Purpose:
 *
 *     Returns the height of the 2D array 
 *
 *   Parameters:
 *
 *     ARRAY: Pointer to Uarray2_T whose height is to be returned
 *     (Assert ARRAY is not NULL)
 * 
 *  Returns: Integer equal to the height of the Uarray2_T which was passed in
 */
int UArray2_height(T array);

/*
 *               UArray2_size
 *
 *   Purpose:
 *
 *     Returns the size (in bytes, memory) of each element in the 2D array 
 *
 *   Parameters:
 *
 *     ARRAY: Pointer to Uarray2_T whose element size is to be returned
 *     (Assert ARRAY is not NULL)
 * 
 *  Returns: Integer equal to the element size of the Uarray2_T which was 
 *           passed in
 */
int UArray2_size(T array);

/*
 *               UArray2_at
 *
 *   Purpose:
 *
 *     Returns void pointer to element stored at a given point in the 2D array 
 *
 *   Parameters:
 *
 *     ARRAY: Pointer to Uarray2_T whose elements are being accessed
 *     (Assert ARRAY is not NULL)
 *
 *     DIM1 (COL): The column index that the user wants to access in UArray2
 *     
 *     DIM2 (ROW): The row index that the user wants to access in UArray2
 *     (Assert both dimensions within the dimensions of the 2d array)
 * 
 *  Returns: a void pointer which represents the value stored at the given
 *           dimensions (DIM1 and DIM2) within the 2D array
 */
void *UArray2_at(T array, int col, int row);

/*
 *               UArray2_map_col_major
 *
 *   Purpose:
 *
 *     Performs a map (calls a given function on every point in the array) 
 *     by iterating through the column first and then going to the next row.
 *
 *   Parameters:
 *
 *     ARRAY: Pointer to Uarray2_T whose elements are being mapped
 *    (Assert ARRAY is not NULL)
 * 
 *     Function: The void function pointer which is being called on each 
 *               element which has these parameters: 
 *               (int i, int j, UArray2_T a, void *p1, void *p2)
 *     
 *     Void Pointer: Void pointer that points to a Flex variable
 *
 * 
 *  Returns: Nothing
 */
void UArray2_map_col_major(T array, void (*func)(int i, int j, UArray2_T a, 
                      void *p1, void *p2), void *flex);

/*
 *               UArray2_map_row_major
 *
 *   Purpose:
 *
 *     Performs a map (calls a given function on every point in the array) 
 *     on the array by iterating through the row first and column second.
 *
 *   Parameters:
 *
 *     ARRAY: Pointer to Uarray2_T whose elements are being mapped
 *     (Assert ARRAY is not NULL)
 *
 *     Function: The void function pointer which is being called on each 
 *               element which has these parameters: 
 *                   (int i, int j, UArray2_T a, void *p1, void *p2)
 *
 *     
 *     Void Pointer: Void pointer that points to a Flex variable
 *
 * 
 *  Returns: Nothing
 */
void UArray2_map_row_major(T array, void (*func)(int i, int j, UArray2_T a, 
                       void *p1, void *p2), void *flex);

/*
 *               UArray2_free
 *
 *   Purpose:
 *
 *     Frees all heap allocated memory used by the Uarray2_T structure
 *
 *   Parameters:
 *
 *     ARRAY: Uarray2_T which is having its heap memory freed
 * 
 *  Returns: Nothing
 */
void UArray2_free(T* array);
 
 
#undef T
#endif

