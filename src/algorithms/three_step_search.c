#include "search.h"

const MotionVector ThreeStepSearch_list[] = 
    { MV(-1, -1), MV(0, -1), MV(1, -1),
      MV(-1,  0),            MV(1,  0),
      MV(-1,  1), MV(0,  1), MV(1,  1), };

int ThreeStepSearch_iter(MotionVector *prevMV, MotionVector *currMV, int better, SearchStatus *status) {
    MotionVector *bestMV = better ? currMV : prevMV;

    status->candidate++;
    if (unlikely(status->candidate == ThreeStepSearch_list + 8)) {
        status->distance >>= 1;
        status->candidate = ThreeStepSearch_list;
        assignMV(&status->origin, bestMV->x, bestMV->y);
    }
    assignMV(prevMV, bestMV->x, bestMV->y);
    assignMV(currMV, status->origin.x + status->candidate->x * status->distance,
                     status->origin.y + status->candidate->y * status->distance);

    return status->distance > 0;
}

SearchStatus ThreeStepSearch_init() {
    return (SearchStatus){4, MV(0, 0), ThreeStepSearch_list};
}

MVSearchAlgo ThreeStepSearch = {
    .search_range        = 4,
    .iter_next_candidate = ThreeStepSearch_iter,
    .init_search_status  = ThreeStepSearch_init,
};

