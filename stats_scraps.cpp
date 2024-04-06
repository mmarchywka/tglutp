/*
	IdxTy runxxx()
	{	IdxTy iteration=0,mod_iteration=0, ok_iterations=0;;
		while (true)
		{
		typedef std::vector<TiTy> Gr;
		Gr g1;
		const IdxTy n1=m_n1,n2=m_n2;
		TiTy dell=1e-6;
		for(IdxTy i=0; i<n1; ++i) g1.push_back(m_1*(*m_p1)(myrand(0))+dell);
		//std::sort(g1.begin(),g1.end());
		for(IdxTy i=0; i<n2; ++i) g1.push_back(-m_2*(*m_p2)(myrand(0))-dell);
		std::sort(g1.begin(),g1.end(),absss());
		TiTy last=0; IdxTy j1=n1,j2=n2;
		const TiTy d1=1.0/n1, d2=1.0/n2;
		TiTy a1=0,a2=0,a3=0,dx1=1,dx2=1;
		IdxTy nlim=n1+n2;
		if ((nlim>m_events)&&(m_events!=0)) nlim=m_events;
		for (IdxTy i=0; i<nlim; ++i)
		{
			TiTy d=g1[i];
			if ( d>0) 
			{ 	TiTy dt=d-last;  a1+=dt*dx1; a2+=dt*dx2; a3+=dt*(dx1-dx2);  
				last=d;   --j1; dx1-=d1;}
			else 
			{ 	TiTy dt=-d-last;  a1+=dt*dx1; a2+=dt*dx2; a3+=dt*(dx1-dx2);  
				last =-d;  --j2; dx2-=d2;}
		//	dos()<<" x"<<j1<<" "<<j2<<'\n';	
	}
		TiTy a4=a3*2/(a1+a2)*1000;
		if (a4>m_ok_limit) ++ok_iterations;
		if (m_display_interval==mod_iteration) 
		{mod_iteration=0; dos()<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<" "<<(1.0f*ok_iterations/iteration) <<'\n'; }
		else ++mod_iteration;
		++iteration; 
	}
	return 0;
}
*/
//class absss {public: bool operator()(const double & a, const double &b) const
//{ return (fabs(a)<fabs(b));}};
//class abpair {public: template < class Ty> bool operator()(const Ty & a, const Ty &b) const
//{ return (a.second<b.second);}};

/*		const IdxTy n1=m_n1,n2=m_n2;
		TiTy dell=1e-6;
		if (m_p1==&Myt::uniform) this->iterate<GrTy,&Myt::uniform>(g,1,n1,m_1,dell);
		else if (m_p1==&Myt::rexp) this->iterate<GrTy,&Myt::rexp>(g,1,n1,m_1,dell);
		if (m_p2==&Myt::uniform) this->iterate<GrTy,&Myt::uniform>(g,2,n2,m_2,dell);
		else if (m_p2==&Myt::rexp) this->iterate<GrTy,&Myt::rexp>(g,2,n2,m_2,dell);
		//this->iterate<GrTy,&Myt::uniform>(g,2,n2,m_2,dell);
	*/	