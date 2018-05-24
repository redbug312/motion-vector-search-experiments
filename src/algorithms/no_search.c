#include "search.h"

bool NoSearch_iter(MotionVector *prevMV, MotionVector *currMV, bool better) {
    return false;
}

void NoSearch_init() {
    return;
}

MVSearchAlgo NoSearch = {
    .iter_next_candidate = NoSearch_iter,
    .init_search_status  = NoSearch_init,
};
