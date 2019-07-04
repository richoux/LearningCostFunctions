#include <algorithm>

#include "increment.hpp"

void increment( const vector<reference_wrapper<Variable>> &variables,
                const int index )
{
	if( index < 0 )
		return;

	if( variables[ index ].get().get_value() < variables[ index ].get().get_domain_max_value() )
		variables[ index ].get().set_value( variables[ index ].get().get_value() + 1 );
	else
	{
		variables[ index ].get().set_value( 0 );
		increment( variables, index - 1 );
	}
}

void increment( const vector<reference_wrapper<Variable>> &variables )
{
	increment( variables, variables.size() - 1 );
}


void increment_some_vars( const vector<reference_wrapper<Variable>> &variables,
                          const vector<int> &vars_index,
                          const int index )
{
	if( index >= vars_index.size() )
		return;

	int current_var_index = vars_index[ index ];

	if( variables[ current_var_index ].get().get_value() < variables[ current_var_index ].get().get_domain_max_value() )
		variables[ current_var_index ].get().set_value( variables[ current_var_index ].get().get_value() + 1 );
	else
	{
		variables[ current_var_index ].get().set_value( 0 );
		increment_some_vars( variables, vars_index, index + 1 );
	}
}

void increment_some_vars( const vector<reference_wrapper<Variable>> &variables,
                          const vector<int> &vars_index )
{
	increment_some_vars( variables, vars_index, 0 );
}

void increment_boolean_vector( vector<bool> &vec,
                               const int index )
{
	if( std::all_of( vec.begin(), vec.end(), [](auto b){ return b; } ) )
		return;

	if( !vec[ index] )
		vec[ index ] = true;
	else
	{
		vec[ index ] = false;
		increment_boolean_vector( vec, index + 1 );
	}
}

void increment_boolean_vector( vector<bool> &vec )
{
	increment_boolean_vector( vec, 0 );
}
