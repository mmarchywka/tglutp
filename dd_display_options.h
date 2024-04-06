#ifndef DD_DISPLAY_OPTIONS_H__
#define DD_DISPLAY_OPTIONS_H__

//#include "../math/linear_regression_matrix.h"
#include "test/noisy_signal_source.h"
#include "test/testing_histogram.h"

#include "./math/hypergeometric.h"

//#include <gsl/gsl_sf_hyperg.h>
#include <math.h>

#include <vector>
#include <list>

class dd_display_options
{
	typedef dd_display_options Myt;
	typedef display_generic_text TextTy;
	typedef int IdxTy;
	typedef char Chr;
	typedef std::string string_type;
	typedef string_type St;
	// histo model
	typedef testing_histogram<1000,float> HistoTy;
	typedef std::vector<HistoTy> Hv;
	// 3d model. 
	typedef std::pair<int,int> GridTy;
	typedef std::vector<float> GValTy;
	typedef std::vector<TextTy> TeLiTy;
	typedef std::map<GridTy,GValTy> ZedTy;

	// general stuff
	class myColor
	{public: myColor(float rr, float gg, float bb) : r(rr),g(gg),b(bb ){}
		float r,g,b;
		
	};
	typedef std::list<myColor> ColorStack;
	
public:
	dd_display_options(): //histo(10,250), 
	m_length(0),m_type("raw"),m_handler(0),m_hist_len(50)
	{}
	template <class Ty > int configure(Ty& co) { return Config(co); }
	// call once after config.
	void init()
	{data_display::display_stl(p_list); }
	
	// after adding one or more data points, recalculate any required
	// expensive stuff and be ready to displat.
	void update() 
	{
		ReCalculate();
		ReDisplay();
	}
	//histo>> push_from_model;
	
	// add points with making display consistent.
//	IdxTy push_from_model(const IdxTy len, float *x){	return PushFromModel(len,x);}
	IdxTy push_from_model(const IdxTy len, Chr **x)
	{	

std::cout<<MM_MARK<<" pushing data "<<len<<"\n";
return PushFromModel(len,x);}
	
	template <class Os> void help(Os& os) {Help(os); }
private:
	
	
	void ReCalculate() 
	{	// what is a virtual function? 
		// yes, I no what a virtual is
std::cout<<MM_MARK<<" recalculate called  "<<m_handler<<"\n";
		switch (m_handler)
		{
			// raw data type
			case 0:{ 
				TrimModel(m_hist_len);
			break;	
			}
			// bunch of histograms 
			case 1:{ 
					ClrModel();
					IdxTy sz=m_histos.size();
					mud()=this;
					for (IdxTy i=0; i<sz; ++i) {idx()=i; m_histos[i]>>DumPush;	}				  
					break;
			}
			// 3d model
			case 2:{ 
					
					ZedTy::iterator ii=m_zed.begin(),ie=m_zed.end();
					// should be an incremental way to do this...
					ClrModel();
					while ( ii!=ie)
					{
						GridTy lo=(*ii).first;
						GValTy& gv=(*ii).second;
						float x[7];
						// size, x,y,z,r,g,b
						x[0]=1; x[1]=lo.first; x[2]=lo.second;
						for (IdxTy i=3; i<7; ++i) x[i]=1;
						if (gv.size()!=0) x[3]=gv[0];
						if (gv.size()>4) 
						{x[0]=gv[1];x[4]=gv[2];x[5]=gv[3];x[6]=gv[4];} 
						RawPush(x); 
						++ii;
					}
					//ClrModel();
					//IdxTy sz=m_histos.size();
					//mud()=this;
					//for (IdxTy i=0; i<sz; ++i) {idx()=i; m_histos[i]>>DumPush;	}				  
					break;
			}
			
		} //switch
	
	}
	static IdxTy &idx() { static IdxTy i=0; return i; }
	static Myt *& mud() { static Myt * unsafe_dum=0; return unsafe_dum; }
	static void DumPush(float *x)
	{ mud()->RawPush(x);}
	void RawPush(float * x)
	{
		float * own = new float[7];	
	 	for ( int ii=0; ii<7 ; ii++) own[ii]=x[ii];
		if (idx()!=0) for ( int ii=1; ii<4 ; ii++) own[ii]=x[ii]+idx()*50;
		p_list.push_back(own);
	}
	void LinePush(float * x)
	{
		float * own = new float[9];	
	 	for ( int ii=0; ii<9 ; ii++) own[ii]=x[ii];
		l_list.push_back(own);
	}
	void NormPush(float * x)
	{
		float * own = new float[10];	
	 	for ( int ii=0; ii<10 ; ii++) own[ii]=x[ii];
		n_list.push_back(own);
	}
	void ReDisplay()
	{data_display::display_stl(p_list); 
	data_display::display_lines(l_list);   
	data_display::display_normals(n_list);  
	//data_display::labels(m_text);
	glutPostRedisplay(); }
	IdxTy ProcCmd(const IdxTy len, Chr ** xx)
	{
		Chr * c=xx[0];
		if (strcmp(c,"LINE")==0)
		{	// line x0 y0 z0 xf yf zf t
			if (len<8 ) return 1;
			float z[9];
			myColor c(1.0f,1.0f,1.0f);
			if (m_color_stack.size()!=0) c=(m_color_stack.back());
			for (IdxTy i=0; i<6; ++i) z[i]=atof(xx[i+1]);
			z[6]=c.r; z[7]=c.g; z[8]=c.b;
			LinePush(z);
			
			return 0;
		}
		if (strcmp(c,"NORMAL")==0)
		{	// NORMAL x y z nx ny nz  r g b sz
			if (len<10 ) return 1;
			float z[10];
			//myColor c(1.0f,1.0f,1.0f);
			//if (m_color_stack.size()!=0) c=(m_color_stack.back());
			for (IdxTy i=0; i<10; ++i) z[i]=atof(xx[i+1]);
			//z[6]=c.r; z[7]=c.g; z[8]=c.b;
			NormPush(z);
			
			return 0;
		}
		if (strcmp(c,"PUSHCOLOR")==0)
		{	// pushcolor r g b
			if (len<4 ) return 1;
			m_color_stack.push_back(myColor(atof(xx[1]),atof(xx[2]),atof(xx[3])));
			return 0;
		}
		if (strcmp(c,"POPCOLOR")==0)
		{	// popcolor
			//if (len<4 ) return 1;
			if (m_color_stack.size()==0) return 1;
			m_color_stack.pop_back();
			return 0;
		}
		if (strcmp(c,"TEXT")==0)
		{	// text x y z scale words
			if (len<5 ) return 1;
			TextTy x;
			myColor& mc=m_color_stack.back();
			x.r=mc.r; x.g=mc.g; x.b=mc.b;
			x.x=atof(xx[1]); x.y=atof(xx[2]);x.z=atof(xx[3]); x.scale=atof(xx[4]); 
			for (IdxTy i=5; i<len; ++i){ x.text+=xx[i];x.text+=" "; }
			data_display::label_list.push_back(x);
			return 0;
		}
		if (strcmp(c,"CLEAR")==0)
		{	// clear
			m_color_stack.clear();
			ClrModel(); 
			return 0;
		}
		return 0;
	}
	IdxTy PushFromModel(const IdxTy len, Chr ** xx)
	{
		if (len<1) return 1;
	//	std::cerr<<__FILE__<<" "<<__LINE__<<'\n';
		if (xx[0][0]>'9' ) return ProcCmd(len,xx);
		float  x[len];
		for ( IdxTy i=0; i<len; ++i) x[i]=atof(xx[i]); 
		
		// yes, I no what a virtual is
		switch (m_handler)
		{
			// raw data type
			case 0:{ 
				if (len<7) return 1;
				RawPush(x);
				
				return 0; 	
			}
			// bunch of histograms 
			case 1:{ 
					if (len>IdxTy(m_histos.size())) 
					{	IdxTy dl=len-m_histos.size(); 
						while (dl>0) {m_histos.push_back(HistoTy(10,250));  
						IdxTy col=m_histos.size(); m_histos.back().color(1,1.0f*(col%3)/3,1);
						--dl;}  
					}
					for (IdxTy i=0; i<len; ++i) m_histos[i].add_point(x[i]);
					 
					return 0; 	
			}
			case 2:{ 
					if (len<3) return -1;
					IdxTy lx=atoi(xx[0]);
					IdxTy y=atoi(xx[1]);
					GValTy gv;
					for (IdxTy i=2; i<len; ++i) gv.push_back(x[i]);
					m_zed[GridTy(lx,y)]=gv;
					return 0; 	
			}
			
		} //switch
	
		return -1; 
	}

