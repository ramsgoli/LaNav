//
//  support.cpp
//  project4
//
//  Created by Ram Goli on 3/12/17.
//  Copyright © 2017 Ram Goli. All rights reserved.
//

#include "support.h"

bool operator>(const GeoCoord& x, const GeoCoord& y) {
    if (x.latitude > y.latitude)
        return true;
    else if (x.latitude < y.latitude)
        return false;
    else{
        if (x.longitude > y.longitude)
            return true;
        else
            return false;
    }
}

bool operator<(const GeoCoord& x, const GeoCoord& y) {
    if (x.latitude < y.latitude)
        return true;
    else if (x.latitude > y.latitude)
        return false;
    else{
        if (x.longitude < y.longitude)
            return true;
        else
            return false;
    }
}

bool operator==(const GeoCoord& x, const GeoCoord& y) {
    
    if (x.latitudeText == y.latitudeText)
        if (x.longitudeText == y.longitudeText)
            return true;
    
    return false;
}

bool operator!=(const GeoCoord& x, const GeoCoord& y) {
    if (x.latitudeText != y.latitudeText)
        if (x.longitudeText != y.longitudeText)
            return true;
    
    return false;
}

bool operator==(const GeoSegment& x, const GeoSegment& y) {
    return (x.start == y.start && x.end == y.end);
}

bool operator==(const StreetSegment& x, const StreetSegment& y) {
    return x.segment == y.segment;
}
