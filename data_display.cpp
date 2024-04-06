#include "data_display.h"

data_display::ThingTy data_display::thing_list;
data_display::ThingTy data_display::line_list;
data_display::TeLiTy data_display::label_list; 
data_display::ThingTy data_display::norm_list; 

namespace {    GLUquadricObj* xxxx=gluNewQuadric();};
void data_display::norm (float * x)
{
//	int asd[]={0};
//	 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE ,asd);
	 myglutil::generic_lighting(); 
//	 	int asd[]={0};
	 glLightModeli(GL_LIGHT_MODEL_TWO_SIDE ,1);
	 glDisable( GL_CULL_FACE );
	 glCullFace(0);
	 GLfloat no_shininess[] = { 0.0 };
     GLfloat low_shininess[] = { 5.0 };
     GLfloat high_shininess[] = { 100.0 };
     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (true)?no_shininess:low_shininess);
//       
//	myglutil::lights_off(7);
//myglutil::lights_on(1);
//	glDisable(GL_LIGHT0);
//	glDisable(GL_LIGHT1);
//	glDisable(GL_LIGHT2);
 //glEnable( GL_LIGHTING );glEnable( GL_LIGHT0 );
 glShadeModel(GL_SMOOTH);
    //	glEnable(GL_BLEND);
    //	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //	glEnable (GL_DEPTH_TEST);
  //  	glEnable (GL_LIGHTING);
    	//float xa[]={0,0,0,0};
//     		xa[0]=lightPosition.x(); xa[1]=lightPosition.y(); 
  //   		xa[2]=lightPosition.z(); xa[3]=0; 
     	//	glLightfv( GL_LIGHT0, GL_POSITION, xa );
  //  	glEnable (GL_LIGHT0);
   // 	glEnable (GL_LIGHT1);
//    	glEnable (GL_LIGHT2);
//    	glEnable (GL_LIGHT3);
   // 	glEnable (GL_LIGHT4);
    	glEnable (GL_NORMALIZE);
// this is simply in the - no apologies, this is .
//	glEnd();
		glPushMatrix();
	//	glPointSize(x[9]); 
	//	glBegin(GL_POINTS);
	// this should be first google hit on  
	//	http://www.opengl.org/sdk/docs/man/xhtml/glLightModel.xml
		GLfloat mat_diffuse[] = { x[6],x[7],x[8],1.0 };
		GLfloat mat_diffusex[] = { 0,0,0,1.0 };
		float am=.35f;
		float off=.05f;
		GLfloat mat_diffusef[] = { x[6]*am+off,x[7]*am+off,x[8]*am+off,1.0 };
		 	glColor3f(x[6],x[7],x[8] );
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_diffusex);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffusef);
    
//   		float x[]={n.x(),n.y(),n.z()};
//		glNormal3fv(x+3);
	//	 myglutil::lights_off(7);
	//	  myglutil::lights_on(1);
		glTranslatef( x[0],x[1],x[2] ); 
   	//	glVertex3f(x[0],x[1],x[2]); 
   		float m_nx=x[3], m_ny=x[4],m_nz=x[5];
   		if ( m_nx>0) {m_nx=-m_nx; m_ny=-m_ny; m_nz=-m_nz;};
   		float mag=sqrt(m_nx*m_nx+m_ny*m_ny+m_nz*m_nz);
  	    float phi=180/3.14*acos(m_nz/mag); // this is target.Z;
  	    // std::cout<<phi<<" "<<m_ny<<" "<<m_nx<<" "<<mag<<"\n";
  	   
  	    if (phi!=0) {  glRotatef(phi,-m_ny/mag,m_nx/mag,0); }
  	    // myglutil::lights_on(1);  
  	 //   glEnable( GL_LIGHTING );glEnable( GL_LIGHT0 );
  	 //  	glLightfv( GL_LIGHT0, GL_POSITION, xa );
  	   	float ju[3]={1.0f,0.0f,0.0f};
  	   	float jun[3]={-1.0f,0.0f,0.0f};
  	   	float juz[3]={0.0f,0.0f,1.0f};
  	   	float junz[3]={0.0f,0.0f,-1.0f};
  	   	float ss=x[9];
  	   	float *nn=junz;//x+3; //juz;//x+3;
  	   	float * nnz=jun;
  	 //  	glNormal3fv(nn); 
  	    glBegin(GL_QUADS);
  	    //gluDisk( xxxx,0,x[9],4,2); 
			glNormal3fv(nn); glVertex3f(-ss, -ss, 0.0f);
			glNormal3fv(nn); glVertex3f(-ss, ss, 0.0f);
			glNormal3fv(nn); glVertex3f(ss, ss, 0.0f);
			glNormal3fv(nn); glVertex3f(ss, -ss, 0.0f);
	//		glNormal3fv(nnz); glVertex3f(-ss, -ss, 0.0f);
	//		glNormal3fv(nnz); glVertex3f(ss, -ss, 0.0f);
	//		glNormal3fv(nnz); glVertex3f(ss, ss, 0.0f);
	//		glNormal3fv(nnz); glVertex3f(-ss, ss, 0.0f);
		
		glEnd();  	    
  	    //glColor3f( 1.0f, 0.0f, 1.0f );
  	    //myglutil::lights_off(1); 
  	   //  glDisable( GL_LIGHTING ); glDisable( GL_LIGHT0 );
  	    //if (phi!=0) {  glRotatef(-phi,-m_ny/mag,m_nx/mag,0); }
   		glPopMatrix();	
  // 		myglutil::lights_on(3);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
