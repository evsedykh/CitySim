#ifndef OSMFILTER_H
#define OSMFILTER_H

#include <string>
#include <set>
#include <map>
#include <vector>
#include <iostream>

namespace Osm {

enum OsmEntity{ Node, Way, Relation, None };

struct OsmFilterKeysValues
{
    std::string ObligatoryKey;
    std::set< std::string > ObligatoryValues;
    std::set< std::string > AcceptedValues;
};

struct OsmFilterElement
{
    OsmEntity Entity;
    OsmFilterKeysValues KeysValues;
};

class OsmFilter
{
public:
    OsmFilter();
    virtual ~OsmFilter();
    void AddFilter( const OsmFilterElement& filter );
    bool IsObligatoryKeyValue( OsmEntity entity, const std::pair< std::string, std::string>& kv );
    bool IsAcceptedKey( OsmEntity entity, const std::string& key );
private:
    std::vector< OsmFilterKeysValues > m_NodeFilters;
    std::vector< OsmFilterKeysValues > m_WayFilters;
    std::vector< OsmFilterKeysValues > m_RelationFilters;
};

}

#endif // OSMFILTER_H
