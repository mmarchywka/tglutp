#ifndef STRING_STACK_H__
#define STRING_STACK_H__

#include <vector>
#include <string>


template < class  DataTy > class string_stack : public std::vector<DataTy>
	{	
		typedef DataTy string_type;
		typedef string_type item_type;
		typedef std::vector<string_type> Super;
		typedef string_stack Myt;
		typedef unsigned int index_type;
		public: 
		
		string_stack():m_cmd_ptr(0),m_size_max(0),m_item("") {}
		string_stack(const index_type lim):m_cmd_ptr(0),m_size_max(lim),m_item("") {}
		void push(const item_type& i) { Push(i); }
		void pull( item_type* i) { Pull(i); }
		void get(item_type & cmd) {Get(cmd);}
		void earlier(  const index_type i){   Earlier(i);  }
		void later(  const index_type i) {  Later(i);   }
		// The current item is possible not complete, save for traversal reversal
		void earlier(  const index_type i,const item_type & it){   Earlier(i,it);  }
		void later(  const index_type i,item_type & it) {  Later(i,it);   }
		 template <class Os> void dump(Os& os) //const 
		 {
		 for (index_type i=0; i<size(); ++i)
		 	os<<((m_cmd_ptr==i)?"*":" ")<<((*this)[i])<<"\n";
		 	
		 	
		 }
		 index_type size() const { return Super::size(); } 
		private:
		void Push(const item_type& i)
		{ Super::push_back(i); 
		if (m_size_max!=0) if (size()>m_size_max)  { Super::erase(Super::begin()); }
		m_cmd_ptr=size(); }
		void Pull( item_type* i)
		{  
		if (size()>0)  { *i=(*this)[0]; Super::erase(Super::begin()); }
		m_cmd_ptr=size(); }
		
		void Get(item_type & cmd) {if (m_cmd_ptr<size()) cmd=(*this)[m_cmd_ptr]; 	}
		void Earlier(  const index_type i){   if (i<=m_cmd_ptr)  m_cmd_ptr-=i; else m_cmd_ptr=0;  }
		void Later(  const index_type i) {  m_cmd_ptr+=i; if ( m_cmd_ptr>size()) m_cmd_ptr=size();  }
		
		void Earlier(  const index_type i,const item_type & it)
		{   if (m_cmd_ptr==size()) m_item=it;Earlier(i); }
		// if these are unsigned, you want to use "+" not "-" where possible
		void Later(  const index_type i, item_type & it) 
		{   if ( (m_cmd_ptr+1)==size()) it=m_item; Later(i); }
		
		
		index_type m_cmd_ptr,m_size_max;
		item_type m_item;
	

};
#endif

