#include "provided.h"
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
    bool operator<(const node &o) const
    {
        return val > o.val;
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
    bool found = false;
    priority_queue<node> m_nodequeue;
    vector<node> finalPath;
    
    node startCoord;
    if (!am.getGeoCoord(start, startCoord.gc))
        return NAV_BAD_SOURCE;
    
    GeoCoord endCoord;
    if (!am.getGeoCoord(end, endCoord))
        return NAV_BAD_DESTINATION;
    
    startCoord.val = distanceEarthMiles(startCoord.gc, endCoord);
    m_nodequeue.push(startCoord);
    
    vector<StreetSegment> segmentsassociatedwithend = sm.getSegments(endCoord);
    vector<StreetSegment> segmentsassociatedwithtop = sm.getSegments(m_nodequeue.top().gc);
    finalPath.push_back(m_nodequeue.top());
    
    for (int i = 0; i < segmentsassociatedwithtop.size(); i++) {
        GeoCoord segStartCoord = segmentsassociatedwithtop[i].segment.start;
        GeoCoord segEndCoord = segmentsassociatedwithtop[i].segment.end;
        
        double segStartCoordVal = distanceEarthMiles(segStartCoord, endCoord) + distanceEarthMiles(segStartCoord, m_nodequeue.top().gc);
        double segEndCoordVal = distanceEarthMiles(segEndCoord, endCoord) + distanceEarthMiles(segEndCoord, m_nodequeue.top().gc);
        
        m_nodequeue.pop();
        m_nodequeue.push(node(segStartCoord, segStartCoordVal));
        m_nodequeue.push(node(segEndCoord, segEndCoordVal));
    }
    
    while (!m_nodequeue.empty()) {
        
        GeoCoord currentlyProcessingGeoCoord = m_nodequeue.top().gc;
        segmentsassociatedwithtop = sm.getSegments(currentlyProcessingGeoCoord);
        finalPath.push_back(m_nodequeue.top());
        m_nodequeue.pop();
        
        for (int i = 0; i < segmentsassociatedwithtop.size(); i++) {
            for (int j = 0; j < segmentsassociatedwithend.size(); j++) {
                if (segmentsassociatedwithtop[i] == segmentsassociatedwithend[j]) {
                    finalPath.push_back(node(endCoord, 0));
                    cout << "We found a route between " << start << " and " << end << endl;
                    calculateNavSegments(directions, finalPath);
                    return NAV_SUCCESS;
                }
            }
        }
        
        for (int i = 0; i < segmentsassociatedwithtop.size(); i++) {
            GeoCoord nextSegment;
            
            if (segmentsassociatedwithtop[i].segment.start == currentlyProcessingGeoCoord) {
                nextSegment = segmentsassociatedwithtop[i].segment.end;
            } else {
                nextSegment = segmentsassociatedwithtop[i].segment.start;
            }
            
            bool alreadyVisited = false;
            
            for (int i = 0; i < finalPath.size(); i++) {
                if (finalPath[i].gc == nextSegment)
                    alreadyVisited = true;
            }
            if (alreadyVisited)
                continue;
            
            double nextSegmentVal = distanceEarthMiles(nextSegment, endCoord) + distanceEarthMiles(currentlyProcessingGeoCoord, nextSegment);
            m_nodequeue.push(node(nextSegment, nextSegmentVal));
            
        }


    }
    
    cout << "no route..." << endl;
    
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
