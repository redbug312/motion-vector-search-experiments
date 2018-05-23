#include <stdint.h>
#include <limits.h>

#include "frame.h"
#include "search.h"

int SAD(const uint8_t *ref, int ref_stride, const uint8_t *pix, int pix_stride) {
    int sum = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            sum += abs(*ref - *pix);
            ref++;
            pix++;
        }
        ref += ref_stride - 8;
        pix += pix_stride - 8;
    }
    return sum;
}

MotionVector MVSearch(Frame *ref_frame, Frame *frame, int xpos, int ypos, MVSearchAlgo *algo) {
    MotionVector *prevMV = &MV(0, 0), *currMV = &MV(0, 0);

    int prev_cost = INT_MAX, curr_cost = INT_MAX;
    SearchStatus status = algo->init_search_status();
    while (algo->iter_next_candidate(prevMV, currMV, prev_cost > curr_cost, &status)) {
        prev_cost = prev_cost > curr_cost ? curr_cost : prev_cost;
        if (likely(xpos + currMV->x >= 0 && xpos + currMV->x + 8 < ref_frame->width &&
                   ypos + currMV->y >= 0 && ypos + currMV->y + 8 < ref_frame->height)) {
            curr_cost = SAD(get_image_at_pos(ref_frame, 0, xpos + currMV->x, ypos + currMV->y), ref_frame->stride,
                            get_image_at_pos(frame, 0, xpos, ypos), frame->stride);
        }
    }
    return MV(prevMV->x, prevMV->y);
}

