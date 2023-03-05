/*
  Dave Skura, 2022
*/

#include "Arduino.h" 

class rotate_coords {
public:
	String Version = "20220623";
	int x = 0;
	int y = 0;
	int new_x = 0;
	int new_y = 0;
	rotate_coords(int coord_x, int coord_y);
	void rotate(double degrees);
private:

};

rotate_coords::rotate_coords(int coord_x, int coord_y) {
	x = coord_x;
	y = coord_y;
}

void rotate_coords::rotate(double degrees) {
	new_x = cos(degrees)*x - sin(degrees)*y;
	new_y = sin(degrees)*x + cos(degrees)*y; 
}
