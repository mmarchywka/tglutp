//b1
/* const IdxTy ns = px2 -px;
								if ( (ns/TPSZ)*TPSZ!=ns) std::cerr<<__FILE__<<__LINE__<<
									"bad ptr size "<<ns<<'\n';
								while (px<px2)
								{  d.push_back( *(Ty*)(px)  );px+=TPSZ;  }
								*/
//b2
/*
							const IdxTy ns = px2 -px;
							if ( (ns/TPSZ)*TPSZ!=ns) std::cerr<<__FILE__<<__LINE__<<
									"bad ptr size "<<ns<<'\n';
							while (px<px2)
							{  d.push_back( *(Ty*)(px)  );  px+=TPSZ;}
							*/								
							
							
//b3
/*		
							const IdxTy ns = px2 -px;
							if ( (ns/TPSZ)*TPSZ!=ns) std::cerr<<__FILE__<<__LINE__<<
									"bad ptr size "<<ns<<'\n';
							while (px<px2)
							{  d.push_back( *(Ty*)(px)  );  px+=TPSZ; }
							*/	

/*
					}
					else 
					{	// all on one blcok.
					//	px+=1+strlen(key); // first entry...
						DaTy * px2=m_data+m_ptr;;
						//DaTy bend=m_data;
						// if this is not the last, then it could end on another block
						if (ptr!=sz-1)   px2=pt(ptr+1);      ;// bend=m_idx[ptr+1].first; 
						DoPush(d,px,px2,__FILE__,__LINE__);
							//b3
					}
					*/

/*	
						
						while ( m_b[ij].first!=bend)
						{ 	++ij; if ( ij==bmsz) break;  }
						//get all blocks that finish on this one... 
						for (IdxTy i=ii; i<ij;)
						{
							const DaTy * px2=m_b[i].first+m_b[i].second;
							DoPush(d,px,px2);//,__FILE__,__LINE__);
							++i;
							if (i<bmsz)  {px=m_b[i].first;  } else px=m_data; 
						}
					
					*/

/*
template <class TgtTy> class indexed_worm_database
{
	typedef indexed_worm_database Myt;
	typedef TgtTy Ty;
	typedef char Chr;
	typedef Chr DaTy;
	typedef Ty* Tpr;
	typedef unsigned int Tu;
	typedef int IdxTy;
	// These are the ordered addresses, Tu is a block index
	
	typedef std::vector<Tu> IdTy;
	enum {TPSZ=sizeof(Tpr) };
public:
	typedef Tpr target_ptr; 
	indexed_worm_database():m_data(new DaTy[1]),m_ptr(0),m_sz(1) { }
	~indexed_worm_database() { delete m_data; }
	// insert the next key and return a pointer to its new or existing target
	// this should really be an INDEX that can be used to a worm of a target type.
	// so, in this usage, the string worm points to an index to an entry worm
	// which is itself contiguous because the input is SORTED by key.
	target_ptr add( const DaTy * key)
	{
	 	const IdxTy need=strlen(key)+1+TPSZ;
	 	// if the key is new, then add it. Otherwise, just add the terminal
	 	// entry. On retrieve, you will just get all entries since you know
	 	// where the index ends and the next thing begins. 
	 	
	}
	void append_tgt_ptr( const Tpr p)
	{
		::memcpy(m_data+m_ptr,p,TPSZ);
		m_ptr+=TPSZ;
	}
private:
	// ptr to next write location, maximum size. 
	IdxTy m_ptr, m_sz;
	DaTy * m_data;
	IdTy m_idx;
	
};
*/


/*

MyClass::api_type MyClass::redraw() 
{
	 // http://www.cs.utexas.edu/users/fussell/courses/cs324e/hear50265_ch03.pdf
	// this is ck, there is no way to push the viewport info AFAIK.
//	return api_type(OK); 
	glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	glMatrixMode(GL_PROJECTION); 
	glPushMatrix(); 
	glLoadIdentity(); 
//	gluPerspective(50.0, 1.0, 3.0, 7.0); 
//	gluOrtho2D(-128,128,-128,128); //l,r,b,t
	//glViewport(-128,-128,256,256);
//	glViewport(0,512,0,512);
//	gluOrtho2D(0,512,0,512); 
	glMatrixMode(GL_MODELVIEW);
	
	//gluLookAt(0,0,3000,0,0,0,0,1,0);
	
	m_ptr->set_color(P4(1,1,1,1) );
	for (int i=0; i<100; ++i)
	{glPushMatrix();
	glLoadIdentity();
	m_ptr->set_loc(P3(20*i-00,20*i-00,0)); 
		m_ptr->draw_string("Marchywka",P3(i*20-100,20*i-100,0));
		glPopMatrix();
		glPopMatrix();
		glPopMatrix(); 
	}
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix(); 
	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
	//glViewport(vp[0],vp[1],vp[2],vp[3]);
	return api_type(OK); 

}
*/

 /*
	glPushMatrix();
	// std::cout<<" label "<<(*ii).second<<" ";
	const Ch * c= s.c_str();
	glColor4f(s.r,s.g,s.b, 1.0f);
	glTranslatef( s.x,s.y,s.z );
	glScaled(s.scale, s.scale, 0.1f);
	float stroke_scale=0.1f;
	while (*c)
	{
	 	glPushMatrix();
		//glColor4f(0.0, 1.0, 0.0, 1.0f);
		glLineWidth(1);
	 	glScalef(stroke_scale, stroke_scale, stroke_scale);
	 	while (*c) 
	 	{
	 		if (*c=='\n'){c++; break;} 
	 		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);c++;
	 	}
		glPopMatrix();
		glTranslatef( 0,-20,0 );
	}
	glPopMatrix();
	*/

/*//	glDepthRange(-1000,1000);
//	gluPerspective(45,ratio,1,1000);
	float r = 500.0f;
	float eyeX = 0; //r * sin(theta * radianFactor) * cos(phi * radianFactor);
	float eyeY = 0;// r * sin(theta * radianFactor) * sin(phi * radianFactor);
	float eyeZ = r;// * cos(radianFactor * theta);

	float centerX = 0, centerY = 0, centerZ = 0;
	float upX = 0, upY = 1.0f, upZ = 0;

	// use gluLookAt or do it by hand
	//if(gluLookAt_On) {
	//	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ); 
	
	*/





