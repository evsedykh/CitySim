#include "OsmFilter.h"

namespace Osm{

OsmFilter::OsmFilter()
{
}

OsmFilter::~OsmFilter()
{
}

void OsmFilter::AddFilter( const OsmFilterElement& filter )
{
    switch( filter.Entity ){
    case Node:
    {
        m_NodeFilters.push_back( filter.KeysValues );
        break;
    }
    case Way:
    {
        m_WayFilters.push_back( filter.KeysValues );
        break;
    }
    case Relation:
    {
        m_RelationFilters.push_back( filter.KeysValues );
        break;
    }
    case None:
    {
        std::cerr << "Can't set filter for None entity!" << std::endl;
    }
    }
}

bool OsmFilter::IsObligatoryKeyValue( OsmEntity entity, const std::pair< std::string, std::string>& kv )
{
    const std::vector< OsmFilterKeysValues >* filters = nullptr;
    switch( entity )
    {
    case Node:
        filters = &m_NodeFilters;
        break;
    case Way:
        filters = &m_WayFilters;
        break;
    case Relation:
        filters = &m_RelationFilters;
        break;
    case None: ;
    }
    if( filters == nullptr )
    {
        std::cerr << "Wrong OSM entity: " << entity << std::endl;
        return false;
    }
    for(auto it = filters->begin(); it != filters->end(); ++it)
    {
        if( it->ObligatoryKey == kv.first  && it->ObligatoryValues.find( kv.second ) != it->ObligatoryValues.end() )
        {
            return true;
        }
    }
    return false;
}

bool OsmFilter::IsAcceptedKey( OsmEntity entity, const std::string& key )
{
    const std::vector< OsmFilterKeysValues >* filters = nullptr;
    switch( entity )
    {
    case Node:
        filters = &m_NodeFilters;
        break;
    case Way:
        filters = &m_WayFilters;
        break;
    case Relation:
        filters = &m_RelationFilters;
        break;
    case None: ;
    }
    if( filters == nullptr )
    {
        std::cerr << "Wrong OSM entity: " << entity << std::endl;
        return false;
    }
    for(auto it: *filters)
    {
        if( it.AcceptedValues.find( key ) != it.AcceptedValues.end() )
        {
            return true;
        }
    }
    return false;
}

}
