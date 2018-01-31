#ifndef _CND_INCLUDED
#define _CND_INCLUDED

#include <string>
#include "objectclass.h"
#include "box.h"

using std::string;

/*                           *
 * structure describing cnd  *
 *                           */
struct cnd : box {
  usint frame;
  ObjectClass cls;
  float confidence;
  bool detected = 0;
  int gtnumber = -1;

/* constructors */
  cnd() = default;
  cnd( const string& );
  
  bool operator> (const cnd& );

  void print();

};



#endif