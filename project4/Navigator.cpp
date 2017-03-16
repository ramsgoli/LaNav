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
    
    //nodes are containers that store a geocoordinate and its value to be sorted by in the priority queue
    //priority queues, by default, sort items from largest to smallest
    //we want our priority queue to hold nodes that are getting closer to the destination at the top
    //so we reverse the behavior of the less than and greater than operators
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
    
    void findNavSegments(GeoCoord &shit, MyMap<GeoCoord, GeoCoord> &mapshit, vector<NavSegment> &directions) const;
    double distFromTwo(const GeoCoord &start, const GeoCoord &next) const;
    string getDirection(const GeoSegment &seg) const;
    void visitNewNode(const GeoCoord &curr, const GeoCoord &next, const GeoCoord &finalCoord, MyMap<GeoCoord, GeoCoord> &cameFrom,
                      MyMap<GeoCoord, double> &currentCost,
                      priority_queue<node> &m_queue) const;
    
    //some minheap
    
};

NavigatorImpl::NavigatorImpl()
{
    
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
	return false;  // we weren't able to correctly load the mapfile
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    
    MyMap<GeoCoord, GeoCoord> cameFrom; //to associate a geocoordinate to the one it came from along a given path
    MyMap<GeoCoord, double> currentCost; //to associate a geocoordinate to the distance of the path taken to get there
    
    GeoCoord startGeoCoord;
    if (!am.getGeoCoord(start, startGeoCoord))
        return NAV_BAD_SOURCE;
    
    GeoCoord endGeoCoord;
    if (!am.getGeoCoord(end, endGeoCoord))
        return NAV_BAD_DESTINATION;
    
    priority_queue<node> m_queue; //THE MEAT OF THE WHOLE DAMN ALGORITHM RIGHT HERE
    
    node startCoordNode(startGeoCoord, distanceEarthMiles(startGeoCoord, endGeoCoord));
    m_queue.push(startCoordNode);
    
    cameFrom.associate(startGeoCoord, startGeoCoord); //associate the start node to itself because it obivously wasnt traveled to from anywhere
    currentCost.associate(startGeoCoord, 0); //there is no cost to travel to the start node
    
    vector<StreetSegment> endCoordStreetSegments = sm.getSegments(endGeoCoord);

    while (!m_queue.empty()) {
        
        node topNode = m_queue.top();
        m_queue.pop();
        
        //check if we hit the right place. We're done!
        if (topNode.gc == endGeoCoord) {
            findNavSegments(topNode.gc, cameFrom, directions);
            return NAV_SUCCESS;
        }
        
        vector<StreetSegment> topNodeStreetSegments = sm.getSegments(topNode.gc);
        
        for (int i = 0; i < topNodeStreetSegments.size(); i++) {
            for (int j = 0; j < endCoordStreetSegments.size(); j++) {
                if (topNodeStreetSegments[i] == endCoordStreetSegments[j]) {
                    visitNewNode(topNode.gc, endGeoCoord, endGeoCoord, cameFrom, currentCost, m_queue);
                }
            }
        }
        
        for (int i = 0; i < topNodeStreetSegments.size(); i++) {
            StreetSegment nextStreet = topNodeStreetSegments[i];
            GeoSegment nextSeg = nextStreet.segment;
            
            if (nextSeg.start == topNode.gc) //the 'start' coord is the same as the current one. visit the 'end' coord;
            {
                visitNewNode(topNode.gc, nextSeg.end, endGeoCoord, cameFrom, currentCost, m_queue);

            } else if (nextSeg.end == topNode.gc) //the 'start' coord is the same as the current one. visit the 'end' coord;
            {
                visitNewNode(topNode.gc, nextSeg.end, endGeoCoord, cameFrom, currentCost, m_queue);

            } else //the geocoord is in the middle of the segment. visit both coordinates!
            {
                visitNewNode(topNode.gc, nextSeg.start, endGeoCoord, cameFrom, currentCost, m_queue);
                visitNewNode(topNode.gc, nextSeg.end, endGeoCoord, cameFrom, currentCost, m_queue);
            }
        }
    }
    
    //if our queue is empty, we will have searched every damn coordinate that can be linked to the start coordinate, so return no route
	return NAV_NO_ROUTE;
    
}

void NavigatorImpl::visitNewNode(const GeoCoord &curr, const GeoCoord &next, const GeoCoord &finalCoord, MyMap<GeoCoord, GeoCoord> &cameFrom,
                  MyMap<GeoCoord, double> &currentCost,
                priority_queue<node> &m_queue) const {
    
    if (*(cameFrom.find(curr)) != next) { //check that we are not backtracking
        double newCost = *(currentCost.find(curr)) + distFromTwo(curr, next); //the cost of going to a coordinate is the distance to travel
                                                                              //up to the current coordinate plus distance to new coordinate
        
        if (currentCost.find(next) == nullptr || newCost < *(currentCost.find(next))) { //if it is less efficient to travel to new coordinate
                                                                                        //than any previous path already taken, dont check it
            currentCost.associate(next, newCost);
            double val = newCost + distFromTwo(next, finalCoord); //the heuristic value of the next coord is the distance it takes to travel
                                                                  //to that coordinate plus the total euclidean distance to the end cooordinate
            node nextCoordNode(next, val);
            m_queue.push(nextCoordNode);
            cameFrom.associate(next, curr);
        }
    }
}

double NavigatorImpl::distFromTwo(const GeoCoord &start, const GeoCoord &next) const {
    return distanceEarthMiles(start, next);
}

void NavigatorImpl::findNavSegments(GeoCoord &end, MyMap<GeoCoord, GeoCoord> &mapshit, vector<NavSegment> &directions) const {
    
    for (;;) {
        GeoCoord secondToEnd = *(mapshit.find(end));
        bool found = false;
        
            vector<StreetSegment> streetsFromEnd = sm.getSegments(end);
            vector<StreetSegment> streetsFromSecondToEnd = sm.getSegments(secondToEnd);
            
            for (int i = 0; i < streetsFromEnd.size(); i++) {
                for (int j = 0; j < streetsFromSecondToEnd.size(); j++) {
                    if (!found) {
                        if (streetsFromEnd[i].segment == streetsFromSecondToEnd[j].segment) //we found the right segment
                        {
                            GeoSegment newS(secondToEnd, end);
                            NavSegment nav;
                            nav.m_geoSegment = newS;
                            nav.m_direction = getDirection(newS);
                            nav.m_distance = distanceEarthMiles(end, secondToEnd);
                            nav.m_streetName=streetsFromEnd[i].streetName;
                            directions.insert(directions.begin(), nav);
                            found = true; //we created a navsegment representing this segement to travel along, so break out of this loop
                        }
                    }
                }
            }
        end = secondToEnd;
        if (*mapshit.find(end) == end) { //we hit the start coordinate (we originally associated it to itself)
            //now we go and add in turn segments wherever appropriate
            vector<NavSegment>::iterator it=directions.begin();
            
            for (int i = 0; i < directions.size()-1; i++) {
                it++;
                if (directions[i].m_streetName != directions[i+1].m_streetName) {
                    string dir;
                    if (angleBetween2Lines(directions[i].m_geoSegment, directions[i+1].m_geoSegment) < 180)
                        dir = "left";
                    else
                        dir = "right";
                    it = directions.insert(it, NavSegment(dir, directions[i+1].m_streetName));
                }
            }
            return;
        }
    }
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
