#!/bin/python

import sys, math

json_head = """
{
  "type": "FeatureCollection",
  "bbox": [ 29.998, 59.998, 30.003, 60.003 ], 
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

def angle(x1, y1, x2, y2, x3, y3, angle):
	angle_rad = math.pi*angle/180.
	line1 = line(x1, y1, x2, y2)
	length = math.sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2))
	x = x2 + length * math.cos(angle_rad)
	y = y2 + length * math.sin(angle_rad)
	return line1 + ',\n' + point(x, y)

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

if(len(sys.argv)!=2):
	print "Should be 1 parameter: angle in degrees. Exit"
	exit
alpha = float(sys.argv[1])

print json_head, angle(30.0, 60.0, 30.001, 60.0, 30.002, 60.0, alpha), json_tail

