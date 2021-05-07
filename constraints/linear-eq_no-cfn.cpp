#include <cmath>
#include <algorithm>

#include "linear-eq_no-cfn.hpp"


LinearEq::LinearEq( const vector< reference_wrapper<Variable> >& variables, int max_value, int rhs )
	: Constraint( variables ),
	  _rhs( rhs ),
	  _le_concept_{ (int)variables.size(), max_value, rhs }
{ }

double LinearEq::required_error() const
{
	if( _le_concept_.concept_( variables ) )
		return 0.;
	else
		return 1.;
}
