#include <stdint.h>
#include <limits.h>

#include "frame.h"
#include "search.h"

int SAD(const uint8_t *ref, const uint8_t *pix, int stride, int mb_size) {
    int sum = 0;
    for (int y = 0; y < mb_size; y++) {
        for (int x = 0; x < mb_size; x++) {
            sum += abs(*ref - *pix);
            ref++;
            pix++;
        }
        ref += stride - mb_size;
        pix += stride - mb_size;
    }
    return sum;
}

MotionVector MVSearch(Frame *ref_frame, Frame *frame, int xpos, int ypos, MVSearchAlgo *algo) {
    MotionVector *prevMV = &MV(0, 0), *currMV = &MV(0, 0);
    int prev_cost = INT_MAX, curr_cost = INT_MAX;
    int mb_size = 1 << frame->mb_size_log2;

    algo->init_search_status();
    while (algo->iter_next_candidate(prevMV, currMV, prev_cost > curr_cost)) {
        prev_cost = prev_cost > curr_cost ? curr_cost : prev_cost;
        if (likely(xpos + currMV->x >= 0 && xpos + currMV->x + mb_size <= ref_frame->width &&
                   ypos + currMV->y >= 0 && ypos + currMV->y + mb_size <= ref_frame->height)) {
            curr_cost = SAD(get_frame_at_pos(ref_frame, 0, xpos + currMV->x, ypos + currMV->y),
                            get_frame_at_pos(frame, 0, xpos, ypos), frame->stride, mb_size);
        }
    }
    return MV(prevMV->x, prevMV->y);
}
