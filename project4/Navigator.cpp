#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct node {
    
    node(GeoCoord xgc, double xval) {
        gc = xgc;
        val = xval;
    }
    
    node() {
        val = 0;
    }
    GeoCoord gc;
    double val;
    
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
    
    void calculateNavSegments(const vector<NavSegment>& navsegments, vector<node>& nodes) const;
    
    //some minheap
    
};

NavigatorImpl::NavigatorImpl()
{
    loadMapData("/Users/ramgoli/Documents/Winter2017/cs32/project4/project4/mapdata.txt");
    
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if (ml.load(mapFile)) {
        sm.init(ml);
        am.init(ml);
        return true;
    }
	return false;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    MyMap<node, node> cameFrom;
    MyMap<node, double> currentCost;
    
    GeoCoord startGeoCoord;
    am.getGeoCoord(start, startGeoCoord);
    
    GeoCoord endGeoCoord;
    am.getGeoCoord(end, endGeoCoord);
    priority_queue<node> m_queue;
    
    node startCoordNode(startGeoCoord, distanceEarthMiles(startGeoCoord, endGeoCoord));
    m_queue.push(startCoordNode);
    
    cameFrom.associate(startCoordNode, startCoordNode);
    currentCost.associate(startCoordNode, distanceEarthMiles(startGeoCoord, endGeoCoord));
    
    vector<StreetSegment> endCoordStreetSegments = sm.getSegments(endGeoCoord);
    
    while (!m_queue.empty()) {
        node topNode = m_queue.top();
        m_queue.pop();
        
        //check if we hit the right place
        vector<StreetSegment> topNodeStreetSegments = sm.getSegments(topNode.gc);
        for (int i = 0; i < topNodeStreetSegments.size(); i++) {
            for (int j = 0; j < endCoordStreetSegments.size(); j++) {
                if (topNodeStreetSegments[i] == endCoordStreetSegments[j]) {
                    cout << "we got a path nigga" << endl;
                    return NAV_SUCCESS;
                }
            }
        }
        
        
        
    }
    
    
    
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

void NavigatorImpl::calculateNavSegments(const vector<NavSegment>& navsegments, vector<node>& nodes) const {
    
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
