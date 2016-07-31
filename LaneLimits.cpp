#include "LaneLimits.h"

LaneLimits::LaneLimits()
    : m_DriveForward(true),
      m_DriveBackward(false),
      m_PublicTransport(true),
      m_Cycles(true),
      m_Cars(true),
      m_Width(3.f)
{

}

LaneLimits::LaneLimits( const LaneLimits& other )
    : m_DriveForward(other.m_DriveForward),
      m_DriveBackward(other.m_DriveBackward),
      m_PublicTransport(other.m_PublicTransport),
      m_Cycles(other.m_Cycles),
      m_Cars(other.m_Cars),
      m_Width(other.m_Width)
{
}

LaneLimits& LaneLimits::operator= (const LaneLimits& other )
{
    m_DriveForward = other.m_DriveForward;
    m_DriveBackward = other.m_DriveBackward;
    m_PublicTransport = other.m_PublicTransport;
    m_Cycles = other.m_Cycles;
    m_Cars = other.m_Cars;
    m_Width = other.m_Width;
    return *this;
}

void LaneLimits::SetForwardDirectionOnly()
{
    m_DriveForward = true;
    m_DriveBackward = false;
}

void LaneLimits::SetBackwardDirectionOnly()
{
    m_DriveForward = false;
    m_DriveBackward = true;
}

void LaneLimits::SetBothDirections()
{
    m_DriveForward = true;
    m_DriveBackward = true;
}

void LaneLimits::SetPublicTransport()
{
    m_PublicTransport = true;
}

void LaneLimits::SetCycles()
{
    m_Cycles = true;
}

void LaneLimits::SetCarsOnly()
{
    m_Cars = true;
    m_Cycles = false;
}

void LaneLimits::SetWidth( qreal width )
{
    if( width <= MaxLaneWidth )
    {
        m_Width = width;
    } else
    {
        std::cerr << "Too wide road! Limit is " << MaxLaneWidth << "." << std::endl;
    }
}

boost::tribool LaneLimits::GetDirections() const
{
    boost::tribool result;
    if( m_DriveForward && ! m_DriveBackward ) result = true;
    if( m_DriveBackward && ! m_DriveForward ) result = false;
    if( m_DriveBackward && m_DriveForward ) result = boost::indeterminate;
    return result;
}

bool LaneLimits::GetPublicTransportAvailability() const
{
    if(m_PublicTransport) return true;
    return false;
}

bool LaneLimits::GetCycleAvailability() const
{
    if(m_Cycles) return true;
    return false;
}

bool LaneLimits::GetCarAvailability() const
{
    if(m_Cars) return true;
    return false;
}

qreal LaneLimits::GetWidth() const
{
    return m_Width;
}
