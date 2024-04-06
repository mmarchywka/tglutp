#ifndef HYBRID_EXPRESSION_H__
#define HYBRID_EXPRESSION_H__

#include "global.h"

#include <stack>
#include <string>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <cctype>
#include <iostream>
#include <sstream>

class hybrid_expression
{
	typedef hybrid_expression Myt;
	typedef char Ch;
	typedef int ExCh;
	typedef std::basic_string<Ch> string_type;
//	typedef Ch OperatorTy;
	typedef string_type OperandTy;
	typedef string_type OperatorTy;
	typedef std::stack<OperatorTy> OrStack;
	typedef std::stack<OperandTy> OdStack;
	typedef int index_type;
public:	
	
	basic_expression (): m_or(), m_od() {}
	
	string_type  parse(const string_type & ex)
	{
	//	char ops[]={'+','-','>','<','=',"*","/","%"};		
		string_type ops="<>=%+-/*(|&";
		index_type prec[]={10,10,10,11,20,20,100,100,-1000,20,100};
	//	string_type op2[]={">=","<=","==","!="};
		const char * ch=ex.c_str();
		const index_type len=ex.length();
		index_type p=0;
		bool check_unary=true,is_unary=false;
		while (p<len)
		{	Ch c=ch[p];
			ExCh e=ExCh(c);
		 	if (isspace(e)!=0) {++p; continue; }
			// variable should have been evaluated already but heck
			if ( isalpha(e)!=0) 
			{ 	index_type p2=p+1;
				while ((isalnum(ExCh(ch[p2]))!=0)&&(p2<len))  {p2++;}
				string_type operand=ex.substr(p,p2-p);
				m_od.push(operand); check_unary=false; p=p2; continue;
			} // no unaries but "." ok
			// ellipsis is a number :)
			if ( (isdigit(e)!=0)||(c=='.')) 
			{ 	index_type p2=p+1;
				while (((isdigit(ExCh(ch[p2]))!=0)||(ch[p2]=='.'))&&(p2<len))  {p2++;}
				string_type operand=ex.substr(p,p2-p);
				// could have had a unary in front...
				m_od.push(operand); p=p2;check_unary=false;
				if (is_unary) Apply(); 
				 continue;
			}
			if (c==')') 
				{ m_or.push(string_type(")")); Apply(); ++p; check_unary=false;continue; }
			
			if (c=='(') { m_or.push(string_type("(")); ++p;check_unary=true; continue; }
			
			// ignoring unaries, asume single char operators for now... 
	//		index_type 
			string_type::size_type loc=ops.find(c);
			if (loc!=string_type::npos)
			{ 
				if (check_unary){
					 is_unary=true; 
				if (c=='-') { m_or.push(string_type("-u"));}
				else if (c=='!') { m_or.push(string_type("!"));}
				else m_or.push(ex.substr(p,1)); ++p; continue;
				}
				else is_unary=false;
				
				if (m_or.size()==0) 
				{ m_or.push(ex.substr(p,1)); ++p;check_unary=true; continue;}
					index_type lold=ops.find(m_or.top().c_str()[0]);
					index_type pr=prec[loc]-prec[lold];
				if (pr<=0) { Apply ();} 
				m_or.push(ex.substr(p,1)); ++p;check_unary=true; continue;  
			
			}
			mmdd::dout(MMDDLOC)<<"dont know what to do with "<<c<<" at "<<p<<"\n";
			++p;
		}
		while (m_or.size()!=0) 
		{	if (m_od.size()<1) 
				mmdd::dout(MMDDLOC)<< " operand stack is wrong\n";
			Apply();
		}
		if (m_od.size()!=1)
		{ mmdd::dout(MMDDLOC)<<" expression stack is wrong\n";
			return "0"; 
			}
		string_type answer= m_od.top();
		if (m_od.size()!=1) mmdd::dout(MMDDLOC)<<" expression stack is wrong\n";
		return answer;
		}
	//http://www.cs.utexas.edu/~lavender/courses/ee360c/lectures/lecture-24.pdf
private:	
	
	void Apply()
	{	index_type nest=0;
		string_type fss="";
		fss=m_or.top(); m_or.pop();
		if (fss==")") {
			if ( m_od.size()<1) 
		{ std::cout<<"Operand stack too small for "<<fss<<"\n"; return; }
			
			fss=m_or.top(); m_or.pop(); ++nest;}
		
		if ( m_od.size()<1) 
		{ std::cout<<"Operand stack too small for "<<fss<<"\n"; return; }
		string_type od2=m_od.top(); m_od.pop();
		if (fss=="-u")
		{ if (od2.substr(0,1)!="-") m_od.push(string_type("-")+od2);
			else m_od.push(od2.substr(1)); 
		}
		else if (fss=="(")
		{   m_od.push(od2); nest=0;  }
		else if (fss=="!")
		{      m_od.push((od2=="0")?"0":"1"); } 
		else {  // binary
		if ( m_od.size()<1) 
		{ std::cout<<"Operand stack too small for "<<fss<<"\n"; return; }
		string_type od1=m_od.top(); m_od.pop();
		typedef float sloTy;
		sloTy sd2=atof(od2.c_str());
		sloTy sd1=atof(od1.c_str());
		
		std::ostringstream p;
		
		if (fss=="+" ) p<<(sd1+sd2);
		else  if (fss=="-" ) p<<(sd1-sd2);
		else  if (fss=="*" ) p<<(sd1*sd2);
		else  if (fss=="/" ) p<<(sd1/sd2);
		else  if (fss=="%" ) p<<(int(sd1)%int(sd2));
		else  if (fss=="=" ) p<<((sd1==sd2)?1:0);
		else  if (fss==">" ) p<<((sd1>sd2)?1:0);
		else  if (fss=="<" ) p<<((sd1<sd2)?1:0);
		else  if (fss=="|" ) p<<(((sd1!=0)||(sd2!=0))?1:0);
		else  if (fss=="&" ) p<<(((sd1!=0)&&(sd2!=0))?1:0);
		else {mmdd::dout(MMDDLOC)<<"Unknown operator "<<fss<<".\n"; }
		m_od.push(p.str());
		} // unary
		
		
		if (nest!=0) {fss=m_or.top(); m_or.pop();
			if (fss!="(" ) { mmdd::dout(MMDDLOC)<<" missing left paren \n"; }
		}
		
		}
	
	OrStack m_or;
	OdStack m_od;
	
};


#endif
