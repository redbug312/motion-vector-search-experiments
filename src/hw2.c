#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "frame.h"

#if __GNUC__ < 3
#define __builtin_expect(x, n) (x)
#endif

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)


double PSNR(Frame *frameA, Frame *frameB) {
    uint8_t *pA = frameA->pixels[0], *pB = frameB->pixels[0];
    size_t width = frameA->width;
    size_t height = frameA->height;

    int MSE = 0, diff;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++, pA++, pB++) {
            diff = abs(*pA - *pB);
            MSE += diff * diff;
    }
    MSE /= (double) width * height;
    return 10 * log10(255.0 * 255.0 / MSE);
}

int main(int argc, char *argv[]) {
    size_t width = 352, height = 288;
    FILE *fptr = fopen("data/Foreman.CIF", "rb");

    Frame frame[2];
    create_frame(&frame[0], width, height);
    create_frame(&frame[1], width, height);

    for (int fi = 0; fi < 300; fi++) {
        int curr = fi & 0x1;
        int prev = curr ^ 0x1;

        read_into_frame(&frame[curr], fptr);

        if (likely(fi)) {
            printf("Frame: %d\tPSNR: %f\n", fi, PSNR(&frame[0], &frame[1]));
        }
    }

    destroy_frame(&frame[0]);
    destroy_frame(&frame[1]);
}
