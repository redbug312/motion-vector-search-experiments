#ifndef SEARCH_H
#define SEARCH_H

#include "frame.h"

#ifndef likely
#if __GNUC__ < 3
#define __builtin_expect(x, n) (x)
#endif
#define likely(x)   __builtin_expect(!!(x),1)
#define unlikely(x) __builtin_expect(!!(x),0)
#endif

#define MV(x0,y0)                   ((MotionVector){.x=x0,.y=y0})
#define assignMV(v,x0,y0)           do{(v)->x=(x0);(v)->y=(y0);}while(0)

typedef struct MotionVector {
    int x;
    int y;
} MotionVector;

typedef struct SearchStatus {
    int distance;
    MotionVector origin;
    const MotionVector *candidate;
} SearchStatus;

typedef int (*IterNextCandidate)(MotionVector*, MotionVector*, int, SearchStatus*);
typedef SearchStatus (*InitSearchStatus)();

typedef struct MVSearchAlgo {
    int search_range;
    IterNextCandidate iter_next_candidate;
    InitSearchStatus init_search_status;
} MVSearchAlgo;

MVSearchAlgo ThreeStepSearch;

MotionVector MVSearch(Frame *ref_frame, Frame *frame, int xpos, int ypos, MVSearchAlgo *algo);

#endif
