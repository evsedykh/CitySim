#ifndef BASEDATAREADERIMPL_H
#define BASEDATAREADERIMPL_H

#include "DataReader.h"
#include "OsmData.h"
#include "OsmFilter.h"

class BaseDataReaderImpl
{
public:
    virtual ~BaseDataReaderImpl(){}
    virtual void ReadData( const std::string& filename, bool read_nodes ) = 0; // transfer ownership to client
    virtual void SetFilter( Osm::OsmFilter* ) = 0;
    virtual std::unique_ptr< OsmData > GetData() = 0;

private:
};

#endif // BASEDATAREADERIMPL_H
