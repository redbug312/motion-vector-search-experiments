#ifndef FRAME_H
#define FRAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Frame {
    uint8_t *raw;
    uint8_t *pixels[3];
    size_t width, height, stride;
    size_t mb_size_log2;
} Frame;

void create_frame(Frame *frame, size_t width, size_t height);
void destroy_frame(Frame *frame);
int read_into_frame(Frame *frame, FILE *fptr);
const uint8_t *get_frame_at_pos(Frame *frame, int cIdx, int xpos, int ypos);

#endif
