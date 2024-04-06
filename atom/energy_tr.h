#ifndef ENERGY_TRAITSS_H__
#define ENERGY_TRAITSS_H__

#include "../data_and_config.h"

#include <sstream>
#include <string>
#include <iostream>
#include <vector>

class energy_tr
{
	public:
	typedef int count_type;
	typedef int index_type;
	typedef float coord_type;
	typedef char Ch;
	typedef std::basic_string<Ch> string_type;	
	typedef std::basic_ostream<Ch>  ostream_type;
	typedef std::istringstream string_parse_type;
	typedef std::ostringstream string_buffer_type; 
	
	
};

#include "../atom/pdb_atom.h"
class caving_tr {
	public:
typedef pdb_atom target_type;

// get common stuff from somewhere
typedef target_type Stuff;
//typedef Stuff::coord_type coord_type;
//typedef Stuff::count_type count_type;

typedef target_type TgtTy;
typedef const TgtTy * TgtPtr;

typedef std::vector<TgtTy> TgtListTy;
typedef TgtListTy::iterator TgtItorTy;
typedef TgtListTy::const_iterator TgtCItorTy;
typedef std::vector<TgtPtr> PtrListTy;
typedef PtrListTy::iterator PtrItorTy;
typedef PtrListTy::const_iterator PtrCItorTy;
};





#endif
