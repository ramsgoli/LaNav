//
//  source.h
//  project4
//
//  Created by Ram Goli on 3/12/17.
//  Copyright © 2017 Ram Goli. All rights reserved.
//

#include "provided.h"

#ifndef support_h
#define support_h

bool operator>(const GeoCoord& x, const GeoCoord& y);

bool operator<(const GeoCoord& x, const GeoCoord& y);

bool operator==(const GeoCoord& x, const GeoCoord& y);


bool operator==(const GeoSegment& x, const GeoSegment& y);
bool operator==(const StreetSegment& x, const StreetSegment& y);



#endif /* support */
