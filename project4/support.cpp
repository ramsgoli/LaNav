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
    
    if (x.latitude == y.latitude)
        if (x.longitude == y.longitude)
            return true;
    
    return false;
}
