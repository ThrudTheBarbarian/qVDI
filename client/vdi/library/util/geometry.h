//
//  geometry.h
//  gemc
//
//  Created by ThrudTheBarbarian on 11/4/23.
//

#ifndef geometry_h
#define geometry_h

#include <stdint.h>

typedef struct
        {
        int16_t x __attribute__ ((aligned (2)));
        int16_t y __attribute__ ((aligned (2)));
        int16_t w __attribute__ ((aligned (2)));
        int16_t h __attribute__ ((aligned (2)));
        } Rect;

/*****************************************************************************\
|* Intersection of two rectangles
\*****************************************************************************/
int intersection(Rect r1, Rect r2, Rect *intersect);

#endif /* geometry_h */
