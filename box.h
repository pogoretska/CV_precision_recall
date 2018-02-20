#ifndef _BOX_INCLUDED
#define _BOX_INCLUDED


/*                              *
 * structure for bounding box   *
 *                              */

struct box {

    unsigned left, top, width, height;

    box() = default;    
    box(    unsigned init_left, 
            unsigned init_top, 
            unsigned init_width, 
            unsigned init_height ) :
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