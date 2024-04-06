#ifndef GLUT_PICKING_H
#define GLUT_PICKING_H

#include "../view/camera_and_lights.h"

#include <iostream>
#include <queue>
#include <map>
template <int SZ> class glut_picking
{
enum {BUFSIZE=SZ};

typedef GLuint gl_count_type;
// not thread safe
static GLuint * bu()
{static GLuint selectBuf[BUFSIZE];
 return selectBuf;
}

public:
typedef pair<gl_count_type,gl_count_type> obj_name_type;
typedef vector<obj_name_type> contain_type;

class zed_cmp{
// 2008-03-18 not sure what this was? 
// typedef glut_picking::obj_name_type acc_type;
typedef obj_name_type acc_type;
public: bool operator()(const acc_type & x, const acc_type & y) const
	{	return (x.second)>(y.second); }};

typedef priority_queue<const obj_name_type,contain_type, zed_cmp> QTy;

gl_count_type size() const { return m_que.size(); }
gl_count_type nearest() const { return m_que.top().first; }
gl_count_type zed() const { return m_que.top().second; }

gl_count_type pop() { if (size()) m_que.pop(); return size(); }

void startPicking(int cursorX, int cursorY) {
	glSelectBuffer(BUFSIZE,bu());
	r_cam.set_pick(cursorX,cursorY);
}

void process_hits (GLint hits, GLuint buffer[])
{
gl_count_type *	ptr = (GLuint *) buffer;
   for (int i = 0; i < hits; i++) {
   	gl_count_type * names=ptr+3; gl_count_type n=(*ptr);
   	while (n--){ m_que.push(obj_name_type(*(names++),*(ptr+1))); }
   ptr+=*ptr+3;
}
	}
void stopPicking() {

	int hits;
	
	// restoring the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	hits = glRenderMode(GL_RENDER);
	if (hits != 0) process_hits(hits,bu());
}


QTy m_que;
};

#endif


