#ifndef FORMULAES_H
#define FORMULAES_H

#include "stdafx.h"

namespace {

const qreal r = 6.371e6;
const qreal DEG_TO_RAD = M_PI / 180.;

QPointF LongLat2Mercator(const EarthCoordinates& coords, qreal lambda0 )
{
    QPointF xy;

    qreal theta = coords.latitude * DEG_TO_RAD;
    qreal lambda = coords.longitude * DEG_TO_RAD;
    std::cout << "LongLat2Mercator: r = " << r << " labmda = " << lambda << " lambda0 = " << lambda0
                 << " theta = " << theta << std::endl;

    xy.setX( r * ( lambda - lambda0 * DEG_TO_RAD));
    xy.setY( r * log(tan(theta/2.+M_PI/4.)));
    std::cout << "LongLat2Mercator: x = " << xy.x() << " y = " << xy.y() << std::endl;


    return xy;
}

}

#endif // FORMULAES_H
