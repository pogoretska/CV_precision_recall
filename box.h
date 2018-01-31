#ifndef _BOX_INCLUDED
#define _BOX_INCLUDED


using usint = unsigned short int;
static_assert(sizeof(usint) >= 2, "short int is too short! minimum 2 bytes needed");
/*                              *
 * structure for bounding box   *
 *                              */

struct box {

    usint left, top, width, height;

    box() = default;    
    box(    usint init_left, 
            usint init_top, 
            usint init_width, 
            usint init_height ) :
      left{init_left}, 
      top{init_top},
      width{init_width},
      height{init_height} 
      {};

    const int area() const { return width*height; }

    const bool overlaps( const box& );
    const int intersection_area( const box& ); 
    const int union_area( const box& ); 
};

#endif