#include <cmath>
#include <complex>
#include <algorithm>

#if defined CHRONO
#include <chrono>
#endif

#include "function_to_learn_cppn.hpp"

#if defined CHRONO or DEBUG
static bool first = true;
#endif

inline double cubic_tanh( double x ) { return std::tanh( std::pow( x, 3 ) ); }
inline double sigmoid( double x ) { return 1. / ( 1 + std::exp( -x ) ); }
inline double gaussian( double x ) { return std::exp( std::pow( ( x - 1 ), 2 ) / -2 ); }

void interpreter( int number, const vector<double>& inputs, vector<double>& outputs )
{
	switch( number )
	{
		// Identity
	case 0:
		copy( inputs.begin(), inputs.end(), outputs.begin() );
		break;
		// Absolute value
	case 1:
		transform( inputs.begin(), inputs.end(), outputs.begin(), [](auto x) -> double { return std::abs(x); } );
		break;
		// Sine
	case 2:
		transform( inputs.begin(), inputs.end(), outputs.begin(), [](auto x) -> double { return std::sin(x); } );
		break;
		// Tanh
	case 3:
		transform( inputs.begin(), inputs.end(), outputs.begin(), [](auto x) -> double { return std::tanh(x); } );
		break;
		// Cubic Tanh
	case 4:
		transform( inputs.begin(), inputs.end(), outputs.begin(), cubic_tanh );
		break;
		// Sigmoid
	case 5:
		transform( inputs.begin(), inputs.end(), outputs.begin(), sigmoid );
		break;
		// Gaussian
	case 6:
		transform( inputs.begin(), inputs.end(), outputs.begin(), gaussian );
		break;
	}
}

void compute( int LO, const vector<double>& inputs, const vector<int>& weights, vector<double>& result )
{
	int L = LO / 10;
	int O = LO % 10;

	auto inputs_size = inputs.size();
	
	vector<double> temp_inputs( inputs_size );
	vector<double> temp_outputs( inputs_size );
	vector<double> temp_result( inputs_size );

	if( weights[ ( L - 1 ) * 7 + O ] != 1 )
		std::fill( result.begin(), result.end(), 0.0 );
	else
	{
		std::copy( inputs.begin(), inputs.end(), temp_inputs.begin() );
		
		for( int l = 1; l < L; ++l )
		{			
			std::fill( temp_outputs.begin(), temp_outputs.end(), 0.0 );
			for( int i = 0; i <= 6; ++i )
			{
				if( weights[ ( l - 1 ) * 7 + i ] == 1 )
				{
					interpreter( i, temp_inputs, temp_result );
					for( int j = 0; j < (int)inputs.size(); ++j )
						temp_outputs[j] += temp_result[j];
				}
			}
			std::copy( temp_outputs.begin(), temp_outputs.end(), temp_inputs.begin() );
		}

		interpreter( O, temp_inputs, result );
	}
}

double intermediate_g( const vector<int>& weights, const vector<double>& inputs, int nb_vars )
{
	int LO = ( weights.size() / 7 ) * 10 + 1;
	vector<double> result( inputs.size() );
	int number_units_last_layer = std::count( weights.begin() + 7, weights.begin() + 14, 1 );
	double max_cost = nb_vars + 0.9;
	
	compute( LO, inputs, weights, result );
		
	return max_cost * ( std::accumulate( result.begin(), result.end(), 0.0 ) / ( nb_vars * number_units_last_layer ) );
}

// ref_wrapper<Variable> version
double g( const vector< reference_wrapper<Variable> >& weights, const vector<int>& vars )
{
	int nb_vars = (int)vars.size();
	vector<double> inputs( vars.size() );
	vector<int> weights_int( weights.size() + 7 );
	std::copy( vars.begin(), vars.end(), inputs.begin() );
	std::transform( weights.begin(), weights.end(), weights_int.begin(), [&]( auto w ){ return w.get().get_value(); } );
	std::fill( weights_int.begin() + weights.size(), weights_int.end(), 0 );
	weights_int[ weights.size() + 1 ] = 1;

	return intermediate_g( weights_int, inputs, nb_vars );
}

// Variable version
double g( const vector< Variable >& weights, const vector<int>& vars )
{
	int nb_vars = (int)vars.size();
	vector<double> inputs( vars.size() );
	vector<int> weights_int( weights.size() + 7 );
	std::copy( vars.begin(), vars.end(), inputs.begin() );
	std::transform( weights.begin(), weights.end(), weights_int.begin(), [&]( auto w ){ return w.get_value(); } );
	std::fill( weights_int.begin() + weights.size(), weights_int.end(), 0 );
	weights_int[ weights.size() + 1 ] = 1;

	return intermediate_g( weights_int, inputs, nb_vars );
}

// Flat vector version
double g( const vector< reference_wrapper<Variable> >& weights, const vector<int>& vars, int start, int end )
{
	int nb_vars = end - start;
	vector<double> inputs( nb_vars );
	vector<int> weights_int( weights.size() + 7 );
	std::copy( vars.begin() + start, vars.begin() + start + end, inputs.begin() );
	std::transform( weights.begin(), weights.end(), weights_int.begin(), [&]( auto w ){ return w.get().get_value(); } );
	std::fill( weights_int.begin() + weights.size(), weights_int.end(), 0 );
	weights_int[ weights.size() + 1 ] = 1;

	return intermediate_g( weights_int, inputs, nb_vars );
}

// Int vector version
double g( const vector< int >& weights, const vector<double>& vars, int start, int end )
{
	return intermediate_g( weights, vars, end - start );
}
