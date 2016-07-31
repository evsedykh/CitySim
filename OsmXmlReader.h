#ifndef OSMXMLREADER_H
#define OSMXMLREADER_H

#include <set>
#include <map>
#include <unordered_set>
#include <list>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>

#include "libxml++/libxml++.h"

#include "DataReader.h"
#include "MappedStorage.h"
#include "OsmFilter.h"

struct osm_bounds_data{
    double minlat;
    double minlon;
    double maxlat;
    double maxlon;
};

typedef std::set< size_t > OsmTags;

struct osm_node_data{
    double node_lon;
    double node_lat;
    OsmTags node_tags;
};

enum osm_relation_member_type{
    node, way, relation
};

struct osm_relation_member_data
{
    osm_relation_member_type member_type;
    int64_t member_ref;
    size_t member_role;
};

struct osm_relation_data{
    std::vector< osm_relation_member_data > relation_members;
    OsmTags relation_tags;
};

struct osm_way_data{
    std::vector< int64_t > way_nodes;
    OsmTags way_tags;
};

struct NodeInfo{
    double longitude;
    double latitude;
    std::list< OsmTags > node_tags;
    std::list< OsmTags > way_tags;
    std::list< OsmTags > relation_tags;
};

struct osm_xml_data{
    osm_bounds_data bounds;
    std::map< int64_t, osm_node_data > nodes;
    std::map< int64_t, osm_way_data > ways;
    std::map< int64_t, osm_relation_data > relations;
};

class OsmXmlReader :
        public BaseDataReaderImpl
        , public xmlpp::SaxParser
{
public:
    OsmXmlReader();
    virtual ~OsmXmlReader();
    virtual void ReadData( const std::string& filename, bool read_nodes ); // transfer ownership to client
    virtual void SetFilter( Osm::OsmFilter* filter );
    virtual std::unique_ptr< OsmData > GetData();

protected:
  void on_start_document() override;
  void on_end_document() override;
  void on_start_element(const Glib::ustring& name, const AttributeList& properties) override;
  void on_end_element(const Glib::ustring& name) override;
  void on_characters(const Glib::ustring& characters) override;
  void on_comment(const Glib::ustring& text) override;
  void on_warning(const Glib::ustring& text) override;
  void on_error(const Glib::ustring& text) override;
  void on_fatal_error(const Glib::ustring& text) override;

private:
  std::unique_ptr< OsmData > ConvertXmlDataToOsmData();

private:
  osm_xml_data m_XmlData;
  osm_bounds_data m_CurrBounds;
  osm_node_data m_CurrNode;
  osm_way_data m_CurrWay;
  osm_relation_data m_CurrRelation;
  OsmTags m_CurrTags;
  int64_t m_CurrId;
  MappedStorage<std::pair<std::string, std::string> > m_TagStorage;
  MappedStorage<std::string> m_RoleStorage;
  std::unordered_set< std::uint64_t > m_NodesOfInterest;
  Osm::OsmFilter* m_Filter;

  Osm::OsmEntity m_Entity;
  bool m_SaveEntity;
  bool m_ReadNodes;
};

#endif // OSMXMLREADER_H
