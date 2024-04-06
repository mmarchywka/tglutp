#include "../base/basic_command_shell.h"
#include "../base/basic_expression.h"

#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <cctype>


typedef basic_command_shell MyClass;

MyClass::StTy MyClass::Evaluate(StTy & in)
{
	StTy subs=JustSubstitute(in);
	basic_expression ex;
	return ex.parse(subs);
	return in;
}
MyClass::StTy MyClass::Subscript(StTy & xx, const StTy & sub)
{
	std::istringstream*  ss= new std::istringstream
		(remove_formatting(xx));
		// mmdd::dout(MMDDLOC)<<"\n";
	index_type idx=atoi(sub.c_str());
	StTy rv="";
	(*ss)>>rv;
	while (idx>0) {(*ss)>>rv; --idx;
	// mmdd::dout(MMDDLOC)<<"\n";
	}
	delete ss;	
	return rv;
	}
MyClass::StTy MyClass::JustSubstitute(StTy & in)
{
	StTy rv="";

// 2024 wtf this is too small?
	//index_type i=in.find("$");
	std::size_t i=in.find("$");
	while (i!=StTy::npos)
	{
MM_ERR(MMPR3(__FUNCTION__,i,StTy::npos))
		rv+=in.substr(0,i);
		in=in.substr(i+1);
		index_type idx=0; 
		while ((in.length()>idx)&&(isalnum(in.c_str()[idx])!=0)) ++idx; 
		//rv+=FindVariable(in.substr(0,idx)); 
		StTy xx=FindVariable(in.substr(0,idx));
		if (in.length()>idx) if (in.substr(idx,1)=="[")
		{  //  mmdd::dout(MMDDLOC)<<"\n";
			//index_type ket=in.find("]");
			std::size_t ket=in.find("]");
			if ((ket!=StTy::npos)&&(ket>idx+1)) 
			{// mmdd::dout(MMDDLOC)<<"\n";
				StTy temp=in.substr(idx+1,ket-idx-1);
				xx=Subscript(xx,JustSubstitute( temp));	
			//mmdd::dout(MMDDLOC)<<xx<<"\n";
		idx=ket+1; // to get rid of stuff... 
		}	
		}
		rv+=xx;
		//mmdd::dout(MMDDLOC)<<rv<<"\n";
		if ( idx<in.length()) in=in.substr(idx);	 else in="";
		i=in.find("$");
	}
	rv+=in;
	return rv;
	}
	