//	glEnable(GL_LIGHT2);
}

void data_display::axes()
{
	glDisable( GL_CULL_FACE );
	glBegin(GL_LINES);
//	glDisable( GL_LIGHTING );
	
	for (float z=-100; z<1000; z+=100)
	{
//	glColor3f( 1.0f, 0.0f, 1.0f );
	GLfloat mat_diffuse[] = { 1, 0, 1, 1.0 };
          glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
          glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
  	for (float x=0; x<1000; x+=100){ glVertex3f(x, 00.0f, z); glVertex3f(x, 1000.0f, z);}
  	GLfloat xmat_diffuse[] = { 0, 1.0f, 1, 1.0 };
          glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, xmat_diffuse);
  	glMaterialfv(GL_FRONT, GL_AMBIENT, xmat_diffuse);
//  	glColor3f( 0.0f, 1.0f, 1.0f );
  	for (float x=0; x<1000; x+=100){ glVertex3f(0,x, z); glVertex3f(1000.0f, x, z);}
	}
//	glColor3f( 1.0f, 1.0f, 0.0f );
  //	for (float x=0; x<100; x+=10){ glVertex3f(0,0.0f, x); glVertex3f(100.0f, 0, x);}
	glEnd();
	
}


void data_display::drawBoxList()
{
	for (int x=0; x<20; ++x) {   
  	  glPushMatrix();

 GLfloat mat_diffuse[] = { 1, x*1.0f/20, 1, 1.0 };
          glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
          myglutil::generic_mat_1(); 
  glTranslatef( x, 0, 0); int mv=0;
      float h=x; while (h>=0) {++mv; glTranslatef( 0, 0, 1); glutSolidCube( 1 ); h=h-1;}
       glTranslatef( -x, 0, -mv);
      glPopMatrix();
}
glDisable( GL_CULL_FACE );
	//glShadeModel( GL_FLAT );
	glBegin(GL_LINES);
	  myglutil::lights_off(0);  //glDisable( GL_LIGHTING );
	//glShadeModel(  GL_SMOOTH  );
	//glEnd();
	glColor3f( 1.0f, 0.0f, 1.0f );
  	for (float x=0; x<100; x+=10){ glVertex3f(x, 00.0f, 0.0f); glVertex3f(x, 100.0f, 0.0f);}
  	for (float x=0; x<100; x+=10){ glVertex3f(0,x, 0.0f); glVertex3f(100.0f, x, 0.0f);}
	//glEnable( GL_CULL_FACE );
	//glShadeModel(  GL_SMOOTH  );
	glEnd();
	glBegin(GL_TRIANGLES);
  glVertex2f(0.0f, 0.0f);      // V0
  glVertex2f(25.0f, 25.0f);    // V1
  glVertex2f(50.0f, 0.0f);    // V2

  glVertex2f(-50.0f, 0.0f);    // V3
  glVertex2f(-75.0f, 50.0f);    // V4
  glVertex2f(-25.0f, 0.0f);    // V5
glEnd();
//	glEnable( GL_LIGHTING );
	
	
}

