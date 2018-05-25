#ifndef SEARCH_H
#define SEARCH_H

#include <stdbool.h>

#include "frame.h"

#ifndef likely
#if __GNUC__ < 3
#define __builtin_expect(x, n) (x)
#endif
#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)
#endif

#define MV(x0,y0)           ((MotionVector){.x=x0,.y=y0})
#define assignMV(v,x0,y0)   do{(v)->x=(x0);(v)->y=(y0);}while(0)

typedef struct MotionVector {
    int x;
    int y;
} MotionVector;

// Return if there's more candidates to examine
typedef bool (*IterNextCandidate)(MotionVector*, MotionVector*, bool);
typedef void (*InitSearchStatus)();

typedef struct MVSearchAlgo {
    IterNextCandidate iter_next_candidate;
    InitSearchStatus init_search_status;
} MVSearchAlgo;

MVSearchAlgo NoSearch;
MVSearchAlgo ThreeStepSearch;
MVSearchAlgo OrthogonalSearch;
MVSearchAlgo GradientDescentSearch;
MVSearchAlgo CrossSearch;
MVSearchAlgo HexagonSearch;

MotionVector MVSearch(Frame *ref_frame, Frame *frame, int xpos, int ypos, const MVSearchAlgo *algo);

#endif