MyClass::StTy MyClass::tok(StTy& in)
{
	StTy rv="";	
	index_type le=in.length();
	if (le==0) return rv;
	const char * inx=in.c_str();
	//Ch c=inx[0];
	//ExtCh cx= ExtCh(c);
	index_type idx=0,iss=0;
	while (isspace(ExtCh(inx[iss++]))!=0) ; iss--;
	if (inx[iss]=='$') {  in=(le>(1+iss))?in.substr(iss+1):""; return"$";}
	if (inx[iss]=='=') {  in=(le>(1+iss))?in.substr(iss+1):""; return"=";}
	if (inx[iss]=='\"') 
	{ idx=iss+1; 
		while ( (ExtCh(inx[idx])!='\"')&&(ExtCh(inx[idx])!=0)) ++idx;;
		--idx; ++iss; // "" -> iss=idx+1
		// equal means ther is only one char... 
		if ( idx>=iss) {	rv=in.substr(iss,idx-iss+1); } else {rv="";}
		++idx;  // point back to trailing quote or zed
		++idx; // remove the trailing quote or "0"
		if (idx<le ) in=in.substr(idx);  else in=""; 
		return rv;   
	}
	if (inx[iss]==ExtCh(0)) {in=""; return "";  }
	idx=iss;
	// now we need to parse file names, so I wanted an underscore. 
	// however, I think the better thing is to add quote capability...
	// underscore can be fixed but what about "/" ? 
	// I don't remember why this doesn't need a length test either but it
	// seems to work :) isalnum fails at zero I guess.. 
	//	while (((isalnum(ExtCh(inx[idx])))!=0)||(ExtCh(inx[idx])=='.')) ++idx;;
	while (((isalnum(ExtCh(inx[idx])))!=0)||
		(ExtCh(inx[idx])=='_')||(ExtCh(inx[idx])=='.')) ++idx;;
	if (idx!=iss) 
	{ 
		rv=in.substr(iss,idx-iss); 
		if (idx<le ) in=in.substr(idx); 
		else in=""; 
		return rv;    
	}
	return rv;
}
// assignment, conditional, expression,parameter
void MyClass::BuildSub( StTy & in)
{	
	// I think a cr is slipping through but I have other things to fix...
	MethodClass & mc=m_methods[m_si.sub];
	if (in.find(":")!=StTy::npos)
	if (in.find(":")>=(in.length()-2))
		if (in.find(" ")==StTy::npos)
		{
			StTy label=in.substr(0,in.find(":"));
			mc.add_label(label);
			return;	
		}
	mc.code.push_back(in);
	
}
MyClass::StTy MyClass::Expand( StTy & in)
{ 
	// This would save some garbage code later...
	if ( in.length()==0) return "";
	// need traits or somethign...
	if (in.substr(0,1)=="#") return "";
	
	if (m_si.state==1) // pushing a subrotine...
	{
		BuildSub(in);
	//	mmdd::dout(MMDDLOC)<<"making " 
	//		<<m_si.sub<<" "<<m_subs[m_si.sub].size()<<"\n";
		StTy to=tok(in);
		if (to=="$") {to=tok(in); if (to=="end") m_si.state=0;};
		return "";
	}
	m_main.code[0]=in;
	StTy in_o=in;
	StTy to=tok(in);
	if (to!="$")
	{ 	in=to+" "+in; return JustSubstitute(in);}

	to=tok(in);
	// Right now this has to be a simple command
	// These things are supposed to return partial parses
	// but this decisions tree needs to be re-thought...
	// 
	if (to=="dump") {return Dump(in);} 
	if (to=="let") { return Let(in);}
	if (to=="readln") { return ReadSome(in);}
	if (to=="clear") { return Clear(in);}
	if (to=="export") { return Pollute(in);}
	if (to=="stream") { return Stream(in);}
	if (to=="sub") {return Sub(in); }
	if (to=="call") {return Call(in); }
	if (to=="if") {return Cond(in); }
	if (to=="end") {return End(in); }
	if (to=="echo") {return Echo(in); }
	return in; 
}
MyClass::StTy MyClass::Clear(StTy & in)
{
	StTy to=tok(in);
	if (to.length()==0) m_stack.back().syms.clear();
	else m_stack.back().syms.erase(to);
	return ""; 
	}
MyClass::StTy MyClass::Cond(StTy & in)
{	// get an expression, nonzero jump
	StTy lbl=tok(in);
	StTy ex=JustSubstitute(in);
	StTy val=Evaluate(ex);
	if (val=="0") return "";
	if (m_stack.jump(lbl)) {} // it worked
	else {} // failed
	return ""; } // assignment 


MyClass::StTy MyClass::Pollute(StTy & in)
{
	std::vector<StTy> sd;
	while (in.length()!=0) 
	{StTy x=tok(in); if (x.length()!=0) sd.push_back(x); }
	m_stack.pollute(sd,sd);
	return "";
	}

void MyClass::ReturnLocalValues()
{
	StTy signature=m_stack.back().method->code[0];
	//index_type rp=signature.find(";");
	std::size_t rp=signature.find(";");
	if (rp==StTy::npos){m_stack.pop_back(); return;  }
	signature=signature.substr(rp+1);
	SymTy locals=m_stack.back().syms;
	m_stack.pop_back();	
	SymTy& keeps=m_stack.back().syms;	
	// invoked from global space will be a problem.. 
	StTy in=m_stack.back().method->code[m_stack.back().pc-1]; 
	rp=in.find(";");
	if (rp==StTy::npos){ return;  }
	in=in.substr(rp+1);
	while ((signature.length()!=0)&&(in.length()!=0)) 
	{ 	mmdd::dout(MMDDLOC)<<"returning " <<signature <<"-"<<in<<"\n";
		StTy k=tok(signature), v=tok(in);
		if (k.length()>0) {
		StTy vv="";
		locals.load(k,vv);
		mmdd::dout(MMDDLOC)<<"addingt " <<k <<"-"<<v<<"..."<<vv<<"\n";
		keeps.store(v,vv); 
		}
	}
	
}

