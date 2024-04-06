#ifndef CAMERA_AND_LIGHTS_H
#define CAMERA_AND_LIGHTS_H


#include "Camera.h"
#include "../data_and_config.h"
#include "../debug_and_support.h"
#include "../base/points_base_classes.h"

#include <GL/glut.h>
#include <string>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



class camera_and_lights
{
typedef std::basic_string<char> string_type;  
	
typedef camera_and_lights Myt;
typedef myPoint3 PointTy;
typedef GLfloat coord_type;
typedef int count_type;
typedef myPoint4<int> SquareTy;
typedef point3_template<int> CenterTy; 
public:
camera_and_lights() : m_eye(),m_look(),m_up(),m_light(),m_rot_phi(0),
	m_rot_theta(0),m_rot_center(),m_xlate_sum(),camera(0),m_old_new(false) {}
camera_and_lights(const count_type width, const count_type height, const float ratio,
 const Point3 & p) : m_eye(),m_look(),m_up(),m_light()
,m_rot_phi(0),m_rot_theta(0),m_rot_center(),m_xlate_sum(),camera(new Camera(width,height, ratio,p)),m_old_new(false)
 {
 	//http://www.opengl.org/resources/code/samples/mjktips/rts/hello2rts.c
 	ResizeViewport(width,height);
 //m_viewport[0]=-width/2; m_viewport[1]=-height/2;
 //m_viewport[2]=width; m_viewport[3]=height;
 m_near=.01; m_far=10000;m_zoom=1;
 m_eye=PointTy(000,000,1); m_look=PointTy(0,0,0); m_up=PointTy(0,1,0);
 m_light=m_eye;
 //m_old_new=true;
 set_camera(); 
 	
 	}

void set_rot_center( const CenterTy & c) { m_rot_center=c; } 
void set_scene_center( const CenterTy & c); 
void rotate_scene(const coord_type d1, const PointTy & n1,
const coord_type d2, const PointTy & n2) ;
void rotate_light(const coord_type _d1, const PointTy & _n1,
const coord_type _d2, const PointTy & _n2) ;
void set_slab(const coord_type near, const coord_type far);
coord_type get_near() //const
 {return get(0); } 
coord_type get_far() //const 
{return get(1); } 
void set_camera();

void near_plane(coord_type z);

void move_near_plane( coord_type dz);

void far_plane(coord_type z);

void move_far_plane( coord_type dz);

//################################################################

Point3 getTarget();

void transform () ;
void transform (int x, int y, int w, int h) ;

Point3 getPosition(); 
void resizeViewport( const count_type w, const count_type h);

float convertLength(const Point3& z, double f);

Ray computeRay(int x, int y);

count_type width() const ;//{ return r_cam.getViewportWidthInPixels(); }
count_type height() const;// { return r_cam.getViewportHeightInPixels(); }

void fix_light_position();

void dollyCameraForward(float d, bool b) ;
float get(int x);
void clip_up(float x); 
void reset() ;
void move_something(int thing, int delta_x, int delta_y);

//#################################################################

Point3 lightPosition();


void XlateSceneRightAndUp( const coord_type dx, const coord_type dy ) ;//{}
void XlateCameraRightAndUp(const coord_type dx,const coord_type dy ); //{}
void PitchAndYawCamera( const coord_type dx, const coord_type dy ) ;//{}
void Zoom(const coord_type dx );

void set_pick(int cursorX, int cursorY);

//###############################################
private:

void ResizeViewport(const count_type width, const count_type height);

PointTy m_eye,m_look,m_up,m_light;
SquareTy m_viewport;
coord_type m_near, m_far,m_zoom, m_rot_phi,m_rot_theta;
 
CenterTy m_rot_center,m_xlate_sum;
Camera* camera ;
bool m_old_new;
};
extern camera_and_lights r_cam; 

#endif
