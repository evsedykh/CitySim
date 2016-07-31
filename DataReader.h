#ifndef DATAREADER_H
#define DATAREADER_H

#include <string>
#include <memory>

#include "BaseDataReaderImpl.h"
#include "OsmData.h"
#include "OsmFilter.h"

//class BaseDataReaderImpl;

class DataReader
{
public:

    explicit DataReader( const std::string& format );

    virtual ~DataReader();

    std::unique_ptr< OsmData > ReadData( const std::string& filename ); // transfer ownership to client

    virtual void SetFilter( Osm::OsmFilter* filter );

private:

    std::shared_ptr< BaseDataReaderImpl > m_Reader;

};

#endif // DATAREADER_H
