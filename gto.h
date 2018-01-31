#ifndef _GTO_DEFINED
#define _GTO_DEFINED

#include <string>
#include "box.h"
#include "objectclass.h"

using std::string;

/*                           *
 * structure describing gt   *
 *                           */
struct gto : box {
  ObjectClass cls;
  usint frame;
  bool detected = 0;

/* constructors */  
  gto() = default;
  gto( const string& ); // parse constructor

  bool operator> ( gto& );

  void print();
};



#endif