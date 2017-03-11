#include "provided.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    
private:
    int m_numSegments;
    vector<StreetSegment*> m_streetSegments;
};

MapLoaderImpl::MapLoaderImpl()
{
    m_numSegments = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
    int numLines = 0;
    string line;
    std::ifstream ifs (mapFile, std::ifstream::in);
    if (ifs.is_open()) {
        
        while ( getline (ifs, line) )
        {
            numLines++;
        }
        ifs.close();
        cout << "numLinse: " << numLines << endl;
        return true;

    }
    
    else {
        return false;
    }
}

size_t MapLoaderImpl::getNumSegments() const
{
    return m_numSegments; // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	return false;  // This compiles, but may not be correct
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
