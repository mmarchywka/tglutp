#include "camera_and_lights.h"
#include "../view/glut_commons.h"

typedef camera_and_lights ClassTy;



void ClassTy::set_scene_center( const CenterTy & c)
{
	 myglutil::lights_off(7);
	 Point3 cp=getPosition();
	 CenterTy cpp=CenterTy(int(cp.x()),int(cp.y()),int(cp.z()));
	 int xx=c.x()-m_xlate_sum.x()+cpp.x();
	 int yy=c.y()-m_xlate_sum.y()+cpp.y();
	 int zz=c.z()-m_xlate_sum.z()+cpp.z();
	std::cout<< "setting to "<<xx<<" "<<yy<<" "<<zz<<"\n";
//	 glTranslatef(c.x()-m_xlate_sum.x(),c.y()-m_xlate_sum.y(),c.z()-m_xlate_sum.z());
	glTranslatef(xx,yy,zz);
	 m_xlate_sum=c+cpp; 
	myglutil::lights_on(7);
};

void ClassTy::rotate_scene(const coord_type d1, const PointTy & n1,
const coord_type d2, const PointTy & n2) 
{
// std::cout <<"rotate x\r"; 
      myglutil::lights_off(7); 
  //    glPushMatrix();
      glTranslatef(m_rot_center.x(),m_rot_center.y(),m_rot_center.z());
      if (d1!=0) n1.glRotatef(d1);if (d2!=0) n2.glRotatef(d2);
      glTranslatef(-m_rot_center.x(),-m_rot_center.y(),-m_rot_center.z());
//	glPopMatrix();
 	myglutil::lights_on(7);
}

void ClassTy::fix_light_position()
{
Point3 lightPositionx =lightPosition(); 
  	m_light=lightPositionx;
	float xa[]={0,0,0,0};
 	xa[0]=lightPositionx.x(); xa[1]=lightPositionx.y();
    xa[2]=lightPositionx.z(); xa[3]=0; 
  	std::cout <<"rotate light "<<m_rot_phi<<" "<<m_rot_theta<<" "<<xa[0]<<" "<<xa[1]
  	<<" "<<xa[2]<<"\n"; 
    glPushMatrix();
    glLoadIdentity();
    const PointTy n1= PointTy(0,1,0);
 	const PointTy n2=PointTy(1,0,0);
    if (m_rot_phi!=0) n1.glRotatef(m_rot_phi);
    if (m_rot_theta!=0) n2.glRotatef(m_rot_theta);
  	glLightfv( GL_LIGHT0, GL_POSITION, xa );
    glDisable( GL_LIGHT0 );   glDisable( GL_LIGHTING );
    glPopMatrix();
//    if (m_rot_theta!=0) n2.glRotatef(-m_rot_theta);
// 	if (m_rot_phi!=0) n1.glRotatef(-m_rot_phi);
  	
  	glEnable( GL_LIGHT0 ); glEnable( GL_LIGHTING );
	
	}

