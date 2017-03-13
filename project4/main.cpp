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
        
    vector<NavSegment> ns;
    Navigator n;
    n.navigate("Bollywood Bites", "Getty Center Helipad", ns);
    
    cout << "we here" << endl;
}
