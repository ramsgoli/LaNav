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
    double cost(const GeoCoord &start, const GeoCoord &next, const GeoCoord &final) const;
    
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
    
    vector<node> finalPath;
    
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
        finalPath.push_back(topNode);
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
        
        vector<StreetSegment> streetSegmentsAssociatedWithCurr = sm.getSegments(topNode.gc);
        for (int i = 0; i < streetSegmentsAssociatedWithCurr.size(); i++) {
            StreetSegment nextStreet = streetSegmentsAssociatedWithCurr[i];
            GeoSegment nextSeg = nextStreet.segment;
            
            if (nextSeg.start == topNode.gc) //the 'start' coord is the same as the current one. pick the 'end' coord;
            {
                GeoCoord nextCoord = nextSeg.end;
                double newCost = topNode.val + cost(topNode.gc, nextCoord, endGeoCoord);
                if (sm.getSegments(nextCoord).size() != 0 || newCost < *(currentCost.find(topNode))) {
                    node nextCoordNode(nextCoord, newCost);
                    m_queue.push(nextCoordNode);
                    currentCost.associate(nextCoordNode, newCost);
                    cameFrom.associate(nextCoordNode, topNode);
                }
            } else if (nextSeg.end == topNode.gc) //the 'start' coord is the same as the current one. pick the 'end' coord;
            {
                GeoCoord nextCoord = nextSeg.start;
                double newCost = topNode.val + cost(topNode.gc, nextCoord, endGeoCoord);
                if (sm.getSegments(nextCoord).size() != 0 || newCost < *(currentCost.find(topNode))) {
                    node nextCoordNode(nextCoord, newCost);
                    m_queue.push(nextCoordNode);
                    currentCost.associate(nextCoordNode, newCost);
                    cameFrom.associate(nextCoordNode, topNode);
                }
            } else //the geocoord is in the middle of the segment. push both!
            {
                GeoCoord firstNextCoord = nextSeg.start;
                GeoCoord secondNextCoord = nextSeg.end;
                double firstNewCost = topNode.val + cost(topNode.gc, firstNextCoord, endGeoCoord);
                double secondNewCost = topNode.val + cost(topNode.gc, secondNextCoord, endGeoCoord);
                
                if (sm.getSegments(firstNextCoord).size() != 0 || firstNewCost < *(currentCost.find(topNode))) {
                    node nextCoordNode(firstNextCoord, firstNewCost);
                    m_queue.push(nextCoordNode);
                    currentCost.associate(nextCoordNode, firstNewCost);
                    cameFrom.associate(nextCoordNode, topNode);
                }
                
                if (sm.getSegments(secondNextCoord).size() != 0 || secondNewCost < *(currentCost.find(topNode))) {
                    node nextCoordNode(secondNextCoord, secondNewCost);
                    m_queue.push(nextCoordNode);
                    currentCost.associate(nextCoordNode, secondNewCost);
                    cameFrom.associate(nextCoordNode, topNode);
                }
            }
        }
        
    }
    
    
    
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

double NavigatorImpl::cost(const GeoCoord &start, const GeoCoord &next, const GeoCoord &final) const {
    
    return distanceEarthMiles(start, next) + distanceEarthMiles(next, final);
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
