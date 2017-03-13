#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
    
private:
    MyMap<string, GeoCoord> attractionmap;
    
};

AttractionMapperImpl::AttractionMapperImpl()
{
    
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    int numSegments = ml.getNumSegments();
    int numshit = 0;
    for (int i = 0; i < numSegments; i++) {
        StreetSegment ss;
        ml.getSegment(i, ss);
        vector<Attraction> possibleAttractions = ss.attractions;
        for (int j = 0; j < possibleAttractions.size(); j++) {
            string name = possibleAttractions[j].name;
            for (std::string::size_type k=0; k<name.length(); ++k)
                name[k] = tolower(name[k]);
            attractionmap.associate(name, possibleAttractions[j].geocoordinates);
            numshit++;
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    
    for (int i = 0; i < attraction.size(); i++) {
        attraction[i] = tolower(attraction[i]);
    }
    if (attractionmap.find(attraction) == nullptr)
        return false;
    
    gc = *attractionmap.find(attraction);
    return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
