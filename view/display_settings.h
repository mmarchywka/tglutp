#ifndef DISPLAY_SETTINGS_H__
#define DISPLAY_SETTINGS_H__

#include "../data_and_config.h"
#include "../base/points_base_classes.h"

class display_tr
{
	public:
	typedef data_and_config DaCoTy;
	typedef DaCoTy::Ch Ch;
	typedef DaCoTy::string_type string_type;
	typedef DaCoTy::name_type name_type;
	typedef DaCoTy::ConvStreamTy ConvStreamTy;
	typedef DaCoTy::OsStreamTy OsStreamTy;
	typedef unsigned int index_type;
	typedef int count_type;
	typedef int flag_type;
	typedef float coord_type;
	typedef int disp_attr_type;
	typedef point3_template<coord_type> point_type;
	typedef myPoint4<coord_type> p_plus_type;
	//typedef map<name_type,display_tree> display_list;
};

class display_settings : public display_tr
{	typedef display_settings Myt;
public:
	display_settings(): m_hidden(false),m_picking(false), m_use_flags(-1),m_color(),m_tint(),
	 m_clip_plane(), m_thick(3),
	m_size(3),m_dia(1),m_style(0),m_substyle(0),m_blend(0),m_light(1) {}

count_type light() const { return m_light;}	
count_type blend() const { return m_blend;}	
count_type style() const { return m_style; }
count_type substyle() const { return m_substyle; }
coord_type dia() const { return m_dia;}
bool picking() const { return m_picking; }
bool hidden() const { return m_hidden; }
	enum {USE_BITS=6, USE_COLOR=1, USE_CLIP=2, USE_THICK=4, USE_SIZE=8, USE_STYLE=16,USE_DIA=32};
	typedef void *(Myt:: *field_ptr);

	static const field_ptr* fields()
	{static const field_ptr  fields[]=
	{ field_ptr(&Myt::m_color),field_ptr(&Myt::m_clip_plane),
	 field_ptr(&Myt::m_thick),field_ptr(&Myt::m_size),field_ptr(&Myt::m_style),
	 field_ptr(&Myt::m_dia)};
	return fields; 
	}
	static const Ch** use_bits()
	{static const Ch* use_bits[]={"COLOR", "CLIP", "THICK", "SIZE","STYLE","DIA" };
	return use_bits; }
template <class Tx,class Ty> bool set_always(const Tx ( Myt:: *thing) , Ty & val) const
	{ val=Ty(this->*thing); return true; }
		
template <class Tx,class Ty> bool set_from (const Tx ( Myt:: *thing) , Ty & val) const
	{ count_type i=0; 
	flag_type uf=m_use_flags;
	while(i<USE_BITS) 
	{ if(((uf&1)!=0)&&(field_ptr(thing)==fields()[i])) 
		{val=Ty(this->*thing); return true; }
	 i++;  uf>>=1;}
	 	return false;
	}
	template <class Tx,class Ty> bool offset_always (const Tx ( Myt:: *thing) , Ty & val) const
	{ val+=Ty(this->*thing); return true;}
template <class Ty,class Tx>
	bool sset(const string_type & nm, Ty& os,const count_type prop, Tx & oss)
{// eh, I really don't care how slow this is...
	if (nm=="hide") {os>>m_hidden; oss<< m_hidden;}
	else if (nm=="pick") {os>>m_picking; oss<< m_picking;}
	else if (nm=="flags") {os>>m_use_flags; oss<< m_use_flags;}
	else if (nm=="thick"){ os>>m_thick;oss<< m_thick;}
	else if (nm=="size") {os>>m_size;oss<< m_size;}
	else if (nm=="style"){ os>>m_style;oss<< m_style;}
	else if (nm=="substyle"){ os>>m_substyle;oss<< m_substyle;}
	else if (nm=="blend"){ os>>m_blend;oss<< m_blend;}
	else if (nm=="light"){ os>>m_light;oss<< m_light;}
	else if (nm=="tint"){ m_tint.load(os);m_tint.store(oss);}
	else if (nm=="dia") {os>>m_dia;oss<< m_dia;}
	else if (nm=="color") {m_color.load(os);m_color.store(oss) ;}
	else if (nm=="clip"){ m_clip_plane.load(os); m_clip_plane.store(oss);}
	else if (nm=="help")
	{
		std::cout<<"Options: pick,flags,thick,size,style,substyle,blend,light,tint,dia,color,clip\n";
		std::cout<<"flags- set bits uses settings rather than list default\n";
		std::cout<<"style- 0=disk, 1=sphere, 2=point\n";
		std::cout<<"substyle- style depedent details \n";
		std::cout<<"blend- 0 for opaque or \n";
		std::cout<<"stc: (0)GL_ZERO, (1)GL_ONE,(2)GL_DST_COLOR,(3) GL_ONE_MINUS_DST_COLOR,\n";
		std::cout<<"(4)GL_SRC_ALPHA,(5)GL_ONE_MINUS_SRC_ALPHA,(6) GL_DST_ALPHA,\n";
		std::cout<<"(7)GL_ONE_MINUS_DST_ALPHA,(8) GL_SRC_ALPHA_SATURATE\n";
		std::cout<<"dest: (0) GL_ZERO, (16)GL_ONE,\n";
		std::cout<<"(32)GL_SRC_COLOR, (48)GL_ONE_MINUS_SRC_COLOR, (64)GL_SRC_ALPHA,\n";
		std::cout<<"(80)  GL_ONE_MINUS_SRC_ALPHA, (96) GL_DST_ALPHA,\n";
		std::cout<<"(112) GL_ONE_MINUS_DST_ALPHA   \n";
		std::cout<<"\n";
		std::cout<<"\n";
		return false; 
		}
	else { std::cout<<"Set failed in settings with "
	<<nm<<".\nOptions: pick,flags,thick,size,style,substyle,blend,light,tint,dia,color,clip\n";
                  return false;   }
	return true;
}	
	//private:
	bool m_hidden,m_picking;
	flag_type m_use_flags;
	
	p_plus_type m_color,m_tint,m_clip_plane;
	coord_type m_thick,m_size,m_dia;	
	count_type m_style,m_substyle,m_blend,m_light;
	public:
template <class Ty> void dump(Ty & os, flag_type f, const Ch** nm, const count_type n )	const
{	for (count_type i=0; i<n; ++i){  if (f&1) { os<<string_type(nm[i])<<" "; }  f>>=1;}	
	if (f) { os<<" etc.";}
}	
template <class Ty> void dump(Ty & os)	const
{
	if (m_hidden) os<<" hidden ";
	if (m_use_flags)
	{    dump(os,m_use_flags,   use_bits(),USE_BITS);    }
	if (m_color) os<<string_type(m_color)<<" ";
	if (m_clip_plane) os<<string_type(m_clip_plane)<<" ";
	if (m_thick) os<<m_thick<<" ";
	if (m_size) os<<m_size<<" ";
	if (m_dia) os<<m_dia<<" ";
	if (m_style) os<<m_style<<"/"<<m_substyle<<" ";
	os<<"\n";
	}
};

#endif
