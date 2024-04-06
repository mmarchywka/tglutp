#include "mouse_commands.h"



mouse_commands * mouse_commands::r_one=NULL;

void mouse_commands::MouseCallback( int button, int state, int x, int y ) {
   int mod = glutGetModifiers();
cout<<" mouse callback "<<mod<<"\n";
   CTRL = mod & GLUT_ACTIVE_CTRL;
   ALT = mod & GLUT_ACTIVE_ALT;
   SHIFT = mod & GLUT_ACTIVE_SHIFT;
   LMB=   (button==GLUT_LEFT_BUTTON)&&(state == GLUT_DOWN);
   RMB=   (button==GLUT_RIGHT_BUTTON)&&(state == GLUT_DOWN);
   
   if (RMB)
   { // try picking... 
  // glut_picking::startPicking(x,y);
  // glEnable( GL_LIGHTING );
   	if(ggb->draw_select_func) ggb->draw_select_func(x,y); //drawSelectionSphereList();
//   	glutSwapBuffers();
   //	glut_picking::stopPicking();
   	}
   mouse_state=(CTRL?1:0)+(ALT?2:0)+(SHIFT?4:0)+(RMB?8:0)+(LMB?16:0);
     if ( mouse_state&(8|16) ) { ggb->pickingRay = r_cam.computeRay( x, y );glutPostRedisplay(); }
   mouse_x = x; mouse_y = y;
//   printf("buttons:(%d,%d,%d); modifiers:(%c,%c,%c); position:(%d,%d)\n",
  //    LMB ? 1 : 0, MMB ? 1 : 0, RMB ? 1 : 0, CTRL ? 'C' : '-', ALT ? 'A' : '-', SHIFT ? 'S' : '-',mouse_x, mouse_y);
}

  void mouse_commands::PassiveMotionCallback( int x, int y ) {
   mouse_x = x;
   mouse_y = y;
   // printf("move:(%d,%d)\n", mouse_x, mouse_y );
}

void mouse_commands::MotionCallback( int x, int y ) {
   int delta_x = x - mouse_x;
   int delta_y = mouse_y - y;
   int delta = delta_x + delta_y;
//	 mouse_state=(CTRL?1:0)+(ATL?2:0)+(SHIFT?4:0)+(RMB?8:0)+(LMB?16:0);
   if (mouse_state&8) return; // move during select is annoying...
   int action=mouse_state&7; 
cout<<" mouse entry "<<action<<"\n";

   switch (action)
   {
     case 1: {   
     	float crap=r_cam.get(0);float junk=r_cam.get(1);
     	 cout<<"dollyfwd x, nearplan y was "<<crap <<" dx="<<delta_x<<" dy="<<delta_y<<"\n"; 
     	 	 r_cam.dollyCameraForward( 3*delta_x, false );
                 float newcrap=delta_y>0?(crap*delta_y):crap;
                 newcrap=(delta_y<0)?(-crap/delta_y):newcrap;  
                 newcrap=crap+delta_y*1;           
                 if (newcrap<.01) newcrap=.01; if (newcrap>.9*junk) newcrap=junk*.9;
                 r_cam.clip_up(newcrap); 
      break; }
      case 2: {    cout<<"xlatescene right,up\n";  r_cam.move_something(0, delta_x, delta_y ); break; }
      case 3: {  cout<<"roll camera right\n";  r_cam.move_something(1, delta,0 );break; }
     case 4:  {   cout<<"pitch camera up\n";  r_cam.move_something(2, delta_x,delta_y );    break; }
     case 5:  {   cout<<"zoom in\r";r_cam.move_something(4, delta,0 );break; }
case 6:   { r_cam.rotate_scene(delta_x,myPoint3(1,0,0),delta_y,myPoint3(0,0,1));break; }
case 7:   { r_cam.rotate_scene(delta_x,myPoint3(0,1,0),delta_y,myPoint3(0,0,1));break; }
     case 0:      {r_cam.move_something(0, delta_x, delta_y );;break; }
   }

   glutPostRedisplay();

   mouse_x = x;
   mouse_y = y;
   // printf("drag:(%d,%d)\n", mouse_x, mouse_y );
}



