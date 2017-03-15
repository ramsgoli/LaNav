// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;
#include <iostream>


void testNavSegments(vector<NavSegment> ns) {
    for (int i = 0; i < ns.size(); i++) {
        if (ns[i].m_command == NavSegment::PROCEED) {
            cout << "directions[" << i << "]:" << endl;
            cout << "   type: PROCEED" << endl;
            cout << "   start: " << ns[i].m_geoSegment.start.latitudeText << ", " << ns[i].m_geoSegment.start.longitudeText << endl;
            cout << "   end: " << ns[i].m_geoSegment.end.latitudeText << ", " << ns[i].m_geoSegment.end.longitudeText << endl;
            cout << "   direction: " << ns[i].m_direction << endl;
            cout << "   distance: " << ns[i].m_distance << endl;
            cout << "   street: " << ns[i].m_streetName << endl;
        } else {
            cout << "directions[" << i << "]:" << endl;
            cout << "   type: TURN" << endl;
            cout << "   direction: " << ns[i].m_direction << endl;
            cout << "   street: " << ns[i].m_streetName << endl;
        }
    }
}

int main()
{
    
    
    MapLoader mp;
    mp.load("/Users/ramgoli/Documents/Winter2017/cs32/project4/project4/mapdata.txt");
    
    vector<NavSegment> ns;
    Navigator n;
    
    
    n.navigate( "Fox Plaza", "Hedrick Hall",  ns);
    testNavSegments(ns);
    
}
