#include "OsmGeojsonReader.h"

OsmGeojsonReader::OsmGeojsonReader()
{
}

void OsmGeojsonReader::ReadData( const std::string&, bool )
{
}

std::unique_ptr< OsmData > OsmGeojsonReader::GetData()
{
    return nullptr;
}
