#ifndef LANELIMITS_H
#define LANELIMITS_H

#include "stdafx.h"

enum LaneTurnRules{ reverse = 1 , left = 2, sharp_left = 4, slight_left = 8,
                    through = 16, slight_right = 32, sharp_right = 64, right = 128,
                    none = 256, merge_to_left = 512, merge_to_right = 1024 };

class LaneLimits
{
public:
    LaneLimits();
    LaneLimits( const LaneLimits& other );
    LaneLimits& operator= (const LaneLimits& other );
    // setters
    void SetForwardDirectionOnly();
    void SetBackwardDirectionOnly();
    void SetBothDirections();

    void SetPublicTransport();
    void SetCycles();
    void SetCarsOnly();
    void SetWidth( qreal width );
    // getters
    unsigned int GetSpeedLimit() const;
    boost::tribool GetDirections() const;
    bool GetPublicTransportAvailability() const;
    bool GetCycleAvailability() const;
    bool GetCarAvailability() const;
    qreal GetWidth() const;

private:
    bool m_DriveForward;        // true
    bool m_DriveBackward;       // false
    bool m_PublicTransport;     // false
    bool m_Cycles;              // false
    bool m_Cars;                // false
    qreal m_Width;              // 3.5 m
    unsigned int m_TurnRules;
};

#endif // LANELIMITS_H