void data_display::draw_a_thing()
{
//	std::cout<<"adscasdcasasdcascascasdcads\n";
	//axes();
		myglutil::lights_off(7);
myglutil::lights_on(1);
//	glDisable(GL_LIGHT0);
//	glDisable(GL_LIGHT1);
//	glDisable(GL_LIGHT2);
 //glEnable( GL_LIGHTING );glEnable( GL_LIGHT0 );
 glShadeModel(GL_SMOOTH);
	 GLfloat no_shininess[] = { 0.0 };
     GLfloat low_shininess[] = { 5.0 };
     GLfloat high_shininess[] = { 100.0 };
     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (true)?no_shininess:low_shininess);
     
     
	generic_lighting(); 
	bool pointss=false;
	labels();
	if ( pointss)
	{	  myglutil::lights_off(0); //glDisable( GL_LIGHTING );
		glDisable(GL_POINT_SMOOTH);
		glDisable( GL_CULL_FACE );
		glPointSize(3.0);
		glBegin(GL_POINTS);}
	//	std::cout<<"atlbesllllll\n";
		
//	count_type n=thing_list.size();
	typedef ThingTy::const_iterator ItorTy;
	ItorTy li=thing_list.begin(), ie=thing_list.end();
	while (li!=ie){
		thing_item_type x=(*li++);
		
		
		
//		GLfloat diffuse[] = { x[4],x[5],x[6],1.0f};
//		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse); 
//		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);  


		GLfloat mat_diffuse[] = { x[4],x[5],x[6],1.0 };
		GLfloat mat_diffusex[] = { 0,0,0,1.0 };
		GLfloat mat_diffusef[] = { x[4]*.1f+.05f,x[5]*.1f+.05f,x[6]*.1f+.05f,1.0 };
		 	glColor3f(x[6],x[7],x[8] );
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_diffusex);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
     	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffusef);



//          glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 
  //        m_highlight?high_shininess:low_shininess);
		glPushMatrix();
	if ( pointss) {
		 glEnd();
		 glPointSize(x[0]); 
		 glBegin(GL_POINTS);
		glColor3f(x[4],x[5],x[6]); glVertex3f(x[1],x[2],x[3]);
		}
  	else {glTranslatef( x[1],x[2],x[3] );  
  //	std::cout<<x[1]<<" "<<x[2]<<" "<<x[3]<<'\n';
  	glutSolidSphere( x[0], 10, 10 ); }
  	glPopMatrix(); 
		}
	if ( pointss) glEnd(); 
	//std::cerr<<" line size is "<<line_list.size()<<"\n";
	if (line_list.size()!=0) lines(line_list); 
	if (label_list.size()!=0) labels(label_list);
	if (norm_list.size()!=0) norms(norm_list);
	}
void data_display::make_disks ()
{
	float x=10,m_x,m_y,m_z, m_nx,m_ny,m_nz;
	float ss=1;
	generic_lighting();
	
	
	for (int i=0; i<100; i+=2){
	for (int k=0; k<100; k+=2 )
	{
		m_x=(i-50); m_y=(k-50); m_z=sqrt(3*50*50-m_x*m_x-2*m_y*m_y); //m_z*=m_z;
		float  m_zx=sqrt(3*50*50-(m_x+1)*(m_x+1)-2*m_y*m_y);
		  float m_zy=sqrt(3*50*50-m_x*m_x-2*(m_y+1)*(m_y+1));
		m_nx=m_zx-m_z;; m_ny=m_zy-m_z; m_nz=1;
		glColor3f( 1.0f, 0.0f, 1.0f );
	
 GLfloat mat_diffuse[] = { 1, x*1.0f/20, 1, 1.0 };
          glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
          myglutil::generic_mat_1();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glDisable( GL_CULL_FACE );
  myglutil::lights_off(1); 		glDisable( GL_LIGHTING ); glDisable( GL_LIGHT0 );
		glTranslatef( m_x*ss,m_y*ss,m_z*ss ); 
		float mag=sqrt(m_nx*m_nx+m_ny*m_ny+m_nz*m_nz);
  	        float phi=180/3.14*acos(m_nz/mag); // this is target.Z;
  	       // std::cout<<phi<<" "<<m_ny<<" "<<m_nx<<" "<<mag<<"\n";
  	        if (phi!=0) {  glRotatef(phi,-m_ny/mag,m_nx/mag,0); }
  	          myglutil::lights_on(1); // glEnable( GL_LIGHTING );glEnable( GL_LIGHT0 );
  	        gluDisk( xxxx,0,1.0f,4,2); //glColor3f( 1.0f, 0.0f, 1.0f );
  	         myglutil::lights_off(1); // glDisable( GL_LIGHTING ); glDisable( GL_LIGHT0 );
  	        if (phi!=0) {  glRotatef(-phi,-m_ny/mag,m_nx/mag,0); }
  	//delete xxx;
  	//cout <<m_rad<<" "<<m_x<<" "<<m_y<<" "<<m_z<<"\n";
  	glTranslatef(- m_x*ss, -m_y*ss, -m_z*ss );
  	
  	  myglutil::lights_on(1);  ;;//glEnable( GL_LIGHTING );glEnable( GL_LIGHT0 );
		}
		}}



