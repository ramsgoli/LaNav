#include "provided.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
    
private:
    int m_numSegments;
    vector<StreetSegment> m_streetSegments;
    void processThreeLines(string line1, string line2, string line3);
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    
    string line;
    string nameline;
    string geocoordline;
    string attractionumline;
    string locationLine;
    
    ifstream myfile (mapFile);
    
    if (myfile.is_open())
    {
        while (getline(myfile, nameline)) {
            getline(myfile, geocoordline);
            getline(myfile, attractionumline);
            
            string startLat = geocoordline.substr(0, geocoordline.find(','));
            geocoordline.erase(0, geocoordline.find(',') +1);
            if (geocoordline[0] == ' ')
                geocoordline.erase(0, 1);
            string startLong = geocoordline.substr(0, geocoordline.find(' '));
            geocoordline.erase(0, geocoordline.find(' ') +1);
            
            string endLat = geocoordline.substr(0, geocoordline.find(','));
            geocoordline.erase(0, geocoordline.find(',') +1);
            if (geocoordline[0] == ' ')
                geocoordline.erase(0, 1);
            string endLong = geocoordline;
            
            GeoSegment gs(GeoCoord(startLat, startLong), GeoCoord(endLat, endLong));
            
            vector<Attraction> va;
            
            int numAttractions = stod(attractionumline.c_str());
            for (int i = 0; i < numAttractions; i++) {
                getline(myfile, locationLine);
                string locationName = locationLine.substr(0, locationLine.find('|'));
                locationLine.erase(0, locationLine.find('|')+1);
                if (locationLine[0] == ' ')
                    locationLine.erase(0, 1);
                
                string startLat = locationLine.substr(0, locationLine.find(','));
                locationLine.erase(0, locationLine.find(',')+1);
                if (locationLine[0] == ' ')
                    locationLine.erase(0, 1);
                string startLong = locationLine;
                
                Attraction a;
                a.geocoordinates = GeoCoord(startLat, startLong);
                a.name = locationName;
                va.push_back(a);
            }
            
            StreetSegment ss;
            ss.segment = gs;
            ss.streetName = nameline;
            ss.attractions = va;
            
            m_streetSegments.push_back(ss);
            m_numSegments++;

        }
        
        myfile.close();
        return true;
    } else {
        return false;
    }
    

}

size_t MapLoaderImpl::getNumSegments() const
{
    return 0; // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    return false;  // This compiles, but may not be correct
}

void MapLoaderImpl::processThreeLines(string line1, string line2, string line3) {
    
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
    m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
    delete m_impl;
}

bool MapLoader::load(string mapFile)
{
    return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
    return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
    return m_impl->getSegment(segNum, seg);
}