MyClass::StTy  MyClass::Step()
{	// never return from global...
	if ( m_stack.at_global()) return "";
	if (m_stack.returning())
	{ 
//		mmdd::dout(MMDDLOC)<<"returning\n"; 
		// This should nest ok now, a little awkward but ok. 
		ReturnLocalValues();	
	//	mmdd::dout(MMDDLOC)<<"locals updated\n"; 
		if (m_param_stream_stack.size()!=0)
		{ //mmdd::dout(MMDDLOC)<<"streaming\n"; 
		 	if (m_param_stream_stack.top().level==m_stack.size()) 
				return StreamAgain();	
		}
		if (m_stack.at_global()) return "";
	}
//	mmdd::dout(MMDDLOC)<<"Have sig "<<ci.sub[0]<<" at "<<ci.pc<<" of "<<ci.sub[ci.pc]<<"\n";
	string_type x=m_stack.next(); 	
	
//	m_stack.dump(std::cout);
//	std::cout<<"executing :"<<x<<"\n"; 
	return x;
}
void MyClass::Call(const StTy & nm, SymTy & params)
{
	if ( !m_methods.has(nm)) 
		mmdd::dout(MMDDLOC)<<"call missing "<<nm<<"\n";
//	mmdd::dout(MMDDLOC)<<"calling "<<nm<<"\n";
	m_stack.call(nm,&m_methods[nm],params);
	m_stack.dump(std::cout);

}
MyClass::StTy MyClass::Echo(StTy & in)
{	std::cout<<Evaluate(in)<<"\n"; return ""; 	}

////////////////////////////////////////////////////////////////////////////
MyClass::StTy MyClass::Dump(StTy & in)
{
	std::cout<<"CALL STACK:\n";
	m_stack.dump(std::cout);
	std::cout<<"GLOBALS:\n";
	m_stack.front().syms.dump(std::cout);
//	std::cout<<"call stak depth is "<<m_stack.size()<<"\n"; 
	std::cout<<"LOCALS:\n";
	if (m_stack.size()!=1) m_stack.back().syms.dump(std::cout);
	std::cout<<"METHODS:\n";
	m_methods.dump(std::cout); 	
	return ""; } // dump context info somewhere

MyClass::StTy MyClass::Let(StTy & in)
{	StTy dest=tok(in);
	StTy val=Evaluate(in);
	StoreVariable(dest,val); 
	return ""; } // assignment 

MyClass::StTy MyClass::ReadSome(StTy & in)
{	// Syntax: readln fn d1 d2 d3 d4 d5
	// read as many of the listed variables as possible 
	StTy fn=tok(in);
//	mmdd::dout(MMDDLOC)<<" now parse "<<in<<'\n'; 
	// it is important to echo this as our parser may not be compatible
	// with file names. It brokes at underscore, probably others... 
	mmdd::dout(MMDDLOC)<<" try to  open "<<fn.c_str()<<" for input \n"; 
	std::istream * is = new std::ifstream(fn.c_str());
	if (!*is)
	{ mmdd::dout(MMDDLOC)<<" can't open "<<fn.c_str()<<" for input \n"; 
		delete is;
		return ""; 
	}
	StTy val="";
	StTy var=tok(in);
//	mmdd::dout(MMDDLOC)<<" now parse "<<in<<'\n';
	while (var!="")
	{
	//	mmdd::dout(MMDDLOC)<<" now parse "<<in<<'\n';
		(*is)>>val;
		mmdd::dout(MMDDLOC)<<" read "<<val<<" to store at "<<var<<'\n'; 
		if (!*is) break; 
		StoreVariable(var,val); 
		var=tok(in);
	}
	delete is;
	// at this point, I think we should return the patrially parsed
	// "in" so someone else could deal with it if command didn't read all. 
	return "";
};
MyClass::StTy MyClass::Sub(StTy & in)
{StTy to=tok(in); // name, leaving params on line
	m_si.sub=to;
	m_methods.erase(to);
	m_methods[to].code.push_back(in);
	m_si.state=1;
	return ""; }  // begin subroutine
