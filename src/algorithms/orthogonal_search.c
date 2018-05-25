#include "search.h"

static const size_t candidate_count = 2;
static const MotionVector OrthogonalSearch_list[][2] =
    { {MV(-1, 0), MV(1, 0)},
      {MV(0, -1), MV(0, 1)}, };

// Status to stored between each calls of iter()
static int step;
static MotionVector origin;
static const MotionVector *candidate;

bool OrthogonalSearch_iter(MotionVector *prevMV, MotionVector *currMV, bool better) {
    MotionVector *bestMV = better ? currMV : prevMV;

    candidate++;
    if (unlikely(candidate == OrthogonalSearch_list[0] + candidate_count)) {
        candidate = OrthogonalSearch_list[1];
        assignMV(&origin, bestMV->x, bestMV->y);
    } else if (unlikely(candidate == OrthogonalSearch_list[1] + candidate_count)) {
        step >>= 1;
        candidate = OrthogonalSearch_list[0];
        assignMV(&origin, bestMV->x, bestMV->y);
    }
    assignMV(prevMV, bestMV->x, bestMV->y);
    assignMV(currMV, origin.x + candidate->x * step,
                     origin.y + candidate->y * step);
    return step > 0;
}

void OrthogonalSearch_init() {
    step = 4;
    origin = MV(0, 0);
    candidate = OrthogonalSearch_list[0];
}

MVSearchAlgo OrthogonalSearch = {
    .iter_next_candidate = OrthogonalSearch_iter,
    .init_search_status  = OrthogonalSearch_init,
};
