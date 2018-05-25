#include "search.h"

static const size_t candidate_count = 4;
static const MotionVector CrossSearch_list[] =
    { MV(-1, -1), MV( 1, -1),
      MV(-1,  1), MV( 1,  1), };
static const MotionVector CrossSearch_final_list[][4] =
    { { MV(-1, -1), MV( 1, -1),
        MV(-1,  1), MV( 1,  1), },
      { MV( 0, -1), MV(-1,  0),
        MV( 1,  0), MV( 0,  1), } };

// Status to stored between each calls of iter()
static int step;
static MotionVector origin;
static const MotionVector *candidate;

bool CrossSearch_iter(MotionVector *prevMV, MotionVector *currMV, bool better) {
    MotionVector *bestMV = better ? currMV : prevMV;

    candidate++;
    if (unlikely(candidate = CrossSearch_list + candidate_count)) {
        step >>= 1;
        bool best_TR_BL = bestMV->x - origin.x + bestMV->y - origin.y == 0; // useless when step > 0
        candidate = (step > 0)
            ? CrossSearch_list : CrossSearch_final_list[(int)best_TR_BL];
        assignMV(&origin, bestMV->x, bestMV->y);
    }
    assignMV(prevMV, bestMV->x, bestMV->y);
    // avoid accessing invalid memory CrossSearch_final_list[n][4]
    bool ended = candidate == CrossSearch_final_list[0] + candidate_count ||
                 candidate == CrossSearch_final_list[1] + candidate_count;
    if (likely(!ended)) {
        assignMV(currMV, origin.x + candidate->x * step,
                         origin.y + candidate->y * step);
    }
    return !ended;
}

void CrossSearch_init() {
    step = 4;
    origin = MV(0, 0);
    candidate = CrossSearch_list;
}

MVSearchAlgo CrossSearch = {
    .iter_next_candidate = CrossSearch_iter,
    .init_search_status  = CrossSearch_init,
};
