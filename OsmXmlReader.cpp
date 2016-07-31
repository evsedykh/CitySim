#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "stdafx.h"
#include "OsmXmlReader.h"
#include <glibmm/convert.h> //For Glib::ConvertError

OsmXmlReader::OsmXmlReader()
    : m_Entity( Osm::None )
    , m_SaveEntity( false )
    , m_ReadNodes( false )
{
}

OsmXmlReader::~OsmXmlReader()
{
}

void OsmXmlReader::ReadData( const std::string& filename, bool read_nodes )
{
    m_ReadNodes = read_nodes;
    try
    {
        std::ifstream is(filename.c_str());
        if (!is)
            throw xmlpp::exception("Could not open file " + filename);

        char buffer[ 640000 ];
        const size_t buffer_size = sizeof(buffer) / sizeof(char);

        set_substitute_entities(true);
        do
        {
            std::memset(buffer, 0, buffer_size);
            is.read(buffer, buffer_size-1);
            if(is.gcount())
            {
                Glib::ustring input(buffer, buffer+is.gcount());
                parse_chunk(input);
            }
        }
        while(is);

        finish_chunk_parsing();
        is.close();
    }
    catch(const xmlpp::exception& ex)
    {
        std::cerr << "Incremental parsing, libxml++ exception: " << ex.what() << std::endl;
    }
/*
    std::cout << "Nodes found:" << std::endl;
    for( auto it_node:  m_XmlData.nodes)
    {
        OsmTags tags = it_node.second.node_tags;
        for( auto it_tag: tags)
        {
            auto tag = m_TagStorage[ it_tag ];
            std::cout << tag.first << ": " << tag.second << "\n";
        }
        std::cout << std::endl;
    }

    std::cout << "Ways found:" << std::endl;
    for( auto it_way:  m_XmlData.ways)
    {
        OsmTags tags = it_way.second.way_tags;
        for( auto it_tag: tags)
        {
            auto tag = m_TagStorage[ it_tag ];
            std::cout << tag.first << ": " << tag.second << "\n";
        }
        std::cout << std::endl;
    }

    std::cout << "Relations found:" << std::endl;
    for( auto it_rel:  m_XmlData.relations)
    {
        OsmTags tags = it_rel.second.relation_tags;
        for( auto it_tag: tags)
        {
            auto tag = m_TagStorage[ it_tag ];
            std::cout << tag.first << ": " << tag.second << "\n";
        }
        std::cout << std::endl;
    }
*/
}

void OsmXmlReader::SetFilter( Osm::OsmFilter* filter )
{
    m_Filter = filter;
}

std::unique_ptr< OsmData > OsmXmlReader::GetData()
{
    return ConvertXmlDataToOsmData();
}

void OsmXmlReader::on_start_document()
{
}

void OsmXmlReader::on_end_document()
{
}

void OsmXmlReader::on_start_element(const Glib::ustring& name,
                                    const AttributeList& attributes )
{
    if( m_ReadNodes )
    {
        if( name == "node" )
        {
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("id"));
            m_CurrId = boost::lexical_cast< int64_t >( attr->value );
            if( m_NodesOfInterest.find( m_CurrId ) == m_NodesOfInterest.end() )
            {
                return;
            }
            m_Entity = Osm::Node;
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("lat"));
            m_CurrNode.node_lat = boost::lexical_cast< double >( attr->value );
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("lon"));
            m_CurrNode.node_lon = boost::lexical_cast< double >( attr->value );
            std::cout << "Node found! id = " << m_CurrId << " lon = " << m_CurrNode.node_lon << " lat = " << m_CurrNode.node_lat << std::endl;
            m_SaveEntity = true;
            return;
        }
    } else {
        if( name == "bounds" )
        {
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("minlat"));
            m_XmlData.bounds.minlat = boost::lexical_cast< double >( attr->value );
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("maxlat"));
            m_XmlData.bounds.maxlat = boost::lexical_cast< double >( attr->value );
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("minlon"));
            m_XmlData.bounds.minlon = boost::lexical_cast< double >( attr->value );
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("maxlon"));
            m_XmlData.bounds.maxlon = boost::lexical_cast< double >( attr->value );
        }

        if( name == "way" )
        {
            m_Entity = Osm::Way;
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("id"));
            m_CurrId = boost::lexical_cast< int64_t >( attr->value );
            //std::cout << "Way found! id = " << m_CurrId << std::endl;
            return;
        }
        if( name == "relation" )
        {
            m_Entity = Osm::Relation;
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("id"));
            m_CurrId = boost::lexical_cast< int64_t >( attr->value );
            //std::cout << "Relation found! id = " << m_CurrId << std::endl;
            return;
        }
        if( name == "tag" && m_Entity != Osm::None)
        {
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("k"));
            std::string key = attr->value.raw();
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("v"));
            std::string value = attr->value.raw();
            auto tag = std::make_pair(key, value);
            if(m_Filter->IsObligatoryKeyValue(m_Entity, tag))
            {
                m_SaveEntity = true;
                size_t ntag = m_TagStorage.insert(tag);
                m_CurrTags.insert( ntag );
            }
            if( m_Filter->IsAcceptedKey(m_Entity, key))
            {
                size_t ntag = m_TagStorage.insert(tag);
                m_CurrTags.insert( ntag );
            }
            return;
        }
        if( name == "nd" && m_Entity == Osm::Way )
        {
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("ref"));
            int64_t ref = boost::lexical_cast< int64_t >(attr->value);
            m_CurrWay.way_nodes.push_back( ref );
            m_NodesOfInterest.insert( ref );
            //std::cout << "\tNode reference found = " << ref << std::endl;
            return;
        }
        if( name == "member" && m_Entity == Osm::Relation )
        {
            osm_relation_member_data member_data;
            auto attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("ref"));
            member_data.member_ref = boost::lexical_cast< int64_t >(attr->value);
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("role"));
            std::string role = attr->value.raw();
            member_data.member_role = m_RoleStorage.insert(role);
            attr = std::find_if(attributes.begin(), attributes.end(), AttributeHasName("type"));
            std::string type = attr->value.raw();
            if( type == "node" ) member_data.member_type = node;
            else if( type == "way" ) member_data.member_type = way;
            else if( type == "relation" ) member_data.member_type = relation;
            //std::cout << "\tMember found! type: " << (int)member_data.member_type << " ref: "
            //          << member_data.member_ref << " role: " << member_data.member_role << std::endl;
            m_CurrRelation.relation_members.push_back( member_data );
            return;
        }
    }
}

