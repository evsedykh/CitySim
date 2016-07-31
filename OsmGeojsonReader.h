#ifndef OSMGEOJSONREADER_H
#define OSMGEOJSONREADER_H

#include "DataReader.h"

class OsmGeojsonReader : public BaseDataReaderImpl
{
public:
    OsmGeojsonReader();
    virtual void ReadData( const std::string&, bool read_nodes ); // transfer ownership to client
    virtual void SetFilter( Osm::OsmFilter* ) { }
    virtual std::unique_ptr< OsmData > GetData();

};

#endif // OSMGEOJSONREADER_H