	void ClrModel()
	{
		clear_list(p_list); clear_list(l_list); 
		//IdleListTy::iterator ii=p_list.begin(),ie=p_list.end(); while (ii!=ie) delete *(ii++); p_list.clear();
	}
	void TrimModel(int sz)
	{
		IdleListTy::iterator ii=p_list.begin(),ie=p_list.end();
		int s=0;
		int szl=p_list.size();
		while (s<(szl-sz)){++s;  delete *(ii); p_list.erase(ii);   ii=p_list.begin();   }
		//p_list.clear();
	}
	template <class Ty> int Config(const Ty & co)
	{
		co.get(St("graphic"),m_type);
		if (m_type=="histo") m_handler=1;
		if (m_type=="3d") m_handler=2;  
		
		co.get(St("cutoff"),m_length);
		co.get(St("history"),m_hist_len);
		return 0; 
	}
	
	St Ldr(const IdxTy i) const {St x="    "; IdxTy j=i; while( 0!=(j--)) x+=" "; return x;} St Trl() const { return "\n"; }
	template <class Os> void Help(Os&os)
	{
		os<<Ldr(0)<<"-graphic {raw,histo}:display type  " <<Trl();
		os<<Ldr(1)<<"raw:each line is 7 values for radius(1), location(3),and color(3)"<<Trl();
		os<<Ldr(2)<<"histo: make a histogram for each element on a line"<<Trl();
		os<<Ldr(2)<<"3d: lines consiste of x,y ints followed by z and optional color"<<Trl();
	}
	
	// point display list
	typedef list<float*> IdleListTy;
	void clear_list(IdleListTy & li)
	{IdleListTy::iterator ii=li.begin(),ie=li.end();
		while (ii!=ie) delete *(ii++); li.clear();}
	
	IdleListTy p_list,l_list,n_list;
//	TeLiTy m_text;
//	HistoTy histo(10,250),histoxx(10,250),histon(10,250);
	Hv m_histos; 
	IdxTy m_length; // number of prior data points to keep in list
	St m_type;
	IdxTy m_handler; // no, I have never heard of a virtual function....
	ZedTy m_zed;
	IdxTy m_hist_len;
	ColorStack m_color_stack;
	
	
};


#endif
