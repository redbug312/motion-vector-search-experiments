#include "search.h"

int NoSearch_iter(MotionVector *prevMV, MotionVector *currMV, int better, SearchStatus *status) {
    return 0;
}

SearchStatus NoSearch_init() {
    return (SearchStatus) {0, MV(0, 0), NULL};
}

MVSearchAlgo NoSearch = {
    .iter_next_candidate = NoSearch_iter,
    .init_search_status  = NoSearch_init,
};
