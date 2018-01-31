#include "box.h"
#include "cnd.h"

#include <iostream>
#include <vector>

using std::vector;
using std::stoi;

bool cnd::operator> (const cnd& acnd) {
    return ( confidence > acnd.confidence );
  }

void cnd::print() {
    std::cout << frame << ":" << static_cast<int>(cls) << ":" << confidence << ":" 
        << left << ":" << top << ":" << width << ":" << height << ":" 
        << detected << std::endl;
  }

cnd::cnd( const string& text) {
  vector<string> tokens;
  size_t start = 0, end = 0;
  
  while ((end = text.find(", ", start)) != string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 2;
  }
  tokens.push_back(text.substr(start));

  cnd::frame = stoi( tokens[0] );
  cnd::cls = class_map[ tokens[1] ];
  cnd::confidence = stof( tokens[2] );
  cnd::left = stoi( tokens[3] );
  cnd::top = stoi( tokens[4] );
  cnd::width = stoi( tokens[5] );
  cnd::height = stoi( tokens[6] );
}