MyClass::StTy MyClass::End(StTy & in)
{ return ""; }  // end subroutine definition
MyClass::StTy MyClass::Call(StTy & in)
{	StTy to=tok(in);
//	mmdd::dout(MMDDLOC)<<"calling" <<to<<"\n";
	if (!m_methods.has(to))
	{     mmdd::dout(MMDDLOC)<<"nofind" <<to<<"\n";   return "";}
	StTy signature=m_methods[to].code[0];
	index_type rp=signature.find(";");
	if (rp!=StTy::npos) signature=signature.substr(0,rp);
//	mmdd::dout(MMDDLOC)<<"sigs is " <<signature<<"\n";
	SymTy params=SymTy();
	while ((signature.length()!=0)&&(in.length()!=0)) 
	{ 	mmdd::dout(MMDDLOC)<<"adding " <<signature <<"-"<<in<<"\n";
		StTy k=tok(signature), v=tok(in);
		if (k.length()>0) {
		StTy vv=Evaluate(v);
		mmdd::dout(MMDDLOC)<<"addingt " <<k <<"-"<<v<<"..."<<vv<<"\n";
		params.store(k,vv); 
		}
		}
	Call(to,params);
	m_callback=true; 
	return ""; } // call 
	
	MyClass::StTy MyClass::remove_formatting(const StTy & v)
{
	index_type c=v.length(),i=0;;
	char buf[c+1];
	char const *  p=v.c_str();
	while (*p!=(char)0) {if ((*p!='{')&&(*p!='}')&&(*p!=',')) buf[i++]=*p; ++p;  }
	buf[i]=0;
	return StTy(buf);
}
	
bool MyClass::Stream(StTy & name, istream_type * ss)
{
	// stream variable, subroutine, param names 
	if (!m_methods.has(name))
	{     mmdd::dout(MMDDLOC)<<"nofind" <<name<<"\n";   return false;}
	StTy signature=m_methods[name].code[0];
	index_type rp=signature.find(";");
	if (rp!=StTy::npos) signature=signature.substr(0,rp);
//	mmdd::dout(MMDDLOC)<<"sigs is " <<signature<<"\n";
	SymTy params=SymTy();
	if (!ss->good()||(*ss).eof())  return false;
	while ((signature.length()!=0)&&(ss->good())) 
	{ 	
		float x=0; (*ss)>>x;
		//mmdd::dout(MMDDLOC)<<"adding " <<signature <<"-"<<x<<"\n";
		StTy k=tok(signature);
		if (k.length()>0) {
			std::ostringstream os; os<<x;
		//mmdd::dout(MMDDLOC)<<"addingt " <<k <<"-"<<os.str()<<"\n";
		params.store(k,os.str()); 
		}
	}
//	mmdd::dout(MMDDLOC)<<"stream calll\n"; 
	Call(name,params);
	return true; 
}
MyClass::StTy MyClass::Stream(StTy & in)
{ // stream variable, subroutine, param na
	StTy to=tok(in);
	std::istringstream*  ss= new std::istringstream
		(remove_formatting(FindVariable(tok(in))));
	if (Stream(to,ss))
	m_param_stream_stack.push(stream_def(ss,to,m_stack.size()-1));
	m_callback=true; 
	return "";
}
MyClass::StTy MyClass::StreamAgain()
{	
	stream_def & c=m_param_stream_stack.top();
	if (!Stream(c.nm,c.ss))
	{	delete c.ss;   m_param_stream_stack.pop();return "";}
	m_callback=true;  // probably redundant///
	return Step(); 
}
////////////////////////////////////////////////////////////////////////////
MyClass::StTy  MyClass::FindVariable(const StTy & nm)
{ return m_stack.find_variable(nm);}
void MyClass::StoreVariable(const StTy & nm,const StTy & val)
{ m_stack.store_variable(nm,val); }




