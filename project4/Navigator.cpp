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
    void printPath(GeoCoord &shit, MyMap<GeoCoord, GeoCoord> &mapshit, vector<NavSegment> &directions) const;
    double distFromTwo(const GeoCoord &start, const GeoCoord &next) const;
    double distToEnd(const GeoCoord &next, const GeoCoord &end) const;
    string getDirection(const GeoSegment &seg) const;
    
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
    
    int count = 0;
    MyMap<GeoCoord, GeoCoord> cameFrom;
    MyMap<GeoCoord, double> currentCost;
    
    GeoCoord startGeoCoord;
    am.getGeoCoord(start, startGeoCoord);
    
    GeoCoord endGeoCoord;
    am.getGeoCoord(end, endGeoCoord);
    priority_queue<node> m_queue;
    
    node startCoordNode(startGeoCoord, distanceEarthMiles(startGeoCoord, endGeoCoord), start);
    m_queue.push(startCoordNode);
    
    cameFrom.associate(startGeoCoord, startGeoCoord);
    currentCost.associate(startGeoCoord, 0);
    
    vector<StreetSegment> endCoordStreetSegments = sm.getSegments(endGeoCoord);

    while (!m_queue.empty()) {
        
        node topNode = m_queue.top();
        m_queue.pop();
        count++;
        
        //check if we hit the right place
        vector<StreetSegment> topNodeStreetSegments = sm.getSegments(topNode.gc);
        for (int i = 0; i < topNodeStreetSegments.size(); i++) {
            for (int j = 0; j < endCoordStreetSegments.size(); j++) {
                if (topNodeStreetSegments[i] == endCoordStreetSegments[j]) {
                    cameFrom.associate(endGeoCoord, topNode.gc);
                    cout << "we got a path" << endl;
                    cout << "path: " << endl;
                    printPath(endGeoCoord, cameFrom, directions);
                    return NAV_SUCCESS;
                }
            }
        }
        
        for (int i = 0; i < topNodeStreetSegments.size(); i++) {
            StreetSegment nextStreet = topNodeStreetSegments[i];
            GeoSegment nextSeg = nextStreet.segment;
            
            if (nextSeg.start == topNode.gc) //the 'start' coord is the same as the current one. pick the 'end' coord;
            {
                GeoCoord nextCoord = nextSeg.end;
                if (*(cameFrom.find(topNode.gc)) != nextCoord) {
                    double newCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, nextCoord);
                    if (currentCost.find(nextCoord) == nullptr || newCost < *(currentCost.find(nextCoord))) {
                        currentCost.associate(nextCoord, newCost);
                        double val = newCost + distFromTwo(nextCoord, endGeoCoord);
                        node nextCoordNode(nextCoord, val, nextStreet.streetName);
                        m_queue.push(nextCoordNode);
                        cameFrom.associate(nextCoord, topNode.gc);
                    }
                }
                
            } else if (nextSeg.end == topNode.gc) //the 'start' coord is the same as the current one. pick the 'end' coord;
            {
                GeoCoord nextCoord = nextSeg.start;
                if (*(cameFrom.find(topNode.gc)) != nextCoord) {
                    double newCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, nextCoord);
                    if (currentCost.find(nextCoord) == nullptr || newCost < *(currentCost.find(nextCoord))) {
                        currentCost.associate(nextCoord, newCost);
                        double val = newCost + distFromTwo(nextCoord, endGeoCoord);
                        node nextCoordNode(nextCoord, val, nextStreet.streetName);
                        m_queue.push(nextCoordNode);
                        cameFrom.associate(nextCoord, topNode.gc);
                    }
                }
            
            } else //the geocoord is in the middle of the segment. push both!
            {
                GeoCoord firstNextCoord = nextSeg.start;
                GeoCoord secondNextCoord = nextSeg.end;
                double firstNewCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, firstNextCoord);
                double secondNewCost = *(currentCost.find(topNode.gc)) + distFromTwo(topNode.gc, secondNextCoord);
                
                if (*(cameFrom.find(topNode.gc)) != firstNextCoord) {
                    if (currentCost.find(firstNextCoord) == nullptr || firstNewCost < *(currentCost.find(firstNextCoord))) {
                        currentCost.associate(firstNextCoord, firstNewCost);
                        double val = firstNewCost + distFromTwo(firstNextCoord, endGeoCoord);
                        node nextCoordNode(firstNextCoord, val, nextStreet.streetName);
                        m_queue.push(nextCoordNode);
                        cameFrom.associate(firstNextCoord, topNode.gc);
                    }
                }
                
                if (*(cameFrom.find(topNode.gc)) != secondNextCoord) {
                    if (currentCost.find(secondNextCoord) == nullptr || secondNewCost < *(currentCost.find(secondNextCoord))) {
                        currentCost.associate(secondNextCoord, secondNewCost);
                        double val = secondNewCost + distFromTwo(secondNextCoord, endGeoCoord);
                        node nextCoordNode(secondNextCoord, val, nextStreet.streetName);
                        m_queue.push(nextCoordNode);
                        cameFrom.associate(secondNextCoord, topNode.gc);
                    }
                }
                
            }
        }
        
    }
    
    cout << "examined " << count << " coordinates" << endl;
    cout << "no route..." << endl;
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

