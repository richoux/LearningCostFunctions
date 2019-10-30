#include <algorithm>

#include "linear-eq_concept.hpp"

LinearEqConcept::LinearEqConcept( int nb_vars, int max_value, int rhs )
	: Concept( nb_vars, max_value ),
	  _rhs( rhs )
{ }

LinearEqConcept::LinearEqConcept( int rhs )
	: Concept( 0, 0 ),
	  _rhs( rhs )	  
{ }

bool LinearEqConcept::concept( const vector<int>& var, int start, int end ) const
{
	// '+ ( end - start )' is equivalent to add 1 to the value of each variable.
	return accumulate( var.begin() + start, var.begin() + end, 0 ) + ( end - start ) == _rhs;
}

bool LinearEqConcept::concept( const vector< reference_wrapper<Variable> >& var ) const
{
	int sum = 0;
	
	for( auto& v : var )
		sum += v.get().get_value() + 1;
	
	return sum == _rhs;	
}