#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "frame.h"
#include "search.h"

int SSE(Frame *ref_frame, Frame *frame, int xpos, int ypos, MotionVector mv) {
    const uint8_t *pA = get_frame_at_pos(ref_frame, 0, xpos + mv.x, ypos + mv.y);
    const uint8_t *pB = get_frame_at_pos(frame, 0, xpos, ypos);
    int mb_size = 1 << frame->mb_size_log2;

    int sse = 0;
    for (int y = 0; y < mb_size; y++) {
        for (int x = 0; x < mb_size; x++) {
            sse += abs(*pA - *pB) * abs(*pA - *pB);
            pA++;
            pB++;
        }
        pA += ref_frame->stride - mb_size;
        pB += ref_frame->stride - mb_size;
    }
    return sse;
}

double PSNR(double mse) {
    return 10 * log10(255.0 * 255.0 / mse);
}

int main(int argc, char *argv[]) {
    size_t width = 352, height = 288;
    FILE *fptr = fopen("data/Foreman.CIF", "rb");

    Frame frame[2];
    create_frame(&frame[0], width, height);
    create_frame(&frame[1], width, height);

    // unloop fi = 0 to avoid if-else branching
    read_into_frame(&frame[0], fptr);
    int mb_size_log2 = frame[0].mb_size_log2;

    for (int fi = 1; fi < 300; fi++) {
        int curr = fi & 0x1;
        int prev = curr ^ 0x1;

        read_into_frame(&frame[curr], fptr);

        uint64_t sse = 0;  // 2**32 < 255*255*352*288*300 < 2**64
        for (int mbx = 0; mbx < width >> mb_size_log2; mbx++)
            for (int mby = 0; mby < height >> mb_size_log2; mby++) {
                MotionVector mv = MVSearch(&frame[prev], &frame[curr],
                                           mbx << mb_size_log2,
                                           mby << mb_size_log2,
                                           &ThreeStepSearch);
                sse += SSE(&frame[prev], &frame[curr],
                           mbx << mb_size_log2,
                           mby << mb_size_log2, mv);
        }

        double mse = (double) sse / width / height;
        printf("frame: %d\tPSNR: %f\n", fi, PSNR(mse));
    }

    destroy_frame(&frame[0]);
    destroy_frame(&frame[1]);
}
