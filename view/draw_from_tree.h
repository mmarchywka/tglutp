#ifndef DRAW_FROM_TREE_H__
#define DRAW_FROM_TREE_H__
#include "./view/display_tree.h"
// need types of things that can be displayed...
//#include "./atom_list.h"
#include <iostream>
#include <sstream>
#include <string>

class draw_from_tree_TR
{
	public:
	typedef char Ch;
typedef std::basic_istream<Ch>  input_type;
typedef std::basic_string<Ch> string_type;
typedef string_type name_type;

typedef int count_type;
typedef int flag_type;
typedef input_type IsTy;
typedef float coord_type;
typedef std::istringstream ConvStreamTy;
typedef std::ostringstream OsStreamTy;
	
};
/*
class draw_from_tree : public draw_from_tree_TR
{
	typedef draw_from_tree Myt;
		
public: 
	typedef display_tree display_type;
	typedef display_type DispTy;
	typedef DispTy* DispPtr;
	typedef AtomLiSTy AListTy;
	typedef AListTy * AListPtr;
	typedef ConnLiSTy CListTy;
	typedef CListTy * CListPtr;
	draw_from_tree() {}	
	
	void list(const DispTy  * const root) const
	{root->traverse(std::cout,0); }
	void draw(const DispPtr root, const AListPtr ap, const CListPtr cp)
	{
		// root should have 2 children- 
		
	}
	
	
	
	
};
*/

#endif

