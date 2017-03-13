#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    MyMap<GeoCoord, vector<StreetSegment>> segmentmap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    int numSegments = ml.getNumSegments();
    
    for (int i = 0; i < numSegments; i++) {
        StreetSegment ss;
        ml.getSegment(i, ss);
        GeoSegment gs = ss.segment;
        vector<Attraction> va = ss.attractions;
        
        GeoCoord start = gs.start;
        GeoCoord end = gs.end;
        
        vector<StreetSegment> *allSS;
        allSS = segmentmap.find(start);
        if (allSS == nullptr){
            allSS = new vector<StreetSegment>;
        }
        
        allSS->push_back(ss);
        segmentmap.associate(start, *allSS);
        
        
        allSS = segmentmap.find(end);
        if (allSS == nullptr){
            allSS = new vector<StreetSegment>;
        }
        allSS->push_back(ss);
        segmentmap.associate(end, *allSS);
        
        for (int j = 0; j < va.size(); j++) {
            GeoCoord gs = va[j].geocoordinates;
            allSS = segmentmap.find(gs);
            if (allSS == nullptr){
                allSS = new vector<StreetSegment>;
            }
            allSS->push_back(ss);
            segmentmap.associate(gs, *allSS);
        }
        
    }
    
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    if (segmentmap.find(gc) == nullptr) {
        vector<StreetSegment> emptyv;
        return emptyv;
    }
    
    return *(segmentmap.find(gc));
    
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
