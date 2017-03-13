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
    
    node(GeoCoord xgc, double xval, string xname) {
        gc = xgc;
        val = xval;
        name = xname;
    }
    
    node() {
        val = 0;
    }
    
    bool operator<(const node &o) const
    {
        return val > o.val;
    }
    
    bool operator>(const node &o) const {
        return val < o.val;
    }
    
    bool operator==(const node &o) const {
        return val==o.val;
    }
    
    string name;
    GeoCoord gc;
    double val;
    
};

void printPath(node &shit, MyMap<node, node> &mapshit) {
    GeoCoord gc = shit.gc;
    cout << gc.latitudeText << ", "<< gc.longitudeText << endl;;
    while (mapshit.find(shit)->gc != shit.gc) {
        shit = *(mapshit.find(shit));
        gc = shit.gc;
        cout << gc.latitudeText << ", "<< gc.longitudeText << endl;;
    }
}


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
    double distFromTwo(const GeoCoord &start, const GeoCoord &next) const;
    double distToEnd(const GeoCoord &next, const GeoCoord &end) const;
    
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
    MyMap<GeoCoord, double> currentCost;
    
    vector<node> finalPath;
    
    GeoCoord startGeoCoord;
    am.getGeoCoord(start, startGeoCoord);
    
    GeoCoord endGeoCoord;
    am.getGeoCoord(end, endGeoCoord);
    priority_queue<node> m_queue;
    
    node startCoordNode(startGeoCoord, distanceEarthMiles(startGeoCoord, endGeoCoord), start);
    m_queue.push(startCoordNode);
    
    cameFrom.associate(startCoordNode, startCoordNode);
    currentCost.associate(startGeoCoord, 0);
    
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
                    node destination(endGeoCoord, 0, end);
                    cameFrom.associate(destination, topNode);
                    cout << "we got a path" << endl;
                    cout << "path: " << endl;
                    printPath(destination, cameFrom);
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
                double newCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, nextCoord);
                if (sm.getSegments(nextCoord).size() != 0 || newCost < *(currentCost.find(nextCoord))) {
                    currentCost.associate(nextCoord, newCost);
                    double val = newCost + distFromTwo(nextCoord, endGeoCoord);
                    node nextCoordNode(nextCoord, val, nextStreet.streetName);
                    m_queue.push(nextCoordNode);
                    cameFrom.associate(nextCoordNode, topNode);
                }
            } else if (nextSeg.end == topNode.gc) //the 'start' coord is the same as the current one. pick the 'end' coord;
            {
                GeoCoord nextCoord = nextSeg.start;
                double newCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, nextCoord);
                if (sm.getSegments(nextCoord).size() != 0 || newCost < *(currentCost.find(nextCoord))) {
                    currentCost.associate(nextCoord, newCost);
                    double val = newCost + distFromTwo(nextCoord, endGeoCoord);
                    node nextCoordNode(nextCoord, val, nextStreet.streetName);
                    m_queue.push(nextCoordNode);
                    cameFrom.associate(nextCoordNode, topNode);
                }
            } else //the geocoord is in the middle of the segment. push both!
            {
                GeoCoord firstNextCoord = nextSeg.start;
                GeoCoord secondNextCoord = nextSeg.end;
                double firstNewCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, firstNextCoord);
                double secondNewCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, secondNextCoord);
                
                if (sm.getSegments(firstNextCoord).size() != 0 || firstNewCost < *(currentCost.find(firstNextCoord))) {
                    currentCost.associate(firstNextCoord, firstNewCost);
                    double val = firstNewCost + distFromTwo(firstNextCoord, endGeoCoord);
                    node nextCoordNode(firstNextCoord, val, nextStreet.streetName);
                    m_queue.push(nextCoordNode);
                    cameFrom.associate(nextCoordNode, topNode);
                }
                
                if (sm.getSegments(secondNextCoord).size() != 0 || secondNewCost < *(currentCost.find(secondNextCoord))) {
                    currentCost.associate(secondNextCoord, secondNewCost);
                    double val = secondNewCost + distFromTwo(secondNextCoord, endGeoCoord);
                    node nextCoordNode(secondNextCoord, val, nextStreet.streetName);
                    m_queue.push(nextCoordNode);
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

double NavigatorImpl::distFromTwo(const GeoCoord &start, const GeoCoord &next) const {
    return distanceEarthMiles(start, next);
}

double NavigatorImpl::distToEnd(const GeoCoord &next, const GeoCoord &end) const {
    return distanceEarthMiles(next, end);
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
