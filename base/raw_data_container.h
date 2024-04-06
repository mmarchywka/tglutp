#ifndef RAW_DATA_CONTAINER_H__
#define RAW_DATA_CONTAINER_H__

#include <iostream>

	
tempalte < unsigned int COORD=3, class DataTy=float> class point_list_container
{
	typedef point_list_container Myt;
public:
typedef int count_type;
typedef DataTy data_type;
typedef data_type* data_ptr;
enum {COORDS=COORD};
point_list_container(): m_size(0),m_limit(0),m_ptr(0),m_cur(0){}
point_list_container(const count_type c)
: m_size(0),m_limit(c),m_ptr(new data_type[m_limit*COORDS]),m_cur(m_ptr)
{
if (!m_ptr) { cout<<"Unable to allocate point_list_container size "<<c<<" didn't work \n"; }	
}
~point_list_container() { delete[] m_ptr; }
count_type size() const { return m_size; } 
void push_back(const pdb_atom & p)
{
	if (m_size>=m_limit)
	{
	// assume memory is around, peak usage huge...
	count_type old=m_limit*COORDS;
	m_limit=(m_limit<<1)+(1<<14);
	//cout<<"Try to allocate at "<<m_limit<<"\n"; cout.flush(); 
	data_ptr ptr= new data_type[m_limit*COORDS];
	if (ptr) { 
		//cout<<"asdfasfasfasdfs\n"; cout.flush();
		for (count_type i=0; i<old; ++i) {*(ptr++)=*(m_ptr++); }
		m_cur=ptr; m_ptr-=old;
		delete [] m_ptr; m_ptr=ptr-old; 
	} 
	else cout<<"Unable to expand buffer.. expect a core dump soon\n";
	}
	
	m_size++;
	*(m_cur++)=p.x();*(m_cur++)=p.y();*(m_cur++)=p.z();	
}	
GLuint MY_GL_LIST;
void glcompile(GLuint MY_GL_LIST=1)
{
glNewList(MY_GL_LIST,GL_COMPILE);	
data_ptr ptr=m_ptr;
glBegin(GL_POINTS);
glColor3f(1.0f,1.0f,1.0f); 
while (ptr<m_cur)
{
	// commute? 
	glVertex3f(*(ptr++),*(ptr++),*(ptr++));
	}
	glEnd();
	glEndList();
	
}
void gldraw() {  glCallList(MY_GL_LIST); }
data_type get(count_type idx,count_type coord) { return *(m_ptr+idx*COORDS+coord); }
private:
	count_type m_size, m_limit; 
        data_ptr m_ptr,m_cur;	


	
};


#endif

