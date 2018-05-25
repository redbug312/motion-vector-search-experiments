#include "search.h"

static const size_t candidate_large_count = 6;
static const MotionVector HexagonSearch_large_list[] =
    {        MV(-1, -2), MV(1, -2),
        MV(-2, 0),            MV(0, 2),
             MV(-1,  2), MV(1,  2),     };

static const size_t candidate_small_count = 4;
static const MotionVector HexagonSearch_small_list[] =
    {              MV(0, -1),
        MV(-1, 0),            MV(0, 1),
                   MV(0,  1),           };

// Status to stored between each calls of iter()
static MotionVector origin;
static const MotionVector *candidate;

bool HexagonSearch_iter(MotionVector *prevMV, MotionVector *currMV, bool better) {
    MotionVector *bestMV = better ? currMV : prevMV;

    // TODO: improve effciency by filtering out used MV
    candidate++;
    if (unlikely(candidate == HexagonSearch_large_list + candidate_large_count)) {
        candidate = (bestMV->x == origin.x && bestMV->y == origin.y)
            ? HexagonSearch_small_list : HexagonSearch_large_list;
        assignMV(&origin, bestMV->x, bestMV->y);
    }
    assignMV(prevMV, bestMV->x, bestMV->y);
    // avoid accessing invalid memory HexagonSearch_small_list[4]
    bool ended = candidate == HexagonSearch_small_list + candidate_small_count;
    if (likely(!ended)) {
        assignMV(currMV, origin.x + candidate->x,
                         origin.y + candidate->y);
    }
    return !ended;
}

void HexagonSearch_init() {
    origin = MV(0, 0);
    candidate = HexagonSearch_large_list;
}

MVSearchAlgo HexagonSearch = {
    .iter_next_candidate = HexagonSearch_iter,
    .init_search_status  = HexagonSearch_init,
};
