#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#if __GNUC__ < 3
#define __builtin_expect(x, n) (x)
#endif

#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)

typedef struct Frame {
    uint8_t *raw;
    uint8_t *pixels[3];
    size_t width, height, stride;
} Frame;

void create_frame(Frame *frame, size_t width, size_t height) {
    frame->raw       = malloc(sizeof(uint8_t) * width * height * 1.5);
    frame->pixels[0] = frame->raw;
    frame->pixels[1] = frame->raw + width * height;
    frame->pixels[2] = frame->raw + ((width * height * 5) >> 2);
    frame->width     = width;
    frame->height    = height;
    frame->stride    = width;
}

void destroy_frame(Frame *frame) {
    free(frame->raw);
}

inline int read_into_frame(Frame *frame, FILE *fptr) {
    return fread(frame->raw, 1, frame->width * frame->height * 1.5, fptr);
}

inline const uint8_t *get_image_at_pos(Frame *frame, int cIdx, int xpos, int ypos) {
    return frame->pixels[cIdx] + (xpos + ypos * frame->stride) * sizeof(uint8_t);
}

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
