#ifndef NOISY_SIGNAL_SOURCE_H__
#define NOISY_SIGNAL_SOURCE_H__

//#include "math/FastNorm3.h"
//void run_model() 
//{ if (!mmm) initnorm();	 for (int i=0; i<10000; ++i) histo.add_point(FastNorm);
//histo>> push_from_model; mmm+=1; }

template < class Ty> class noisy_signal_source
{

	typedef noisy_signal_source Myt;
	
	typedef Ty mytype;
	typedef double randTy;
	//typedef double mytype;
	typedef mytype x_type;
	typedef mytype y_type;
	typedef int index_type;
	typedef long unsigned int history_index_type;
	typedef double noise_type;
public:

noisy_signal_source() :m_f1(& Myt::line1),m_f2(&Myt::line2), m_hyst(false),m_state(0),m_cutoff(.5f),
m_offset(400),m_noise_x(0),m_noise_y(0),
m_x(0),m_x_nominal(0), m_y(0),m_y_nominal(0),m_pos(0) {}
Myt &  time_reset() { m_pos=0; return *this; }
Myt &  x_noise(const x_type nx) { m_noise_x=nx; return * this;}
Myt & offset(const y_type ny) { m_offset=ny; return *this; }
x_type x() const { return m_x;}
x_type x_() const { return m_x_nominal; }
y_type y() const {  return m_y; }
y_type y_() const { return m_y_nominal; }

static noise_type myrand(int seed) { return noise_type(rand())/RAND_MAX; }
//float t=i+myrand(3);
index_type State() 
{   bool f1=(myrand(0)<m_cutoff); bool f2=f1&&!m_hyst||m_hyst&&f1&&!m_state||m_hyst&&!f1&&m_state; 
	m_state=f2?1:0; return m_state;   }
void Inc()
{
	m_x_nominal=x_type(m_pos); m_x=m_x_nominal+m_noise_x*x_type(myrand(0));
	float xx=m_x;
	y_type y= //( myrand(0) < m_cutoff) ? 
	 State()? ((*this).*m_f1)(xx): ((*this).*m_f2)(xx);
	m_y_nominal=y; m_y=y+m_noise_y*y_type(myrand(0));
m_pos++;
}
mytype line1(float p) { return (p*1.0-p*p/1000*0)*(1+myrand(0)*0); }
mytype line2(float p) { return (p*1+m_offset)*(1+myrand(0)*0); }


mytype (Myt:: *m_f1)(float);// =line1;
mytype (Myt:: *m_f2)(float);// =line2;
bool m_hyst;
index_type m_state;
mytype m_cutoff,m_offset,m_noise_x,m_noise_y;
x_type m_x,m_x_nominal;
y_type m_y,m_y_nominal;
index_type m_pos;
};

#endif

