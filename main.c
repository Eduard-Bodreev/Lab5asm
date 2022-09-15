#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <sys/time.h>
#include "lodepng.h"


void transform_png_red(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w) {
        for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                        out[w * i + j] = in[(w * i + j) * 4 + 0];
                }
        }
}
void transform_png_green(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w) {
        for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                        out[w * i + j] = in[(w * i + j) * 4 + 1];
                }
        }
}
void transform_png_blue(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w) {
        for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                        out[w * i + j] = in[(w * i + j) * 4 + 2];
                }
        }
}

void transform_png_asm_red(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w);
void transform_png_asm_green(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w);
void transform_png_asm_blue(unsigned char *in, unsigned char *out, unsigned int h, unsigned int w);


unsigned char * load_png(char *filename, unsigned int *h, unsigned int *w) {
        unsigned err;

        unsigned char* png;
        size_t pngsize;
        LodePNGState state;

        unsigned char* data;

        lodepng_state_init(&state);

        state.info_png.color.colortype = LCT_RGB;

        png = data = NULL;

        err = lodepng_load_file(&png, &pngsize, filename);
        if(!err) {
                err = lodepng_decode(&data, w, h, &state, png, pngsize);
        }
        if(err) {
                lodepng_state_cleanup(&state);
                error(5, 0, "%s", lodepng_error_text(err)); 
        }

        free(png);
        lodepng_state_cleanup(&state);

        return data;
}

void store_png(char *filename, unsigned char *data, unsigned int h, unsigned int w) {
        unsigned err;

        unsigned char* png;
        size_t pngsize;
        LodePNGState state;

        lodepng_state_init(&state);

        state.info_raw.colortype = LCT_GREY;

        err = lodepng_encode(&png, &pngsize, data, w, h, &state);
        if(!err) {
                err = lodepng_save_file(png, pngsize, filename);
        }

        if(err) {
                lodepng_state_cleanup(&state);
                error(6, 0, "%s", lodepng_error_text(err));
        }

        lodepng_state_cleanup(&state);
        free(png);
}

static double get_time() {
        struct timeval t;
        gettimeofday(&t, NULL);

        return t.tv_sec + t.tv_usec * 1e-6;
}

int main(int argc, char *argv[]) {
        unsigned int height;
        unsigned int width;
        unsigned char *img_buf = NULL;
        unsigned char *r_img_buf = NULL;
        double timer_start;
	int runs;

        if (argc != 4 && argc != 5) {
                fprintf(stderr, "usage: %s <input> <output> <color> [runs]\n", argv[0]);
                exit(-1);
        }

	if (argc == 5) {
		if (sscanf(argv[4], "%i", &runs) != 1) {
			error(1, errno, "Could not parse int '%s'.\n", argv[3]);
		}
	} else {
		runs = 1;
	}


        img_buf = load_png(argv[1], &height, &width);
	r_img_buf = (unsigned char *)malloc(width * height);

        timer_start = get_time();
        for (int i = 0; i < runs; i++) {
#ifdef IMPL_ASM
                switch(tolower(argv[3][0])) {
                        case 'r':
                                transform_png_asm_red(img_buf, r_img_buf, height, width);
                                break;
                        case 'g':
                                transform_png_asm_green(img_buf, r_img_buf, height, width);
                                break;
                        case 'b':
                                transform_png_asm_blue(img_buf, r_img_buf, height, width);
                                break;
                        default:
                                error(45, 0, "no such color %s", argv[3]);
                }
#else
                switch(tolower(argv[3][0])) {
                        case 'r':
                                transform_png_red(img_buf, r_img_buf, height, width);
                                break;
                        case 'g':
                                transform_png_green(img_buf, r_img_buf, height, width);
                                break;
                        case 'b':
                                transform_png_blue(img_buf, r_img_buf, height, width);
                                break;
                        default:
                                error(45, 0, "no such color %s", argv[3]);
                }

#endif
        }
        printf("Time elapsed: %lf\n", get_time() - timer_start);


        store_png(argv[2], r_img_buf, height, width);

        free(img_buf);
	free(r_img_buf);
}
