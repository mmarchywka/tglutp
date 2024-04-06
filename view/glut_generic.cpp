#include "glut_generic.h"

glut_generic * glut_generic::r_one=NULL; 


typedef glut_generic MyClass;
 void MyClass::reshapeCallback( int width, int height ) 
{ r_one->ReshapeCallback(width,height); }
void MyClass::ReshapeCallback( int width, int height ) 
{      r_cam.resizeViewport( width, height ); }
void MyClass::timerCallback( int id)
{ r_one-> TimerCallback(id);}
void MyClass::TimerCallback( int  id  ) {
MM_ERR(MMPR(__FUNCTION__))
  
//  std::cout<<"timed callback\n";
  m_timed.run();
  //if (timer_func) timer_func();
 // text_commander.cin_problem();
   glutTimerFunc( timerInterval, timerCallback, 0 );
  // std::cout<<"timed callback exit \n";
}

void MyClass::drawFinish()
{
  std::cout<<"drawFinishk\n";
   if ( drawMode == WIREFRAME ) glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
   else glDisable( GL_LIGHTING );
   // ----- draw the selected picking ray
 //  if ( drawPickingRay ) {glColor3f( 0, 1, 1 ); drawRay( pickingRay ); }
   // ----- draw little crosshairs at the camera's target
//   if ( drawTarget ) { const Point3& t = r_cam.getTarget(); glColor3f( 0, 1, 1 ); drawCrossHairs( t, r_cam.convertLength( t, 0.05 ) );}
   
}

void MyClass::modelinit()
{
MM_ERR(MMPR(__FUNCTION__))
//  std::cout<<"ModelInit ok\n";
	 Matrix m;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDepthFunc( GL_LEQUAL );
   glEnable( GL_DEPTH_TEST );
   if ( cullBackfaces ) glEnable( GL_CULL_FACE );
   else glDisable( GL_CULL_FACE );

   r_cam.transform();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
//  glEnable(GL_BLEND);		// Turn Blending On
//			glDisable(GL_DEPTH_TEST);
   glColor3f( 1, 1, .1 );	
  std::cout<<"ModelInit DUN ok\n";
}
void MyClass::drawCallback() {r_one->DrawCallback(); }
void MyClass::DrawCallback() {
//  std::cout<<"DrawCallBack ok\n";
MM_ERR(MMPR(__FUNCTION__))
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (need_init) {modelinit();};
	glPushMatrix();
  	r_cam.transform();
 
   	if ( drawMode == WIREFRAME ) {glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );}
   	else {
      	Point3 lightPosition =r_cam.lightPosition();  
     	if (need_init){ 
     		float xa[]={0,0,0,0};
     		xa[0]=lightPosition.x(); xa[1]=lightPosition.y(); 
     		xa[2]=lightPosition.z(); xa[3]=0; 
     		glLightfv( GL_LIGHT0, GL_POSITION, xa );
      		}
      		glShadeModel( drawMode==SMOOTH_SHADED ? GL_SMOOTH : GL_FLAT );
	}need_init=false; 

    	myglutil::lights_off(1); 
  	glPopMatrix();
  	glMatrixMode(GL_MODELVIEW);
    	myglutil::lights_on(1);
  	glShadeModel( drawMode==SMOOTH_SHADED ? GL_SMOOTH : GL_FLAT );
 	glShadeModel(  GL_SMOOTH  );
   
 	if (draw_func) (*draw_func)();
 	m_draw.run();
	std::cout<<"in draw call back\n";
	m_anot.redraw(); 
	drawFinish();
	myglutil:: lights_on(1); 
   	glutSwapBuffers();
   	++ frameCount;
   	std::cout<<"return from dark\n";
}
void MyClass::start_glut( int* argc, char *argv[]) {r_one->StartGlut(argc,argv); }
void MyClass::almost_start_glut( int* argc, char *argv[]) {r_one->AlmostStartGlut(argc,argv); }
void MyClass::start_glut_main_loop( ) {glutMainLoop();}
void MyClass::set_panic_location(int * p) {text_commands::global_panic=p; }
void MyClass::AlmostStartGlut( int* argc, char *argv[])
{		
//std::cout<<" glut_generic almost starting ghlut \n";
// needed for atom but not streaming data???? 
   glutInit( argc, argv );

MM_ERR(MMPR(__FUNCTION__))
//std::cout<<" T comment out will adfasdfas  viewer main\n";


   glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
   glutInitWindowSize( m_w_w, m_w_h );
   glutCreateWindow("Marchywka");

 cout<<"window made ok \n";
   glutDisplayFunc( Myt::drawCallback );
   glutReshapeFunc( Myt::reshapeCallback );
   glutKeyboardFunc( text_commands::keyboardCallback );
   glutSpecialFunc( text_commands::specialCallback );
   glutMouseFunc( mouse_commands::mouseCallback );
   glutPassiveMotionFunc( mouse_commands::passiveMotionCallback );
   glutMotionFunc( mouse_commands::motionCallback );
   idle_dispatch=idleCallback;
	glutIdleFunc( idle_dispatch/*idle_func*/);
   glutTimerFunc( timerInterval, timerCallback, 0 );
//right_menu();
   
 cout<<"lf way through init  \n";

   int width = glutGet( GLUT_WINDOW_WIDTH );
   int height = glutGet( GLUT_WINDOW_HEIGHT );
 r_cam = camera_and_lights(width,height,800,Point3(0,0,0)); 


 cout<<"done with init   \n";
	
}
void MyClass::StartGlut( int* argc, char *argv[])
{
MM_ERR(MMPR(__FUNCTION__))
 AlmostStartGlut(argc,argv);
 cout<<"Starting glut mainloop\n";
   glutMainLoop();
	
	}






