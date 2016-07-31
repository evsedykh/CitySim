#!/bin/python

import math

json_head = """
{
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {
        "foot": "no",
        "highway": "secondary",
        "lanes": "1",
        "maxspeed": "RU:urban",
        "name": "Test street",
        "surface": "asphalt"
      },
      "geometry": {
        "type": "LineString",
        "coordinates": [
"""

indent = "                ";

json_body = """
		[ 60.0, 30.0 ],
		[ 61.0, 30.0 ],
		[ 61.0, 31.0 ],
		[ 60.0, 31.0 ],
		[ 60.0, 30.0 ] """

json_tail = """
        ]
      }
    }
  ]
}
"""

def point(x, y):
	return indent + '[ ' + str(x) + ', ' + str(y) + ' ]'

def line(x1, y1, x2, y2):
	return point(x1, y1) + ',\n' + point(x2, y2)

def rect(x1, y1, x2, y2):
	return line(x1, y1, x2, y1) + ',\n' + line(x2, y2, x1, y2) + ',\n' + point(x1, y1)

def ellipse(x, y, r1, r2):
	res = ''
	delta = 0.03
        phi = 0.
	while(phi<=(2.*math.pi-delta)):
		curr_x = r1 * math.cos(phi) + x
		curr_y = r2 * math.sin(phi) + y
		res += point(curr_x, curr_y) + ',\n'
		phi+=delta
	res += point(x+r1, y)
	return res

#print json_head, line(30.0, 60.001, 30.001, 60.0), json_tail
print json_head, ellipse(30.0, 60.0, 0.001, 0.0005), json_tail

