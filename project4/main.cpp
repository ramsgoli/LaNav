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
    
    GeoCoord fillMe;
    string attraction = "The Coffee Bean & Tea Leaf";
    
    bool found = am.getGeoCoord(attraction, fillMe);
    if (!found)
    {
        cout << "No geolocation found for " << attraction << endl;
    } else
        cout << "The location of " << attraction << " is " << fillMe.latitude << ", " << fillMe.longitude << endl;
}