void OsmXmlReader::on_end_element(const Glib::ustring& name )
{
    if( m_ReadNodes )
    {
        if( name == "node" )
        {
            m_Entity = Osm::None;
            if( m_SaveEntity )
            {
                m_CurrNode.node_tags = m_CurrTags;
                m_XmlData.nodes.insert( std::make_pair(m_CurrId, m_CurrNode));
                m_CurrNode.node_tags.clear();
                m_SaveEntity = false;
            }
            //std::cout << "insert node id = " << m_CurrId << " lat = " << m_CurrNode.node_lat << " lon = " << m_CurrNode.node_lon << std::endl;
            //for( auto it: m_CurrNode.node_tags )
            //{
            //  auto tag = m_TagStorage[ it ];
            //  std::cout << "tag: storage# " << it << " " << tag.first << ": " << tag.second << std::endl;
            //}
            m_CurrTags.clear();
        }
    } else {
        if( name == "way")
        {
            m_Entity = Osm::None;
            if( m_SaveEntity )
            {
                m_SaveEntity = false;
                m_CurrWay.way_tags = m_CurrTags;
                m_XmlData.ways.insert( std::make_pair(m_CurrId, m_CurrWay));
                m_CurrWay.way_nodes.clear();
                m_CurrWay.way_tags.clear();
            }
            m_CurrTags.clear();
        }
        if( name == "relation")
        {
            m_Entity = Osm::None;
            if( m_SaveEntity )
            {
                m_CurrRelation.relation_tags = m_CurrTags;
                m_XmlData.relations.insert( std::make_pair(m_CurrId, m_CurrRelation));
                m_CurrRelation.relation_members.clear();
                m_CurrRelation.relation_tags.clear();
            }
            m_CurrTags.clear();
        }
    }
}

void OsmXmlReader::on_characters(const Glib::ustring& )
{
}

void OsmXmlReader::on_comment(const Glib::ustring& )
{
}

void OsmXmlReader::on_warning(const Glib::ustring& )
{
}

void OsmXmlReader::on_error(const Glib::ustring& )
{
}

void OsmXmlReader::on_fatal_error(const Glib::ustring& )
{
}

std::unique_ptr< OsmData > OsmXmlReader::ConvertXmlDataToOsmData()
{
    std::unique_ptr< OsmData > osm_data( new OsmData );

    // nodes
    for( auto it_node: m_XmlData.nodes )
    {
        std::cout << "lon = " << it_node.second.node_lon << " lat = " << it_node.second.node_lat << std::endl;
        NodeDataImpl node_data( it_node.second.node_lon, it_node.second.node_lat );
        osm_data->AddNode( it_node.first, node_data );
    }

    // ways
    for( auto it_way: m_XmlData.ways )
    {
        bool is_highway = false;
        for( auto it_tag: it_way.second.way_tags )
        {
            if( m_TagStorage[ it_tag ].first == "highway" )
            {
               is_highway = true;
               break;
            }
        }
        if( is_highway )
        {
            RoadSegmentDataImpl segment_data;
            //std::cout << "Way " << it_way.first << " is highway with nodes:" << std::endl;
            for( auto it_node: it_way.second.way_nodes )
            {
                //qreal longitude = m_XmlData.nodes[ it_node ].node_lon;
                //qreal latitude = m_XmlData.nodes[ it_node ].node_lat;

                //std::cout << " " << it_node << "( " << longitude << ", " << latitude << ") ,";
                //EarthCoordinates coords( longitude, latitude);
                segment_data.nodes.push_back( it_node );
                /*if( ! m_XmlData.nodes[ it_node ].node_tags.empty() )
                {
                    std::cout << "tags: ";
                    for( auto it_tag: m_XmlData.nodes[ it_node ].node_tags )
                    {
                        std::cout << m_TagStorage[ it_tag ].first << " = " << m_TagStorage[ it_tag ].second << ", ";
                    }
                }*/
            }
            std::cout << std::endl;

            osm_data->AddRoadSegment( segment_data );
        }
    }

    return osm_data;
}
