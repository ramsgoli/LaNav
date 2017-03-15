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
    MyMap<GeoCoord, vector<StreetSegment> > segmentmap;
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
        
        if (end.latitudeText == "51.510220") {
            
        }
        
        
        if (segmentmap.find(start) == nullptr){
            vector<StreetSegment> temp;
            temp.push_back(ss);
            segmentmap.associate(start, temp);
        } else {
            vector<StreetSegment> *temp = segmentmap.find(start);
            temp->push_back(ss);
            segmentmap.associate(start, *temp);
        }
        
        vector<StreetSegment> *endss;
        endss = segmentmap.find(end);
        
        if (endss == nullptr) {
            vector<StreetSegment> temp;
            temp.push_back(ss);
            segmentmap.associate(end, temp);
        } else {
            vector<StreetSegment> *temp = segmentmap.find(end);
            temp->push_back(ss);
            segmentmap.associate(end, *temp);
        }
        
        
        for (int j = 0; j < va.size(); j++) {
            GeoCoord gs = va[j].geocoordinates;
            if (segmentmap.find(gs) == nullptr){
                vector<StreetSegment> temp;
                temp.push_back(ss);
                segmentmap.associate(gs, temp);
            } else {
                vector<StreetSegment> *temp = segmentmap.find(gs);
                temp->push_back(ss);
                segmentmap.associate(gs, *temp);
            }
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