//static 
void data_display::display_nurb(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glCallList(1);
  glFlush();
}

//static 
int data_display::main_nurb(int argc, char *argv[])
{GLfloat mat_red_diffuse[] = { 0.7, 0.0, 0.1, 1.0 };
GLfloat mat_green_diffuse[] = { 0.0, 0.7, 0.1, 1.0 };
GLfloat mat_blue_diffuse[] = { 0.0, 0.1, 0.7, 1.0 };
GLfloat mat_yellow_diffuse[] = { 0.7, 0.8, 0.1, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 100.0 };
GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
GLfloat pts1[4][4][3], pts2[4][4][3];
GLfloat pts3[4][4][3], pts4[4][4][3];
GLUnurbsObj *nurb;
int u, v;
std::cout<<MM_MARK<<"  this it\n";
glut_generic::almost_start_glut(&argc,argv);
/*
  glutInit(&argc, argv);
  glutCreateWindow("molehill");
  */
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   myglutil::lights_on(1); // glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  nurb = gluNewNurbsRenderer();
  gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 25.0);
  gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);

  /* Build control points for NURBS mole hills. */
  for(u=0; u<4; u++) {
    for(v=0; v<4; v++) {
      /* Red. */
      pts1[u][v][0] = 2.0*((GLfloat)u);
      pts1[u][v][1] = 2.0*((GLfloat)v);
      if((u==1 || u == 2) && (v == 1 || v == 2))
	/* Stretch up middle. */
	pts1[u][v][2] = 6.0;
      else
	pts1[u][v][2] = 0.0;

      /* Green. */
      pts2[u][v][0] = 2.0*((GLfloat)u - 3.0);
      pts2[u][v][1] = 2.0*((GLfloat)v - 3.0);
      if((u==1 || u == 2) && (v == 1 || v == 2))
	if(u == 1 && v == 1) 
	  /* Pull hard on single middle square. */
	  pts2[u][v][2] = 15.0;
	else
	  /* Push down on other middle squares. */
	  pts2[u][v][2] = -2.0;
      else
	pts2[u][v][2] = 0.0;

      /* Blue. */
      pts3[u][v][0] = 2.0*((GLfloat)u - 3.0);
      pts3[u][v][1] = 2.0*((GLfloat)v);
      if((u==1 || u == 2) && (v == 1 || v == 2))
	if(u == 1 && v == 2)
	  /* Pull up on single middple square. */
	  pts3[u][v][2] = 11.0;
	else
	  /* Pull up slightly on other middle squares. */
	  pts3[u][v][2] = 2.0;
      else
	pts3[u][v][2] = 0.0;

      /* Yellow. */
      pts4[u][v][0] = 2.0*((GLfloat)u);
      pts4[u][v][1] = 2.0*((GLfloat)v - 3.0);
      if((u==1 || u == 2 || u == 3) && (v == 1 || v == 2))
	if(v == 1) 
	  /* Push down front middle and right squares. */
	  pts4[u][v][2] = -2.0;
	else
	  /* Pull up back middle and right squares. */
	  pts4[u][v][2] = 5.0;
      else
	pts4[u][v][2] = 0.0;
    }
  }
  /* Stretch up red's far right corner. */
  pts1[3][3][2] = 6;
  /* Pull down green's near left corner a little. */
  pts2[0][0][2] = -2;
  /* Turn up meeting of four corners. */
  pts1[0][0][2] = 1;
  pts2[3][3][2] = 1;
  pts3[3][0][2] = 1;
  pts4[0][3][2] = 1;

  glMatrixMode(GL_PROJECTION);

  gluPerspective(55.0, 1.0, 2.0, 24.0);
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0.0, 0.0, -15.0);
  glRotatef(330.0, 1.0, 0.0, 0.0);

  glNewList(1, GL_COMPILE);
    /* Render red hill. */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_red_diffuse);
    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots,
        4 * 3, 3, &pts1[0][0][0],
        4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

    /* Render green hill. */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_green_diffuse);
    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots,
        4 * 3, 3, &pts2[0][0][0],
        4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

    /* Render blue hill. */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_blue_diffuse);
    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots,
        4 * 3, 3, &pts3[0][0][0],
        4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

    /* Render yellow hill. */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_yellow_diffuse);
    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots,
        4 * 3, 3, &pts4[0][0][0],
        4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);
  glEndList();

 // glutDisplayFunc(display_nurb);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
