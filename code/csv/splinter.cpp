#include <istream>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <bspline_builders.h>
#include <stdlib.h>

using std::cout;
using std::endl;

using namespace SPLINTER;

std::vector<std::string> get_next_line_and_split_into_tokens(std::istream& str)
{
	std::vector<std::string>   result;
	std::string                line;
	std::getline(str,line);

	std::stringstream          lineStream(line);
	std::string                cell;

	while(std::getline(lineStream,cell, ','))
	{
		result.push_back(cell);
	}
	// This checks for a trailing comma with no data after it.
	if (!lineStream && cell.empty())
	{
		// If there was a trailing comma then add an empty element.
		result.push_back("");
	}
	return result;
}

int main(int argc, char *argv[])
{
	// Create new DataTable to manage samples
	DataTable samples;

	std::ifstream file("all-diff_4.csv");
	auto dummy = get_next_line_and_split_into_tokens( file );

	while( file.good() )
	{
		auto tokens = get_next_line_and_split_into_tokens( file );
		if( std::rand() % 100 < 10 )
		{
			std::vector<double> x;

			double y = std::stod( tokens[2] );
		
			for( auto it = tokens.begin() + 3 ; it < tokens.end() ; ++it )
				x.push_back( std::stod( *it ) );

			samples.add_sample( x, y );
		}
	}

	samples.add_sample( std::vector<double> {0,0,0,0}, 3.6);
	samples.add_sample( std::vector<double> {0,0,0,3}, 2.3);
	samples.add_sample( std::vector<double> {0,0,3,0}, 2.3);
	samples.add_sample( std::vector<double> {0,3,0,0}, 2.3);
	samples.add_sample( std::vector<double> {3,0,0,0}, 2.3);
	samples.add_sample( std::vector<double> {0,0,3,3}, 2.2);
	samples.add_sample( std::vector<double> {0,3,0,3}, 2.2);
	samples.add_sample( std::vector<double> {0,3,3,0}, 2.2);
	samples.add_sample( std::vector<double> {3,0,0,3}, 2.2);
	samples.add_sample( std::vector<double> {3,0,3,0}, 2.2);
	samples.add_sample( std::vector<double> {3,3,0,0}, 2.2);
	samples.add_sample( std::vector<double> {0,3,3,3}, 2.3);
	samples.add_sample( std::vector<double> {3,0,3,3}, 2.3);
	samples.add_sample( std::vector<double> {3,3,0,3}, 2.3);
	samples.add_sample( std::vector<double> {3,3,3,0}, 2.3);
	samples.add_sample( std::vector<double> {3,3,3,3}, 3.6);
	
	// Build B-splines that interpolate the samples
	BSpline bspline = bspline_interpolator(samples, 3);

	// Build degree 3 penalized B-spline (P-spline) that smooths the samples
	// The smoothing/regularization parameter is set to 0.03
	BSpline pspline = bspline_smoother(samples, 3, BSpline::Smoothing::PSPLINE, 0.03);

	/*
	 * Evaluate the splines at x = (1,1)
	 * NOTE1: the error will be 0 at that point (except for the P-spline, which may introduce an error
	 * in favor of a smooth approximation) because it is a point we sampled at.
	 * NOTE2: The BSpline::eval function returns an output vector (in this case of size 1)
	 */

	std::ifstream file2("all-diff_4.csv");
	dummy = get_next_line_and_split_into_tokens( file2 );
	
	while( file2.good() )
	{
		auto tokens = get_next_line_and_split_into_tokens( file2 );
		std::vector<double> x;

		double y = std::stod( tokens[2] );
		
		for( auto it = tokens.begin() + 3 ; it < tokens.end() ; ++it )
			x.push_back( std::stod( *it ) );

		auto y_bs = bspline.eval(x).at(0);
		auto y_ps = pspline.eval(x).at(0);

		cout << "Diff Quadratic B-spline at x:          " << y - y_bs              << endl;
		cout << "Diff P-spline at x:                 " << y - y_ps               << endl;
	}
	
	return 0;
}