void ClassTy::rotate_light(const coord_type _d1, const PointTy & _n1,
const coord_type _d2, const PointTy & _n2) 
{
 	if ( _d1==0) if (_d2==0) return; 
// 2024
// 	const PointTy n1= PointTy(0,1,0);
// 	const PointTy n2=PointTy(1,0,0);
 	coord_type d1=0; coord_type d2=0;
 	if (_n1.x()!=0) d1=_d1; if (_n1.y()) d2=_d1;
 	if (_n2.x()) d1=_d2; if (_n2.y()) d2=_d2; 
 
  	glEnable( GL_LIGHT0 );   glEnable( GL_LIGHTING );
  	
  	m_rot_phi+=d1*10;m_rot_theta+=d2*10;
  fix_light_position();
//  Matrix m;m.setToRotation(d1*3.14/180,Vector3(0,0,1));
  	

}
void ClassTy::set_slab(const coord_type near, const coord_type far)
{ m_near=near; m_far=far; }
void ClassTy::set_camera()
{
	if (!m_old_new)	return;
 glViewport(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
 
 glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
gluPerspective(80*m_zoom,float(m_viewport[2]/m_viewport[3]),m_near,m_far);	

glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

gluLookAt(m_eye.x(),m_eye.y(),m_eye.z(), 
	m_look.x(),m_look.y(), m_look.x(),
	m_up.x(),m_up.y(), m_up.z());
	
	 GLfloat xyz[4];
	 xyz[0]=m_light.x(); xyz[1]=m_light.y(); xyz[2]=m_light.z(); xyz[3]=1.0f; 
glLightfv( GL_LIGHT0, GL_POSITION, xyz );
	
	
	
	GLdouble cx=-1280,cy=1280,diam=300*m_zoom;
	GLdouble left = cx - diam; 
	GLdouble right = cx + diam; 
	GLdouble bottom =cy - diam ;
	GLdouble top = cy + diam; 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(left, right, bottom, top, m_near, m_far); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 

	
	
}

ClassTy::count_type ClassTy::width()const 
{ return camera->getViewportWidthInPixels(); }
ClassTy::count_type ClassTy::height()const 
{ return camera->getViewportHeightInPixels(); }



void ClassTy::near_plane(coord_type z)
{ m_near=z; set_camera(); }
void ClassTy::move_near_plane( coord_type dz)
{ 
if (!m_old_new)	camera->clip_up(camera->get(0)+dz);
	m_near+=dz; set_camera();
}
void ClassTy::far_plane(coord_type z)
{ m_far=z; set_camera(); 
if (!m_old_new)	camera->clip_down(z);
}
void ClassTy::move_far_plane( coord_type dz)
{ 
if (!m_old_new)	camera->clip_down(camera->get(1)+dz);
	m_far+=dz; set_camera();
}
//################################################################

Point3 ClassTy::getTarget() {  if (!m_old_new)	return camera->getTarget(); 
return Point3(m_look); 
} 
void ClassTy::transform(int x, int y, int w, int h)
{
	if (!m_old_new)camera->transform(x,y,w,h); 
	}
void ClassTy::transform () { 
	if (!m_old_new)camera->transform(); 
	
	set_camera();
	}
Point3 ClassTy::getPosition() {
	if (!m_old_new) return camera->getPosition(); 
	return Point3(m_eye); 
	}
void ClassTy::resizeViewport( const count_type w, const count_type h) {
	ResizeViewport(w,h);
	if (!m_old_new)camera->resizeViewport(w,h); 
}
 
float ClassTy::convertLength(const Point3& z, double f) { 
	if (!m_old_new)return camera->convertLength(z,f); 
	return 1;
	}
Ray ClassTy::computeRay(int x, int y) { if (!m_old_new)return camera->computeRay(x,y);
 return Ray();
 }
void ClassTy::dollyCameraForward(float d, bool b) { 
	if (!m_old_new) camera->dollyCameraForward(d,b); 
	}
float ClassTy::get(int x) {if (!m_old_new) return camera->get(x); 
return 1; 
} 
void ClassTy::clip_up(float x) { if (!m_old_new)camera->clip_up(x); 
near_plane(x);
}
void ClassTy::reset() {if (!m_old_new) camera->reset(); }
void ClassTy::move_something(int thing, int delta_x, int delta_y)
{
	if (!m_old_new)
	{
	switch (thing)
	{
	case 0: {camera->translateSceneRightAndUp( delta_x, delta_y ) ;break; }
	case 1: {camera->rollCameraRight( delta_x ) ;break; }
	case 2: {camera->pitchCameraUp( delta_y/10 ) ;camera->yawCameraRight( delta_x/10 );break; }
	case 3: {camera->translateSceneRightAndUp( delta_x, delta_y ) ;break; }
	case 4: { camera->zoomIn(delta_x); break; } 
	}
	} //if
	else
{ switch (thing)
	{
	case 0: {XlateSceneRightAndUp( delta_x, delta_y ) ;break; }
	case 1: {XlateCameraRightAndUp( delta_x,delta_y ) ;break; }
	case 2: {PitchAndYawCamera( delta_y/10,delta_x/10 );break; }
	case 3: {XlateSceneRightAndUp( delta_x, delta_y ) ;break; }
	case 4: { Zoom(delta_x); break; } 
}
set_camera(); 
	} //else
	}
//#################################################################

Point3 ClassTy::lightPosition()
{ if (!m_old_new)
return camera->getPosition()+ ( camera->getPosition() - camera->getTarget() );
return Point3(m_light);
}//

void ClassTy::XlateSceneRightAndUp( const coord_type dx, const coord_type dy )
{}
void ClassTy::XlateCameraRightAndUp(const coord_type dx,const coord_type dy )
{}
void ClassTy::PitchAndYawCamera( const coord_type dx, const coord_type dy )
{}
void ClassTy::Zoom(const coord_type dx )
	{
		coord_type ddf=dx;
		if (ddf<0) ddf=1.0f/ddf;
		m_zoom=ddf;
		//m_eye/=m_eye.length()*ddf;
		
		//static float zoomFactor;
		 /* Global, if you want. Modified by user input. Initially 1.0 */ 
		 /* A routine for setting the projection matrix. 
		 May be called from a resize event handler in a typical application.
		  Takes integer width and height dimensions of the drawing area. 
		  Creates a projection matrix with correct aspect ratio and zoom factor. */ 
		  //void setProjectionMatrix (int width, int height) 
		  //{ 
		  //glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
		  //gluPerspective (50.0*zoomFactor, (float)width/(float)height, zNear, zFar); 
		  /* ...Where 'zNear' and 'zFar' are up to you to fill in. */  
		
		
		set_camera();
		}



void ClassTy::set_pick(int cursorX, int cursorY){
	
	glRenderMode(GL_SELECT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDepthFunc( GL_LEQUAL );
   glEnable( GL_DEPTH_TEST );
  
 	r_cam.transform(cursorX,cursorY,5,5);
   glMatrixMode(GL_MODELVIEW);
   glInitNames();
  //This was reseting the accumulated rotates....
  // glLoadIdentity();
   return; 
   /////////////////////////////////////////////


GLint viewport[4];
	glRenderMode(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glGetIntegerv(GL_VIEWPORT,viewport);
	glLoadIdentity();

	
	float np=camera->near_plane();
	float fp=camera->far_plane();
	float _vw=100; float _vh=100;
	gluPickMatrix(cursorX,viewport[3]-cursorY,
			_vw,_vh,viewport);
	//		float ratio=2;
			
			cout <<camera->near_plane()<<" "<<camera->far_plane()<<"\n";

//	gluPerspective(30,ratio,camera->near_plane(),camera->far_plane());

//	gluPerspective(45,ratio,.01,1000);
	cout<<"cx="<<cursorX<<" y "<<cursorY<<" vp3="<<viewport[3]  <<"\n";
	 glFrustum( cursorX,  cursorX+ _vw,viewport[3]-cursorY ,viewport[3]-cursorY+_vh ,np, fp);
camera->get(5);	
	//camera->transform();
//	glGetDoublev(GL_PROJECTION_MATRIX);
	glMatrixMode(GL_MODELVIEW);
	glInitNames();
	}


void ClassTy::ResizeViewport(const count_type width, const count_type height)
{
 m_viewport[0]=-width/2; m_viewport[1]=-height/2;
 m_viewport[2]=width; m_viewport[3]=height;
}


camera_and_lights r_cam; 


