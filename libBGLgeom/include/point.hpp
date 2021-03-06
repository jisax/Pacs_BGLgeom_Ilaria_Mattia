/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*!
	@file	point.hpp
	@author	Ilaria Speranza and Mattia Tantardini
	@date	Jan, 2017
	@brief	Definition of the type used to represent points in this 
			library, and definition of some utilities for points
*/

#ifndef HH_POINT_HH
#define HH_POINT_HH

#include <iostream>
#include <iomanip>
#include <Eigen/Dense>
#include <limits>

#ifndef TOL
#define TOL 20*std::numeric_limits<double>::epsilon()
#endif

namespace BGLgeom{

//! Alias template for a point in N-th dimensional space, using Eigen::Matrix
template <unsigned int N>
using point = Eigen::Matrix<double,1,N>;

//! Overload of the input operator for Eigen Matrices
template <typename Derived>
std::istream &
operator>>(std::istream & in, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < static_cast<unsigned int>(m.rows()); ++i){
		for(std::size_t j = 0; j < static_cast<unsigned int>(m.cols()); ++j){
		    in >> m(i,j); 
	   }
	}
	return in;
}

//! Overload of the output operator for Eigen Matrices
template <typename Derived>
std::ostream &
operator<<(std::ostream & out, Eigen::DenseBase<Derived> & m){
	for(std::size_t i = 0; i < static_cast<unsigned int>(m.rows()); ++i){
		for(std::size_t j = 0; j < static_cast<unsigned int>(m.cols())-1; ++j){
		    out << m(i,j) << " "; 
	   }
	}
	out << m(m.rows()-1,m.cols()-1);
	return out;
}

/*!
	@brief Operator< overloading. Implements lexicographic order		

	Point1 < Point2 if Point1.x is smaller than Point2.x; if they are 
	equal, compare in the same way the y coordinate, and so on.

	@return True if Point1 comes "first" w.r.t. to this ordering, false 
			otherwise
*/
template <typename Derived>
bool
operator< (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){							
	for(std::size_t i = 0; i < static_cast<unsigned int>(P1.cols()); ++i){
		if(P1(0,i) < P2(0,i))
			return true;
		else if (P1(0,i) > P2(0,i))
			return false;
	}			
	return false;		//if they are equal	
}

/*!
	@brief Operator> overloading
				
	It is the negation of operator<
*/
template <typename Derived>
bool
operator> (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){							
	return !(operator<(P1,P2));	
}

/*!
	@brief Operator== overloading		
		
	It checks if all the coordinates are equal, with a default tolerance
*/
template <typename Derived>
bool
operator== (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){
	return (P1-P2).norm() < TOL;
}

/*!
	@brief Operator!= overloading	
			
	It checks if the two points are different, with a default tolerance.
	It is implemented as the negation of the operator==
*/
template <typename Derived>
bool
operator!= (Eigen::DenseBase<Derived> const& P1, Eigen::DenseBase<Derived> const& P2){
	return !(P1==P2);
}

} //BGLgeom

#endif //HH_GENERIC_POINT_HH
