#include "data_and_config.h"

char kluge::nl='\n';
	
typedef data_and_config::Config MyClass;

data_and_config::IsTy * data_and_config::open(const name_type & nm)
{ 	
//	if (nm=="-") return &cin; 
	IsTy * x= new std::ifstream(nm.c_str());	
	if (!*x) { std::cout<<"Cant open "<<nm<<"\n"; }	
	return x;	
}	
	bool MyClass::has ( const KeyTy & key, const ValItemTy & val )  const
	{
		debug("checking on key=",key );
		string_type v; // discard anyway...
		return ( parse(key,val,v));
	}
	bool MyClass::has ( const KeyTy & key )  const
	{
		debug("checking on key=",key);
		const_iterator crap=find(key);
		return ( crap!=end());
	}
	const MyClass::ValTy * MyClass::get_vector(const KeyTy & key )  const
	{	// wtf? no const access???? 
		debug("trying to get key=",key);
		const_iterator crap=find(key);
		
		if (crap==end()) { faile("didnt find key=",key); return NULL; }
		const ValTy* d=&(*crap).second;
		return d;
		} 
	const MyClass::ValItemTy & MyClass::get(const KeyTy & key )  const
	{	// wtf? no const access???? 
		//debug("trying to get key=",key);
		//const_iterator crap=find(key);
		
		//if (crap==end()) { faile("didnt find key=",key); return key; }
		const ValTy* d= get_vector(key)    ;//(*crap).second; 
		if (!d) return key; // already flagged above
		debug(" found a value of val=",(*d)[0]);
		return (*d)[0];
//		return ((*this)[key])[0];
		
	}
	// faking invalid return init? wtf? reference is ok ???
	MyClass::value_iterator  MyClass::get_all(const KeyTy & key )  const
	{       const ValTy* d= get_vector(key)    ;
		static value_iterator r_null=value_iterator();;
		if (!d) return r_null; // already flagged above
//		debug(" found a value of val=",(*d)[0]);
		debug(" found a value of val=",*((*d).begin()));
		return (*d).begin();
//		return ((*this)[key])[0];
		
	}
	MyClass::value_iterator  MyClass::get_end(const KeyTy & key )  const
	{       const ValTy* d= get_vector(key)    ;
		static value_iterator r_null=value_iterator();;
		if (!d) return r_null; // already flagged above
//		debug(" found a value of val=",(*d)[0]);
		debug(" found a value of val=",*((*d).begin()));
		return (*d).end();
//		return ((*this)[key])[0];
		
	}
	
	int MyClass::load(int argc, char *argv[], HelpTy * phelp, HelpTy * ahelp )
	{
		if ( argc<2) {  (*phelp)();  return 1; }
		if (strcmp("-help",argv[1])==0) { (*phelp)(); return 2;   }
		if (strcmp("-about",argv[1])==0) { (*ahelp)(); return 3;   }
		load(argc,argv);
		return 0;
	}
	void MyClass::load(int argc, char *argv[])
	{ for (count_type i=0; i<argc; ++i)
		{  name_type key=name_type(argv[i]); 
		 if (key.length()>1) if (key.substr(0,1)=="-") 
		 { if (i<argc-1) { name_type val=name_type(argv[i+1]); 
		 if (val.find("=")!=name_type::npos) 
		 { MyItor f=find(key.substr(1)); 
		 //std::cout<<"looking for key "<<key<<"\n";
		 if(f!=end()) erase(f);} // override 
		  if (val.find("=")==0) {val=val.substr(1);}
		 add(key.substr(1),val);  
		// std::cout<<"Added "<<key<<" "<<val<<"\n";
		 
		 }  }
		 if (key=="-config" )
		 { 
		 	IsTy * is= data_and_config::open(name_type(argv[i+1]));
		 	if (is) {delete & load(*is);} }
		 
		if (key.length()) add(key,key); 
		}	
		
		
	}
	void data_and_config::Config::unload(const name_type & xx)
	{
		ConvStreamTy str(xx);
		KeyTy key="";
		if (str.good())str>>key;
		MyItor ii=find(key);
		if (ii!=end()) {erase(ii); Inc();}
		}
	void data_and_config::Config::load(const name_type & xx)
	{
		name_type x=xx;
		if (x.length()>0){
		if(x.substr(x.length()-1)=="\r" ) x=x.substr(0,x.length()-1); // remove crlf; 
		debug("have a line ",x);
		if (x.length()<1) {}
		else if (x.substr(0,1)=="}") {m_prefix="";}
		else if (x.substr(0,1)=="{") {m_prefix=m_prefix+x.substr(1);}
		else if (x.substr(0,1)!=";")
		{
			
		ConvStreamTy str(x);
		KeyTy key;
		if (str.good())str>>key;
		if (key=="DEBUG" ) {m_debug_mode=true; 
		debug("Enable debug mode, use DEBUG FAIL for error only. At size ",size());
		}	
//		MyItor f=find(key);
//		if ( f==end()) { (*this)[key]= ValTy(); }
//		ValTy& val=(*this)[key];
//		while (!str.eof()) {str >> x; val.push_back(x); }
		while (!str.eof()) { x=""; str>>x; // trailing spaces do a dummy read and 
									// replicate last value ......
		if (x.length()>0) 
		{
			while (x.find(",")!=string_type::npos) { x=x.replace(x.find(","),1," ");}  
			debug("   adding a value of ",x); add(key,x); 
			if (key=="DEBUG") if (x=="FAIL"){  
				debug("Restrict output to failures at ",size() );m_fail_mode=true; }
		}; //if
		} // while 
		add(key,key); 
		}	} else debug("   zero line length found with size=",size());
		
		}
	data_and_config::IsTy &  data_and_config::Config::load(IsTy & is)
	{
		data_and_config::Ch stupid[MM_MAX_LINE_LENGTH];
		name_type x="";
		m_prefix="";
		while (!is.eof())
		{
		//	 is>>x; cout<<x; cout<<flush;	
		is.getline(stupid,MM_MAX_LINE_LENGTH,kluge::nl);
		x=name_type(stupid);
		load(x);
	}
	debug("done loading config with size=",size());
return is;	
	}
void data_and_config::Config::add(const KeyTy & key, const ValItemTy & val) 
{		
	MyItor f=find(key);
	if (key=="DEBUG" ) {m_debug_mode=true;}
	if ( f==end()) { (*this)[key]= ValTy();
	 debug("making entry for ",key);
	 }
	ValTy& valc=(*this)[key];
	 valc.push_back(val); 
	 debug("added value ",val); 
	 Inc(); 
		}
		
	//}
	data_and_config::input_type & data_and_config::Config::script_source()
	{
		if (m_script_source) {return *m_script_source;}
		// Else try to open one...
		if (has("SCRIPT"))
		{
			
			
			}
		return *m_script_source;
		}
	

