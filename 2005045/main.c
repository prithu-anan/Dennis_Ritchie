#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "utils.h"

Image *read_PPM(char *filename) 
{
    /* opens a PPM file, constructs an Image object and returns a pointer to it.
    Use fopen(), fscanf(), fprintf(), and fclose().*/

    char format[3];
    int max,i=0,j=0;
    Image *img;

    FILE *fp;
    
    fp = fopen(filename, "rb"); 

    img = (Image *) malloc (sizeof(Image));
    
    fscanf(fp, "%s", format);
    fscanf(fp, "%d %d", &img->cols, &img->rows);
    fscanf(fp, "%d", &max);

    img -> image = (Color**) malloc (img -> rows * sizeof(Color*));

    for(i=0; i<img->rows; i++){
        img->image[i] = (Color*)malloc(img->cols*sizeof(Color));
    }

    for(i=0; i<img->rows; i++){
       for(j=0; j<img->cols; j++){
          fscanf(fp,"%d %d %d", &img->image[i][j].R, &img->image[i][j].G, &img->image[i][j].B);
       }
    }    

    // while (!feof(fp)) {   
    //     *img->image = (Color*) malloc(img->cols*img->rows*sizeof(Color));
    //     fread(*img->image, 3 * img->cols, img->rows, fp);
    // }

    fclose(fp);
    return img;

}

void write_PPM(Image *image, char *filename)
{
	/* takes an Image object and writes to filename in PPM format.*/
    FILE *fp;
    fp = fopen(filename, "wb");
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", image->cols, image->rows);
    fprintf(fp,"255\n");

    for(int i=0; i<image->rows; i++){
       for(int j=0; j<image->cols; j++){
          fprintf(fp,"%d %d %d ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
       }
    }    

    // while (!feof(fp)){
    //     fwrite(*image->image, 3 * image->cols, image->rows, fp);

    // }

	
}

void free_image(Image *image)
{
	/* takes an Image object and frees all the memory associated with it.
    This involves not only calling free on image but also on the appropriate
    members of it.
    */
   for(int i=0; i<image->rows; i++){
        free(image->image[i]);
    }
   free(image->image);
   free(image);
}

Color *evaluate_one_pixel(Image *image, int row, int col)
{
	/* Takes an Image object and returns what color the pixel at the given row/col 
    should be in the secret image. This function should not change image*/
    Color *pixel;
    pixel = (Image *) malloc (sizeof(Image));
    if(image->image[row][col].B%2){
        pixel->R = 255;
        pixel->G = 255;
        pixel->B = 255;
    }
    else{
        pixel->R = 0;
        pixel->G = 0;
        pixel->B = 0;
    }
    return pixel;

}

Image *get_secret_image(Image *image)
{
	/* takes an Image object, and constructs the secret image from it by extracting 
    the LSB of the B channel. You should call evaluate_one_pixel() here. */
    int i=0, j=0;
    for(i=0; i<image->rows; i++){
       for(j=0; j<image->cols; j++){
           image->image[i][j].R = evaluate_one_pixel(image, i, j)->R;
           image->image[i][j].G = evaluate_one_pixel(image, i, j)->G;
           image->image[i][j].B = evaluate_one_pixel(image, i, j)->B;
       }
   }
   return image;

}

int main()
{
    /* Call read_PPM(), write_PPM(), free_image(), get_secret_image() in some order
    to obtain the hidden message.*/
    Image *img;
    img = read_PPM("DennisRitchie.ppm");
    get_secret_image(img);
    write_PPM(img,"secret message.ppm");
    free_image(img);

	return 0;
}


