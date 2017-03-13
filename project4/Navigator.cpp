#include "provided.h"
#include "support.h"
#include <list>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct node {
    GeoCoord gc;
    double disToEnd;
    string name;
    bool operator<(const node &o) const
    {
        return disToEnd > o.disToEnd;
    }
};


class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
    
private:
    SegmentMapper sm;
    AttractionMapper am;
    MapLoader ml;
    
    //some minheap
    
};

NavigatorImpl::NavigatorImpl()
{
    loadMapData("/Users/ramgoli/Documents/Winter2017/cs32/project4/project4/mapdata.txt");
    sm.init(ml);
    am.init(ml);
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if (ml.load(mapFile))
        return true;
	return false;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    priority_queue<node> m_nodequeue;
    list<node> finalPath;
    
    node startCoord;
    if (!am.getGeoCoord(start, startCoord.gc))
        return NAV_BAD_SOURCE;
    
    GeoCoord endCoord;
    if (!am.getGeoCoord(end, endCoord))
        return NAV_BAD_DESTINATION;
    
    startCoord.disToEnd = distanceEarthMiles(startCoord.gc, endCoord);
    startCoord.name = start;
    m_nodequeue.push(startCoord);
    
    
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
