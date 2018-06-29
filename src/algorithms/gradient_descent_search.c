#include "search.h"

static const size_t candidate_count = 8;
static const MotionVector GradientDescentSearch_list[] = 
    { MV(-1, -1), MV(0, -1), MV(1, -1),
      MV(-1,  0)           , MV(1,  0),
      MV(-1,  1), MV(0,  1), MV(1,  1), };

// Status to stored between each calls of iter()
static MotionVector origin;
static const MotionVector *candidate;

bool GradientDescentSearch_iter(MotionVector *prevMV, MotionVector *currMV, bool better) {
    MotionVector *bestMV = better ? currMV : prevMV;

    // TODO: improve effciency by filtering out used MV
    candidate++;
    if (candidate == GradientDescentSearch_list + candidate_count) {
        if (bestMV->x == origin.x && bestMV->y == origin.y)
            return false;
        candidate = GradientDescentSearch_list;
        assignMV(&origin, bestMV->x, bestMV->y);
    }
    assignMV(prevMV, bestMV->x, bestMV->y);
    assignMV(currMV, origin.x + candidate->x,
                     origin.y + candidate->y);
    return true;
}

void GradientDescentSearch_init() {
    origin = MV(0, 0);
    candidate = GradientDescentSearch_list;
}

MVSearchAlgo GradientDescentSearch = {
    .iter_next_candidate = GradientDescentSearch_iter,
    .init_search_status  = GradientDescentSearch_init,
};
