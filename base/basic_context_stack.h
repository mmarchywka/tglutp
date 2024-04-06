#ifndef BASIC_CONTEXT_STACK_H__
#define BASIC_CONTEXT_STACK_H__

#include "../base/basic_symbol_store.h"

#include <map>
#include <vector>
	
	class context_typedefs
	{public:
	
	typedef char Ch;
	typedef unsigned int index_type;
	typedef std::basic_string<Ch> string_type;
	typedef string_type StTy;
	
	};
	
	// inegrate later...
class MethodClass : public context_typedefs
	{ 
		public: 
		typedef std::vector<string_type> SubTy;
		typedef std::map<string_type,index_type> LabelTableTy;
		SubTy code; 
		LabelTableTy labels;  
		
		void add_label(const string_type & label) 
		{	index_type target=code.size();
			mmdd::dout(MMDDLOC)<<"Adding laberl "<<label<<" tar="<<target<<"\n";
			labels[label]=target;
		}
		index_type find_label(const string_type & lbl) const 
		{
		//LabelTableTy& lt=ci.method->labels;
		LabelTableTy::const_iterator ii=labels.find(lbl),ie=labels.end();
		if (ii==ie)
		{	mmdd::dout(MMDDLOC)<<"Label not found :" <<lbl <<" in \n"; 
			LabelTableTy::const_iterator ii=labels.begin();
			while (ii!=ie) 
				{std::cout<<(*ii).first<<" "<<(*ii).second<<"\n"; ++ii; }
			return 0; 
		}
		return (*ii).second;
		}
		
	}; 
	
	typedef std::map<MethodClass::string_type,MethodClass> MethodMapBaseTy; 
class MethodMapTy : public MethodMapBaseTy,public context_typedefs
{ 
	public: 
	bool has(const string_type & nm) const 	{return (find(nm)!=end());}
	void erase( const string_type & nm)
	{ 	if (has(nm)) MethodMapBaseTy::erase(find(nm));  }
	template <class Os> void dump (Os& os)
	{
		MethodMapBaseTy::iterator ii=begin(),ie=end();
		while (ii!=ie) 
			{os<<(*ii).first<<" "<<(*ii).second.code[0]<<"\n"; ++ii; }
	}
};

class ContextInfo:public context_typedefs
{
	public: 
	typedef basic_symbol_store SymTy;
	typedef MethodClass method_type;
	ContextInfo(): method(0),pc(0),nm("wtf") {} 	
	ContextInfo(method_type * m, const SymTy & globals, const SymTy& locals)
		:syms(globals),method(m),pc(1)
	{ 	syms+=locals; }	
	//string_type operator()() { return method->code[pc++]; }
	//bool more()const { return method->code.size()<pc; }
	bool returning() const {return (pc>=(method->code.size()-1)); }  
	const string_type & next()  
	{
		const string_type & inst=method->code[pc];
		pc++; 
		return inst;
	}
	template <class Os> void dump( Os& os) const
	{
		if (method!=NULL)
		os<<nm<<" at pc="<<pc <<" of "<<(method->code.size())<<"\n"; 
	else os<<nm<<" at pc="<<pc <<" of "<<0<<"\n"; 
	}
	// change these to pointers...
	SymTy syms; 
	method_type * method;
	index_type pc;
	string_type nm; // just for debuggin...
	
};


typedef std::list<ContextInfo> ContextBaseTy; 
class ContextStackTy : public ContextBaseTy,public context_typedefs
{
	public:
		typedef ContextInfo::SymTy SymTy;
		bool at_global() const { return (size()==1);}
		// the global context is not valie- these will explode...
		bool returning() const {
		//const ContextInfo * ci =&back();
		// done't execut the end statement????
		return back().returning(); }  
		const string_type & next() {//ContextInfo & ci =back();
		// string_type inst=ci.method->code[ci.pc];
		//ci.pc++; return inst; 
		return back().next(); }
	
	void call(const StTy & nm,MethodClass* me,const SymTy & params)
	{
		//ContextInfo ci=ContextInfo();
		//ci.method=me; ci.syms=back().syms;  ci.syms+=params; ci.pc=1;
		push_back(ContextInfo(me,back().syms,params));
		back().nm=nm;
	}
	
	bool jump(const string_type & lbl)
	{	ContextInfo & ci =back();
		index_type tgt=ci.method->find_label(lbl);
		if (tgt!=0) ci.pc=tgt; 
		//ci.pc=lt[lbl];
		return true;
	}
	// always operate on local contexts...
	StTy  find_variable(const StTy & nm)
	{ SymTy & local=back().syms; if (local.has(nm)) return local[nm];
/*	bool have_stack=(size()!=1);
	SymTy & global=front().syms;
	SymTy & local=back().syms;
	if (have_stack) 
	{	if (local.has(nm)) return local[nm]; }
	if (global.has(nm)) return global[nm];
	*/
	return "";
}
void store_variable(const StTy & nm,const StTy & val)
{ back().syms.store(nm,val); 
	/*
	bool have_stack=(size()!=1);
	SymTy & global=front().syms;
	if (!have_stack) {  global.store(nm,val); return;}
	SymTy & local=back().syms;
	if (local.has(nm)) local.store(nm,val);
	else global.store(nm,val); 
*/
}

template <class Os> void dump(Os& os)
{
	reverse_iterator is=rbegin(),ie=rend();
	while (is!=ie)
	{
		(*is).dump(os);
		++is; 
		}
	}
// pollute higher contexts variables "dest" using local names in "src"
// this isn't a member function of the symbol store because the conversion
// type is application specific, I guess you could pass it as a tempate param...
template <class Is> void pollute( Is& src, Is& dest)//, index_type level)
{
//	if (level>=size()) return; 
	if (size()<2) return;
// 2024
	//reverse_iterator is=rbegin(),ie=rend(),id=rbegin();
	reverse_iterator ie=rend(),id=rbegin();
	index_type siz=src.size(); if (dest.size()<siz) siz=dest.size();
	StTy v="";
	SymTy& locals=back().syms;	
	//while (level--) id++;
	id++;
	while (id!=ie)
	{
		SymTy& keeps=(*id).syms;
		for (index_type i=0; i<siz;++i)
		{ locals.load(src[i],v); keeps.store(dest[i],v); } 
	++id;
	}
}

};


#endif