void NavigatorImpl::printPath(GeoCoord &shit, MyMap<GeoCoord, GeoCoord> &mapshit, vector<NavSegment> &directions) const {
    
    for (;;) {
        GeoCoord secondToEnd = *(mapshit.find(shit));
    
            vector<StreetSegment> streetsFromEnd = sm.getSegments(shit);
            vector<StreetSegment> streetsFromSecondToEnd = sm.getSegments(secondToEnd);
            
            for (int i = 0; i < streetsFromEnd.size(); i++) {
                for (int j = 0; j < streetsFromSecondToEnd.size(); j++) {
                    if (streetsFromEnd[i].segment.start == streetsFromSecondToEnd[j].segment.start &&
                        streetsFromEnd[i].segment.end == streetsFromSecondToEnd[j].segment.end) //we found the right segment
                    {
                        
                        GeoSegment newS(secondToEnd, shit);
                        
                        NavSegment nav;
                        nav.m_geoSegment = newS;
                        nav.m_direction = getDirection(newS);
                        nav.m_distance = distanceEarthMiles(shit, secondToEnd);
                        nav.m_streetName=streetsFromEnd[i].streetName;
                        directions.insert(directions.begin(), nav);
                    }
                }
            }
        shit = secondToEnd;
        if (*mapshit.find(shit) == shit) {
            vector<NavSegment>::iterator it=directions.begin();
            
            for (int i = 0; i < directions.size()-1; i++) {
                it++;
                if (directions[i].m_streetName != directions[i+1].m_streetName) {
                    string dir;
                    if (angleBetween2Lines(directions[i].m_geoSegment, directions[i+1].m_geoSegment) < 180)
                        dir = "left";
                    else
                        dir = "right";
                    directions.insert(it, NavSegment(dir, directions[i+1].m_streetName));
                }
            }
            return;
        }
    }
    
//    if (streetsFromEnd[i].streetName != streetsFromSecondToEnd[j].streetName) { //we switched streets
//        
//        string dir;
//        if (angleBetween2Lines(streetsFromEnd[i].segment, streetsFromSecondToEnd[j].segment) < 180) {
//            dir = "LEFT";
//        } else {
//            dir = "RIGHT";
//        }
//        
//        directions.insert(directions.begin(), NavSegment(dir, streetsFromEnd[i].streetName));
//    }
}

string NavigatorImpl::getDirection(const GeoSegment &seg) const {
    double dir = angleOfLine(seg);
    
    if (dir > 337.5)
        return "east";
    else if (dir > 292.5)
        return "southeast";
    else if (dir > 247.5)
        return "south";
    else if (dir > 202.5)
        return "southwest";
    else if (dir > 157.5)
        return "west";
    else if (dir > 112.5)
        return "northwest";
    else if (dir > 67.5)
        return "north";
    else if (dir > 22.5)
        return "northeast";
    else
        return "east";
    
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
