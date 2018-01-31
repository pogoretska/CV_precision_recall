#include <vector>
#include <iostream>

#include "box.h"
#include "gto.h"

using std::vector;
using std::stoi;

bool gto::operator> ( gto& agto ) {
    return ( area() > agto.area() );
  };

void gto::print() {
    std::cout << frame << ":" << static_cast<int>(cls) << ":" << left << ":" << top << ":" 
         << width << ":" << height << ":" << detected << std::endl;
  };

gto::gto( const string& text) { 
    vector<string> tokens;
    size_t start=0, end=0;
  
    while ((end = text.find(", ", start)) != string::npos) {
      tokens.push_back(text.substr(start, end - start));
      start = end + 2;
    }
    tokens.push_back(text.substr(start));
  
    gto::frame = stoi(tokens[0]);
    gto::cls = class_map[ tokens[1] ];
    gto::left = stoi(tokens[2]);
    gto::top = stoi(tokens[3]);
    gto::width = stoi(tokens[4]);
    gto::height = stoi(tokens[5]);
}
