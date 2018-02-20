#include <algorithm>
#include "box.h"

using std::min;
using std::max;

/* method for checking if this object overlaps another, given as argument */
const bool box::overlaps( const box& abox ) { 
      
      auto lbound = max(left, abox.left);                    // left bound of intersection
      auto rbound = min(left + width, abox.left + abox.width); // right bound
      auto tbound = max(top, abox.top);                      // top bound
      auto bbound = min(top + height, abox.top + abox.height); // bottom bound
      
      return (( lbound < rbound ) && ( tbound < bbound )); 
      // if left bound of intersection is lefter than its right bound 
      // AND top bound upper than bottom, objects overlap
}

/* method for calculating intersection area */
    const int box::intersection_area( const box& abox ) {
      return ( overlaps( abox ) ) ? 
            ( min(left + width, abox.left + abox.width) - max(left, abox.left) )
            *( min(top + height, abox.top + abox.height) - max(top, abox.top) ) 
            : 0; // if overlaps, then return intersection, otherwise return 0
    }

/* method for calculating union area */    
    const int box::union_area( const box& abox ) {           // area of union of overlapping objects is sum of their areas minus intersection area
      return area() + abox.area() - intersection_area(abox);
    }

