#include "search.h"

const size_t candidate_count = 9;
const MotionVector ThreeStepSearch_list[] = 
    { MV(-1, -1), MV(0, -1), MV(1, -1),
      MV(-1,  0), MV(0,  0), MV(1,  0),
      MV(-1,  1), MV(0,  1), MV(1,  1), };

// Status to stored between each calls of iter()
static int step;
static MotionVector origin;
static const MotionVector *candidate;

bool ThreeStepSearch_iter(MotionVector *prevMV, MotionVector *currMV, bool better) {
    MotionVector *bestMV = better ? currMV : prevMV;

    candidate++;
    if (unlikely(candidate == ThreeStepSearch_list + candidate_count)) {
        step >>= 1;
        candidate = ThreeStepSearch_list;
        assignMV(&origin, bestMV->x, bestMV->y);
    }
    assignMV(prevMV, bestMV->x, bestMV->y);
    assignMV(currMV, origin.x + candidate->x * step,
                     origin.y + candidate->y * step);

    return step > 0;
}

void ThreeStepSearch_init() {
    step = 4;
    origin = MV(0, 0);
    candidate = ThreeStepSearch_list;
}

MVSearchAlgo ThreeStepSearch = {
    .iter_next_candidate = ThreeStepSearch_iter,
    .init_search_status  = ThreeStepSearch_init,
};
