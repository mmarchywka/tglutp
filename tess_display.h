#ifndef TESS_DISPLAY_H
#define TESS_DISPLAY_H

#include <math.h>
#include <list>
typedef GLdouble * tess_type;
typedef list<tess_type> TessLiTy;
typedef TessLiTy::iterator TessLiTyItor;
typedef TessLiTy::const_iterator TessLiTyItorConst;
// #define GLvoid 
//first polygon: a star-5 vertices and color information
GLdouble star[5][6] = {  {0.6f,  -0.1f, -2.0f, 1.0f, 1.0f, 1.0f},
                        {1.35f, 1.4f, -2.0f, 1.0f, 1.0f, 1.0f},
                        {2.1f,  -0.1f, -2.0f, 1.0f, 1.0f, 1.0f},
                        {0.6f, 0.9f, -2.0f, 1.0f, 1.0f, 1.0f},
                        {2.1f, 0.9f, -2.0f, 1.0f, 1.0f, 1.0f} };//second polygon: a quad-4 vertices; first contour
GLdouble quad[4][6] = {{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f},
                        {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
                        {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f} };//second polygon: a triangle-3 vertices; second contour
GLdouble tri[3][6] = { {0.3f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f},
                       {0.7f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f},
                       {0.5f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f} };

// #define mAPIENTRY __attribute__ ((__stdcall__))  
#define mAPIENTRY   
void  mAPIENTRY combineCallback(GLdouble coords[3], GLdouble *vertex_data[4],
GLfloat weight[4], GLdouble **dataOut)
{
GLdouble *vertex;vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
vertex[0] = coords[0];
vertex[1] = coords[1];
vertex[2] = coords[2]+0*coords[1]*coords[0];for (int i = 3; i < 6; i++)
{
vertex[i] = 
weight[0] * vertex_data[0][i] +
 weight[1] * vertex_data[1][i] +
weight[2] * vertex_data[2][i] +
weight[3] * vertex_data[3][i];
}
//vertex[3]=.5; vertex[4]=.2f; vertex[5]=1-vertex[4];
*dataOut = vertex;
}
TessLiTy tess_list;

void  mAPIENTRY vertexCallback(GLvoid *vertex)
{
GLdouble *ptr;ptr = (GLdouble *) vertex;
glVertex3dv((GLdouble *) ptr);
glColor3dv((GLdouble *) ptr + 3);
/*
tess_type rptr = new GLdouble[6];
*(rptr++)=*(ptr+0); *(rptr++)=*(ptr+1); *(rptr++)=*(ptr+2); 
*(rptr++)=*(ptr+03); *(rptr++)=*(ptr+4); *(rptr++)=*(ptr+5); 
tess_list.push_back(rptr);
*/
}

class Tess_Poly {private:GLUtesselator *tobj; // the tessellation object

public:int Init();



int Set_Winding_Rule(GLenum winding_rule);
int Render_Contour(GLdouble obj_data[][6], int num_vertices);
int Begin_Polygon();
int End_Polygon();
int Begin_Contour();
int End_Contour();
int End();};

Tess_Poly Poly;
// compiler complains stdcall is ignored now 
//#undef mAPIENTRY
//#define mAPIENTRY __attribute__ ((__stdcall__))  
typedef void (mAPIENTRY *_GLUfuncptr)();


int Tess_Poly::Init()
{
// Create a new tessellation object 
tobj = gluNewTess(); // Set callback functions
gluTessCallback(tobj, GLU_TESS_VERTEX, (_GLUfuncptr)vertexCallback);
gluTessCallback(tobj, GLU_TESS_BEGIN, (_GLUfuncptr)glBegin);
gluTessCallback(tobj, GLU_TESS_END, (_GLUfuncptr)glEnd); //(void (*) ( ))
gluTessCallback(tobj, GLU_TESS_COMBINE, (_GLUfuncptr)combineCallback);return(1);
}

int Tess_Poly::Set_Winding_Rule(GLenum winding_rule)
{
// Set the winding rule
gluTessProperty(tobj, GLU_TESS_WINDING_RULE, winding_rule); return(1);
}

int Tess_Poly::Render_Contour(GLdouble obj_data[][6], int num_vertices)
{
for (int x = 0; x < num_vertices; x++) //loop through the vertices
{
gluTessVertex(tobj, obj_data[x], obj_data[x]); //store the vertex
}return(1);
}
int Tess_Poly::Begin_Polygon()
{
gluTessBeginPolygon(tobj, NULL);return(1);
}
int Tess_Poly::End_Polygon()
{
gluTessEndPolygon(tobj);return(1);
}
int Tess_Poly::Begin_Contour()
{
gluTessBeginContour(tobj);return(1);
}
int Tess_Poly::End_Contour()
{
gluTessEndContour(tobj);return(1);
}
int Tess_Poly::End()
{
gluDeleteTess(tobj);return(1);
}
 

GLdouble * contarr=NULL;
class poly_display
{
	public :
//GLdouble quad[4][6] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//                        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//                        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
//                        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, };
static GLdouble *  mkarray()
{
	contarr= new GLdouble[6*10*10];
	GLdouble* pos=contarr;
	for (int i=0; i<10;i++) {
		int ind=(i&1)?1:0;
	for (int j=ind?0:9; ((ind)?j:-j)<((ind)?10:1); j+=(ind?1:-1))
	{
	// cout <<j<<"\n";
	 *(pos++)=i; *(pos++)=j; *(pos++)=  i*i+j*j*sin(.3*(double) j);	
		*(pos++)=.5f*ind; *(pos++)=.1f; *(pos++)=.3f;
	}}
	return contarr;
	//create tess object; create callback functions
Poly.Init();
cout<<"a\n"; 
//set winding rule to positive
Poly.Set_Winding_Rule(GLU_TESS_WINDING_POSITIVE); 
cout<<"x\n";
Poly.Begin_Polygon(); 
Poly.Begin_Contour();
Poly.Render_Contour((GLdouble(*)[6]) contarr, 10*10); 
cout<<"z\n";
Poly.End_Contour();
cout<<"f\n";
Poly.End_Polygon();//render the second polygon: triangle cut out of a quad
cout<<"g\n";
Poly.End();
cout<<"b\n";
return contarr;

// this dies...
Poly.Init();
//set winding rule to odd
Poly.Set_Winding_Rule(GLU_TESS_WINDING_ODD); 
//begin the new polygon
Poly.Begin_Polygon(); 
Poly.Begin_Contour();
Poly.Render_Contour(quad, 4); 
Poly.End_Contour();
Poly.Begin_Contour();
Poly.Render_Contour(tri, 3); 
Poly.End_Contour();
Poly.End_Polygon();
//delete the tess object
Poly.End();

	
	return contarr;
	}
static void drawList()
{
	glDisable( GL_LIGHTING );
	
	glDisable( GL_CULL_FACE );
//	glBegin(GL_TRIANGLES);
//	for (TessLiTyItor p=tess_list.begin(); p!=tess_list.end(); ++p)
//	{
//		GLdouble * ptr=*p;
//		glVertex3dv((GLdouble *) ptr);
//		glColor3dv((GLdouble *) ptr + 3);
		
//		}
//	glEnd();
Poly.Init();
Poly.Set_Winding_Rule(GLU_TESS_WINDING_POSITIVE); 
Poly.Begin_Polygon(); 
Poly.Begin_Contour();
//Poly.Render_Contour((GLdouble(*)[6]) star, 5); 
Poly.Render_Contour((GLdouble(*)[6]) contarr, 10*10); 
Poly.End_Contour();
Poly.End_Polygon();//render the second polygon: triangle cut out of a quad
Poly.End();


	}

static void display(int argc, char *argv[])
{	//ggb->draw_func=poly_display::drawList;
ggb->set_df(poly_display::drawList);
poly_display::mkarray(); 	
}

};







class tess_display
{
	public :

static void drawList()
{
	glDisable( GL_LIGHTING );
	//drawMode=WIREFRAME;
	GLfloat diffuse[] = { 1,0,1,1};

         glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glDisable( GL_CULL_FACE );
	glBegin(GL_LINES);
	glColor3f( 1.0f, 0.0f, 1.0f );
  	for (float x=0; x<100; x+=10){ glVertex3f(x, 00.0f, 0.0f); glVertex3f(x, 100.0f, 0.0f);}
  	for (float x=0; x<100; x+=10){ glVertex3f(0,x, 0.0f); glVertex3f(100.0f, x, 0.0f);}

	glEnd();
	glColor3f( 1.0f, 1.0f, 0.0f );
	glBegin(GL_TRIANGLES);
  glVertex3f(0.0f, 0.0f,30);      // V0
  glVertex3f(25.0f, 25.0f,30);    // V1
  glVertex3f(50.0f, 0.0f,35);    // V2
glColor3f( 0.0f, 1.0f, 1.0f );
//glEnable( GL_LIGHTING );
  glVertex2f(-50.0f, 0.0f);    // V3
  glVertex2f(-75.0f, 50.0f);    // V4
  glVertex2f(-25.0f, 0.0f);    // V5
glEnd();
//	glEnable( GL_LIGHTING );
	
	
	}

static void display(int argc, char *argv[])
{//	ggb->draw_func=tess_display::drawList;
	ggb->set_df(tess_display::drawList);
}

};
#endif


