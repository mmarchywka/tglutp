#ifndef RADIAL_POTENTIALS_H__
#define RADIAL_POTENTIALS_H__

#include <map> // to pickup pair for defered ints...
#include <math.h>
// NOTE that the PARAMETER IS R^2 as this is the cheapest thing
// to compute initially... 
namespace radial_potentials
{
typedef std::pair<int,int> defered_int_type;

inline float one_over( const float R) { return 1.0f/R;};
	
inline float one_over_3( const float R) { return ::pow(R,-1.5);};
inline float one_over_4( const float R) { return 1.0f/(R*R);};
inline float one_well( const float R) { return (1.0f-R/2)/(R*R*R);};
	
};


#endif
