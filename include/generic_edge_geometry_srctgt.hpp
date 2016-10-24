/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file generic_edge_geometry_srctgt.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief edge geometry source&target dependent
	@detail 
*/

#ifndef HH_GENERIC_EDGE_GEOMETRY_HH
#define HH_GENERIC_EDGE_GEOMETRY_HH

#include<array>
#include<functional>
#include"generic_point.hpp"
#include "edge_geometry.hpp"

namespace BGLgeom{

template<unsigned int dim> // dim is the dimension of the space we are working in (2 or 3 in normal cases)
class
linear_edge_geometry: public BGLgeom::generic_edge_geometry<dim>
{
	private:
	
	std::function<BGLgeom::point<dim>(double)> value_fun;      //! stores the function x_i(s) = f_i(s), i=1:dim, s=0:1. f: R -> R
	

	public:
	
	//! constructor 
	generic_edge_geometry
	(std::function<BGLgeom::point<dim>(double)> value_) :
	value_fun(value_)
	{};
	
	//! default constructor: linear edge (oppure defaultizzo già il fatto di chiamare sempre il linear_edge se non altrimenti specificato?)
	generic_edge_geometry()
	{
		value_fun = [](double s) -> BGLgeom::point<dim> 
					{std::array<double,dim> coordinates(s);
					 coordinates.fill(s); //x(s)=s; y(s)=s; ...
					 BGLgeom::point<dim> p(coordinates);
					 return p;
					};
	};	
	
	//! first derivative
	virtual std::vector<double> 
	first_derivatives(const double x)
	{
		//reads data from a data file
		//GetPot   ifl("data.pot");
		//double h = ifl("h", 0.001);
		const double h = 0.001;
		std::cout<<"Spacing "<<h<<std::endl;
	
		// Point where derivative is computed
		std::cout<<"X value "<<x<<std::endl;

		double constexpr half(0.5);
		
		BGLgeom::point<dim> diff; //! declare the point that will contain the result
		
		// Compute finite difference depending on the value x +_ h
		if(x+h > 1)
			diff = (value_fun(x) - value_fun(x-h))/h;

		else if(x-h < 0)
			diff = (value_fun(x+h) - value_fun(x))/h;

		else 
			diff = half*(value_fun(x+h)-value_fun(x-h))/h;
		
		// Copy in a vector the coordinates of diff
		std::vector<double> dn(diff.coord, diff.coord + dim); 
		
		for(auto i: dn)
			std::cout << i << " ";
		std::cout << std::endl;
		
		return dn;	
	}	
	
	
	//! second derivative
	virtual	std::vector<double> 
	second_derivatives(const double x)
	{
		//reads data from a data file
		//GetPot   ifl("data.pot");
		//double h = ifl("h", 0.05);
		const double h = 0.001;
		std::cout<<"Spacing "<<h<<std::endl;
		
		// Point where derivative is computed
		std::cout<<"X value "<<x<<std::endl;

		double constexpr half(0.5);
		
		BGLgeom::point<dim> diff;
			
		// Compute finite difference depending on the value x +_ h
		if(x+h > 1)
			diff = (this->first_derivative_fun(x) - this->first_derivative_fun(x-h))/h;

		else if(x-h < 0)
			diff = (this->first_derivative_fun(x+h) - this->first_derivative_fun(x))/h;

		else 
			diff = half*(this->first_derivative_fun(x+h)-this->first_derivative_fun(x-h))/h;
			
		// Copy in a vector the coordinates of diff
		std::vector<double> dn(diff.coord, diff.coord + dim); 
			
		for(auto i: dn)
			std::cout << i << " ";
		std::cout << std::endl;
			
		return dn;	
	}
	

	//! curvilinear abscissa

    //! returns value fun (parametrized between 0 and 1) in s between 0 and 1 
	virtual BGLgeom::point<dim> value (const double parameter)
	{
		//check if param belongs to 0->1
		return value_fun(parameter);
	};
	
}; //class


} //namespace






#endif
