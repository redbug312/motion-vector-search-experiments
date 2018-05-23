#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "frame.h"

void create_frame(Frame *frame, size_t width, size_t height) {
    frame->raw          = malloc(sizeof(uint8_t) * width * height * 3 >> 1);
    frame->pixels[0]    = frame->raw;
    frame->pixels[1]    = frame->pixels[0] + width * height;
    frame->pixels[2]    = frame->pixels[1] + ((width * height) >> 2);
    frame->width        = width;
    frame->height       = height;
    frame->stride       = width;
    frame->mb_size_log2 = 3;
}

void destroy_frame(Frame *frame) {
    free(frame->raw);
}

inline int read_into_frame(Frame *frame, FILE *fptr) {
    return fread(frame->raw, 1, frame->width * frame->height * 3 >> 1, fptr);
}

inline const uint8_t *get_frame_at_pos(Frame *frame, int cIdx, int xpos, int ypos) {
    return frame->pixels[cIdx] + (xpos + ypos * frame->stride) * sizeof(uint8_t);
}
