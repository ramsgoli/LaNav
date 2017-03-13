// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;
#include <iostream>

int main()
{
    
    MyMap<string,double> nameToGPA; // maps student name to GPA
    // add new items to the binary search tree-based map
    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
    nameToGPA.associate("David", 3.99); // David beat Carey
    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
    nameToGPA.associate("Zach", 1.1);
    
    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr)
        *davidsGPA = 1.5;
    
    MapLoader mp;
    mp.load("/Users/ramgoli/Documents/Winter2017/cs32/project4/project4/mapdata.txt");
    
    AttractionMapper am;
    am.init(mp);
    
    SegmentMapper sm;
    sm.init(mp); // let our object build its internal data structures
    // by iterating thru all segments from the MapLoader object
    GeoCoord lookMeUp("000", "-118.4417620");
    std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
    if (vecOfAssociatedSegs.empty())
    {
        cout << "Error - no segments found matching this coordinate\n";
    }
    cout << "Here are all the segments associated with your coordinate:" << endl;
    for (auto s: vecOfAssociatedSegs)
    {
        cout << "Segment’s street: " << s.streetName << endl;
        cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " << s.segment.start.longitude << endl;
        cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " << s.segment.end.longitude << endl;
        cout << "This segment has " << s.attractions.size() << " attractions on it." << endl;
    }
    
    vector<NavSegment> ns;
    Navigator n;
    n.navigate("Covel Commons", "Regent", ns);
}
