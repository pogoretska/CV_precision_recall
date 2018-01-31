#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <memory>
#include <algorithm>
#include <numeric>

#include "gto.h"
#include "cnd.h"

/* helper function to read data from input files */
template <typename T>
int read( const std::string& filename, std::vector<T>& container) {

  std::ifstream datafile( filename );
//  T tmp; 
  
  if ( datafile.is_open() ) {
    std::string line;
    while ( getline( datafile, line )) {
        
      try {
  	// construct new element based on line read and push it into container
        container.push_back( T(line) ); 
      } 
      catch (...) {
        return 2;
      }
    }
        
    datafile.close();
 
  } else 
      return 1; // couldn't open file 
  
  return 0;
}



/************************
 *    main function	*
 ************************/

int main( int argc, char *argv[] ) {

  // arrays for storing original data read from input files
  std::vector<gto> gtos; 
  std::vector<cnd> cnds; 

/* --------------------   Reading input files and parsing contents into arrays  -------------------- */

  if ( argc != 3 ) { // argc should be 2 for correct execution

    std::cout << "usage: " << argv[0] << " <gtfile> <cndfile>\n";
    exit(-1);
    
  } 

  // argc has all arguments, let's start reading
  if ( auto err = read( argv[1], gtos) ) {
    switch( err ) {
      case 1 : 
        std::cout << "Error reading GT file, exiting :(" << std::endl;
        break;
      case 2 :
        std::cout << "Could not construct a GT element from given input, exiting :(" << std::endl;
    }
    exit(-1);

  } else std::cout << "GT file read successfully" << std::endl;
    
  if ( auto err = read( argv[2], cnds) ) {
    switch( err ) {
      case 1 :
        std::cout << "Error reading CND file, exiting :(" << std::endl;
        break;
      case 2 :
        std::cout << "Could not construct a CND element from given input, exiting :(" << std::endl;
    }
    exit(-1);

  } else std::cout << "CND file read successfull" << std::endl;


/* --------------------   Loop through candidates to find gts 			*/
/*                        with matching frame no & class 			*/
/*                        and overlapping bounding boxes   -------------------- */

  std::vector< std::pair<size_t, std::vector<size_t> > > matches; 
  // vector of matches: pair<cnd number, vector of numbers of matching gtos>
    
  for ( size_t i = 0; i < cnds.size(); i++ ) { 
    bool flag = false; 				// no match yet    
    for ( size_t j = 0; j < gtos.size(); j++ ) { 
      if ( gtos[j].frame > cnds[i].frame ) 	// reaching gt with frame number exceeding candidate's frame number 
        break; 					// - don't need to continue looping, since they're sorted by frame no
      else if (( cnds[i].frame == gtos[j].frame ) 
                && ( cnds[i].cls == gtos[j].cls ) 
                && cnds[i].overlaps( gtos[j] )) { // found overlapping gt with same frame no and class
        if (!flag) {
          matches.push_back(make_pair(i, std::vector<size_t>{j}));
          flag = true;
        } else
          matches.back().second.push_back(j);	
      }
    } // end for each gto for i-th cnd
  }


  std::cout << "total gtos " << gtos.size() << ", total cnds " << cnds.size() << std::endl;

  for (auto m : matches) {

////////////////*** check check for timing - what's quicker, with "if" or without */////
    if (m.second.size() - 1 > 0)	{ 		
      std::sort(m.second.begin(), m.second.end(), 
        [&] (size_t i, size_t j) { return gtos[i] > gtos[j]; });	// sort bigger area first
  }

  for (auto i : m.second)	// check sorted candidates starting with ones with bigger area
    if ( (float)cnds[m.first].intersection_area( gtos[i] ) 
            / cnds[m.first].union_area( gtos[i] ) > 0.5 ) {  
        gtos[ i ].detected = 1;                                                                                     
        cnds[ m.first ].detected = 1;                                                                                                          
        cnds[ m.first ].gtnumber = i;                                                                                      
        break;            
      } // end if detected

  }  

    

/* --------------------  Filter out candidates matching same gt    -------------------- */
  
  std::map<size_t, size_t> final_match;	// <gto index, detected cnd index>
  
  for (size_t i = 0; i < cnds.size(); ++i) {
    if (cnds[i].detected) {
        auto res = final_match.emplace(cnds[i].gtnumber, i);
        if (!res.second) {
          if (cnds[final_match[cnds[i].gtnumber]].confidence < cnds[i].confidence) {
            cnds[final_match[cnds[i].gtnumber]].detected = false;
            final_match[cnds[i].gtnumber] = i;
          } else {
            cnds[i].detected = false;
          }
        }
    }
  }
    
  std::cout << "Total matches: " << final_match.size()  
            << ", while " << gtos.size()-final_match.size() 
            << " ground truth objects and " 
            << cnds.size()-final_match.size() 
            << " candidates left undetected" << std::endl;
  

/* --------------------  Build precision table  -------------------- */

  std::vector<int> tp(10,0); // true positives
  std::vector<int> fp(10,0); // false positives

  for (auto& c : cnds)  
    if (c.confidence < 0.1) 
      c.detected ? ++tp[0] : ++ fp[0];    
    else if (c.confidence < 0.2) 
      c.detected ? ++tp[1] : ++ fp[1];
    else if (c.confidence < 0.3) 
      c.detected ? ++tp[2] : ++ fp[2];
    else if (c.confidence < 0.4)
      c.detected ? ++tp[3] : ++ fp[3];
    else if (c.confidence < 0.5) 
      c.detected ? ++tp[4] : ++ fp[4];
    else if (c.confidence < 0.6) 
      c.detected ? ++tp[5] : ++ fp[5];
    else if (c.confidence < 0.7) 
      c.detected ? ++tp[6] : ++ fp[6];
    else if (c.confidence < 0.8) 
      c.detected ? ++tp[7] : ++ fp[7];
    else if (c.confidence < 0.9)
      c.detected ? ++tp[8] : ++ fp[8];
    else
      c.detected ? ++tp[9] : ++ fp[9];

  auto tp_total = std::accumulate(tp.begin(), tp.end(), 0);
  auto fp_total = std::accumulate(fp.begin(), fp.end(), 0);

  std::cout << std::endl;
  std::cout << std::setw(15) << "diapason" 
          << std::setw(10) << "TP" 
          << std::setw(10) << "FP" 
          << std::setw(20) << "Precision" 
          << std::setw(20) << "Recall" << std::endl;

  for (auto i = 0; i < 10; i++) { 
    std::cout << std::setw(6) << "[" << std::setw(3) << (float)i*0.1 << ".." << std::setw(3) << (float)(i+1)*0.1 << ")" 
            << std::setw(10) << tp[i] 
            << std::setw(10) << fp[i] 
            << std::setw(20) << (float)tp[i] / (tp[i] + fp[i]) 
            << std::setw(20) << (float)tp[i] / gtos.size()
            << std::endl;    
  }

  std::cout << std::setw(15) << "Total"
            << std::setw(10) << tp_total
            << std::setw(10) << fp_total
            << std::setw(20) << (float)tp_total / (tp_total + fp_total)
            << std::setw(20) << (float)tp_total / gtos.size()
            << std::endl;
  return 0;

}
