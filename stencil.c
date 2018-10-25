#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Define output file name
#define OUTPUT_FILE "stencil.pgm"

void stencil(const int nx, const int ny, float *  image, float *  tmp_image);
void init_image(const int nx, const int ny, float *  image, float *  tmp_image);
void output_image(const char * file_name, const int nx, const int ny, float *image);
double wtime(void);

int main(int argc, char *argv[]) {

  // Check usage
  if (argc != 4) {
    fprintf(stderr, "Usage: %s nx ny niters\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Initiliase problem dimensions from command line arguments
  int nx = atoi(argv[1]);
  int ny = atoi(argv[2]);
  int niters = atoi(argv[3]);

  // Allocate the image
  float *image = malloc(sizeof(float)*(nx+2)*(ny+2));
  float *tmp_image = malloc(sizeof(float)*(nx+2)*(ny+2));

  // Set the input image
  init_image(nx, ny, image, tmp_image);

  // Call the stencil kernel
  double tic = wtime();
  for (int t = 0; t < niters; ++t) {
    stencil(nx, ny, image, tmp_image);
    stencil(nx, ny, tmp_image, image);
  }
  double toc = wtime();


  // Output
  printf("------------------------------------\n");
  printf(" runtime: %lf s\n", toc-tic);
  printf("------------------------------------\n");

  output_image(OUTPUT_FILE, nx, ny, image);
  free(image);
}

void stencil(const int nx, const int ny, float *  image, float *  tmp_image) {
  int line = ny + 2;

  // Rest
  for (int i = 1; i <= nx; ++i) {
    int base = i * line;
    int curr = 0;

    for (int j = 1; j <= ny; ++j) {
      curr = base + j;

      tmp_image[curr] = image[curr - line] + image[curr + line] + image[curr - 1] + image[curr + 1];

      tmp_image[curr] *= 0.1f;
      tmp_image[curr] += image[curr] * 0.6f;
    }
  }
}

// Create the input image
void init_image(const int nx, const int ny, float *  image, float *  tmp_image) {
  // Zero everything
  for (int j = 0; j < ny + 2; ++j) {
    for (int i = 0; i < nx + 2; ++i) {
      image[j+i*(ny+2)] = 0.0;
      tmp_image[j+i*(ny+2)] = 0.0;
    }
  }

  // Checkerboard
  for (int j = 0; j < 8; ++j) {
    for (int i = 0; i < 8; ++i) {
      for (int jj = 1 + j*ny/8; jj <= (j+1)*ny/8; ++jj) {
        for (int ii = 1 + i*nx/8; ii <= (i+1)*nx/8; ++ii) {
          if ((i+j)%2)
          image[jj+ii*(ny+2)] = 100.0;
        }
      }
    }
  }
}

// Routine to output the image in Netpbm grayscale binary image format
void output_image(const char * file_name, const int nx, const int ny, float *image) {

  // Open output file
  FILE *fp = fopen(file_name, "w");
  if (!fp) {
    fprintf(stderr, "Error: Could not open %s\n", OUTPUT_FILE);
    exit(EXIT_FAILURE);
  }

  // Ouptut image header
  fprintf(fp, "P5 %d %d 255\n", nx, ny);

  // Calculate maximum value of image
  // This is used to rescale the values
  // to a range of 0-255 for output
  double maximum = 0.0;
  for (int j = 1; j <= ny; ++j) {
    for (int i = 1; i <= nx; ++i) {
      if (image[j+i*(ny+2)] > maximum)
        maximum = image[j+i*(ny+2)];
    }
  }

  // Output image, converting to numbers 0-255
  for (int j = 1; j <= ny; ++j) {
    for (int i = 1; i <= nx; ++i) {
      fputc((char)(255.0*image[j+i*(ny+2)]/maximum), fp);
    }
  }

  // Close the file
  fclose(fp);

}

// Get the current time in seconds since the Epoch
double wtime(void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec*1e-6;
}
