/*
 * dataStructs.hpp
 *
 *  Created on: Jul 23, 2015
 *      Author: Joe Polin
 */

#ifndef DATASTRUCTS_HPP_
#define DATASTRUCTS_HPP_

#include <stdexcept>

// Data structs
struct vertex;
typedef vertex vector3;

struct vertex {
	float x, y, z;
	vertex(float x_, float y_, float z_) {x = x_; y = y_; z = z_;}
	vertex() {x = 0; y = 0 ;z = 0; }
	vector3 operator-(vertex const v) {return vertex(x - v.x, y - v.y, z - v.z); }
  float operator[](size_t n) {
    if (n==0) return x;
    else if (n == 1) return y;
    else if (n == 2) return z;
    else throw std::out_of_range("Bad index for vector");
    
    return 0.0;
  };
};
struct texture {
	bool defined;
	float x, y, w;
  float operator[](size_t n) {
    if (n==0) return x;
    else if (n == 1) return y;
    else throw std::out_of_range("Bad index for vector");
    
    return 0.0;
  };

};

struct face {
	bool textured;
	vertex vert[3];
	texture text[3]; // Only defined if 'textures == true'
  vector3 norm[3]; // Vertex normals
};


#endif /* DATASTRUCTS_HPP_ */
