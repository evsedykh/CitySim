
#include "stdafx.h"

RoadNetworkGenerator::RoadNetworkGenerator()
{
}

void RoadNetworkGenerator::Generate()
{
    Coordinates coord_Muzhestva{400., 200.};
    Coordinates coord_Svetlana{200., 160.};
    Coordinates coord_Polytech{410., 120.};
    Coordinates coord_Benua{405., 40.};
    Coordinates coord_Idoly{310., 105.};
    Coordinates coord_Sputnik{350., 140.};
    Coordinates coord_Gidrotekhnikov{405., 140.};


    m_Network.AddKeyPoint(0, coord_Muzhestva);
    m_Network.AddKeyPoint(1, coord_Svetlana);
    m_Network.AddKeyPoint(2, coord_Polytech);
    m_Network.AddKeyPoint(3, coord_Benua);
    m_Network.AddKeyPoint(4, coord_Idoly);
    m_Network.AddKeyPoint(5, coord_Sputnik);

    m_Network.AddRoadSegment(0, 1);
    m_Network.AddRoadSegment(1, 4);
    m_Network.AddRoadSegment(4, 3);
    m_Network.AddRoadSegment(3, 2);
    m_Network.AddRoadSegment(2, 0);
    m_Network.AddRoadSegment(4, 5);
    m_Network.AddRoadSegment(5, 0);



}


const RoadNetwork& RoadNetworkGenerator::GetRoadNetwork()
{
    return m_Network;
}

