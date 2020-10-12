#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "cputiming.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

FILE *get_file(FILE **file, int argc, char *argv[], int i);
void rotate90(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl);
void rotate180(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl);
void rotate270(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl);

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        FILE *timer = NULL;
        int   rotation       = 0;
        int   i;
        CPUTime_T time;
        double time_used;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
				    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
				    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
					"Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
				argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }
        printf("past into\n");

        FILE *file = NULL;
        FILE *writeTo = fopen("before.ppm", "w");
        file = get_file(&file, argc, argv, i);
        assert(file != NULL);

        Pnm_ppm pic = Pnm_ppmread(file, methods);
        Pnm_ppmwrite(writeTo, pic);
        fclose(writeTo);
        writeTo = fopen("after.ppm", "w");
        int picW = pic->width;
        int picH = pic->height;
        fclose(file);
        
        printf("three\n");

        A2Methods_UArray2 output = methods -> new(pic->width, pic->height, (sizeof (struct Pnm_rgb)));

        time = CPUTime_New();

        printf("Rotation == %d\n", rotation);
        if(rotation == 90){
               CPUTime_Start(time); 
               map(pic->pixels, rotate90, output);
               time_used = CPUTime_Stop(time);
               printf("TIME USED == %f\n", time_used);
               picW = methods->height(pic->pixels);
               picH = methods->width(pic->pixels);
        } else if (rotation == 180) {
                CPUTime_Start(time); 
                map(pic->pixels, rotate180, output);
                time_used = CPUTime_Stop(time);
                printf("TIME USED == %f\n", time_used);
        }
        else if (rotation == 270)
        {
                CPUTime_Start(time); 
                map(pic->pixels, rotate90, output);
                map(pic->pixels, rotate180, output);
                time_used = CPUTime_Stop(time);
                printf("TIME USED == %f\n", time_used);
                picW = methods->height(pic->pixels);
                picH = methods->width(pic->pixels);
                
        }
        if(time_file_name != NULL){
                timer = fopen(time_file_name, "w");
                if(timer != NULL){
                        fprintf(timer, "Rotation completed in %f nanoseconds.\n", 
                                        time_used/(picW * picH));
                        fprintf(timer, "Time per pixel: %f nanoseconds.\n", 
                                time_used/(picW * picH));
                } else{
                        exit(EXIT_FAILURE);
                }
        }
        methods->free(&pic->pixels);
        pic->pixels = output;
        pic->width = picW;
        pic->height = picH;
        Pnm_ppmwrite(writeTo, pic);
        printf("\n5.5\n");
        /* Free pixmap pointed to by ppmp. Does not free methods. */
        Pnm_ppmfree(&pic);
        printf("six\n");
        fclose(writeTo);

        CPUTime_Free(&time);
        return 0;
}

void rotate90(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl)
{
         *(Pnm_rgb) uarray2_methods_plain -> at((A2Methods_UArray2 *)cl, (uarray2_methods_plain -> height(array2b)) - row - 1, col) 
                = *(Pnm_rgb)elem;
}

void rotate180(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl)
{
        *(Pnm_rgb) uarray2_methods_plain -> at((A2Methods_UArray2 *)cl, (uarray2_methods_plain -> width(array2b)) - col - 1,
                                       (uarray2_methods_plain -> height((A2Methods_UArray2 *) cl)) - row - 1) 
                = *(Pnm_rgb)elem;
}

void rotate270(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl)
{
        //*(Pnm_rgb)uarray2_methods_plain->at((A2Methods_UArray2 *)cl, row, (uarray2_methods_plain->width(array2b)) - col - 1) 
        //        = *(Pnm_rgb)elem;
        
        rotate180(row, col, array2b, elem, cl);
        rotate90(row, col, array2b, elem, cl);
        
}

void rotate0(int row, int col, A2Methods_UArray2 array2b, void *elem, void *cl)
{
        (void) row;
        (void) col;
        (void) array2b;
        (void) elem;
        (void) cl;
}

FILE *get_file(FILE **file, int argc, char *argv[], int i)
{
    if(argc - i == 0){
            *file = stdin;
    } else {
            *file = fopen(argv[argc- 1], "r");
            assert(file != NULL);
    }
    return *file; 
}
