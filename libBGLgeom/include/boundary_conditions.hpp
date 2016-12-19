/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file boundary_condition.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Data structure for boundary condition on a vertex
	@detail 
*/

#ifndef HH_BOUNDARY_CONDITION_HH
#define HH_BOUNDARY_CONDITION_HH

#include <iostream>
#include <iomanip>
#include <array>
#include <string>

//! Overload of operator= for assignments when there is only 1 BC.
/*
namespace{

template <typename T>
double std::array<T,1>::operator=(double const& val){
	return val;
}


}	//namespace

*/

namespace BGLgeom{
/*!
	@brief An enum class defining the type of the boundary condition we want to add in the node
	@detail The types of boundary conditions are: \n
			- NONE: the vertex doesn't contain a boundary condition; \n
			- INT: Stands for "Internal". Thougth to be used in contiguous vertices of the graph. No value (zero default). \n
			- DIR: Dirichlet boundary condition; \n
			- NEU: Neumann boundary condition; \n
			- MIX: Mixed boundary condition (for instance Robin or similar); \n
			- OTHER: A boundary condition of type different from all the previous ones
*/
enum class BC_type {NONE, INT, DIR, NEU, MIX, OTHER};

/*!
	@brief The struct defining a general boundary condition
	@detail It contains the type of the boundary condition (one among those defined
			in BC_t) and the value associated to it.
	@param Value_t Defaulted to double. It is templated because one may need to use
					boundary conditions in which are needed more than one associated
					value, for example:
					\n
					The user may use a vector or an array to rapresent this
*/
template <unsigned int num_bc = 1>
struct boundary_condition{
	// Definition of types:
	//using BC_t = typename BGLgeom::BC_type;
	//using value_type = Value_t;
		
	//! Type of the boundary condition
	std::array<BC_type, num_bc> type;
	//! The value associated to the boundary condition
	std::array<double, num_bc> value;
	
	//! Default constructor
	boundary_condition() : type(), value() {
		type.fill(BC_type::NONE);
		value.fill(.0);
	}
	
	//! Constructor
	boundary_condition(BC_type _type, std::array<double,num_bc> _value) : type(_type), value(_value) {};

	//! Copy constructor
	boundary_condition(boundary_condition const&) = default;
	
	//! Move constructor
	boundary_condition(boundary_condition &&) = default;
		
	//! Assignment operator
	boundary_condition & operator=(boundary_condition const&) = default;
	
	//! Move assignment
	boundary_condition & operator=(boundary_condition &&) = default;
	
	//! Setting methods
	void set_BC_type(BC_type const& _type, std::size_t const& i){ type[i] = _type; }
	void set_BC_value(double const& _value, std::size_t const& i){ value[i] = _value; }
	
	//! Getting methods
	std::array<BC_type,num_bc> get_type(std::size_t const& i) { return type[i]; }
	std::array<BC_type,num_bc> get_type(std::size_t const& i) const { return type[i]; }
	std::array<double,num_bc> get_value() { return value; }
	std::array<double,num_bc> get_value() const { return value; }	
	
	/*!
		@brief Overload of input operator
		@detail Reads input of the form: \n
				"BC_type" "value" "BC_type" "value" ...
	*/
	friend std::istream & operator>>(std::istream & in, boundary_condition & BC){
		std::string type;
		for(std::size_t i = 0; i < num_bc; ++i){
			in >> type;
			if(type == "NONE"){	// we suppose that there is a zero after the type. However, we check
				//{
					BC.type = BC_type::NONE;		
					in >> BC.value[i];
					if(BC.value[i] != .0)
						BC.value[i] = .0;
				//}
				//return in;
			} else if(type == "INT"){
				//for(std::size_t i = 0; i < num_bc; ++i){
					BC.type = BC_type::INT;
					in >> BC.value[i];
					if(BC.value[i] != .0)
						BC.value[i] = .0;
				//}
				//return in;
			} else if(type == "DIR"){
				//for(std::size_t i = 0; i < num_bc; ++i){
					BC.type = BC_type::DIR;
					in >> BC.value[i];
				//}
				//return in;
			} else if(type == "NEU"){
				//for(std::size_t i = 0; i < num_bc; ++i){
					BC.type = BC_type::NEU;
					in >> BC.value[i];
				//}
				//return in;
			} else if(type == "MIX"){
				//for(std::size_t i = 0; i < num_bc; ++i){
					BC.type = BC_type::MIX;
					in >> BC.value[i];
				//}
				//return in;
			} else if(type == "OTHER"){
				//for(std::size_t i = 0; i < num_bc; ++i){
					BC.type = BC_type::OTHER;
					in >> BC.value[i];
				//}
				//return in;
			}
		}	//for
		return in;
	}	//operator>>

	//! Overload of output operator
	friend std::ostream & operator<<(std::ostream & out, boundary_condition const& BC){
		for(std::size_t i = 0; i < num_bc; ++i){
			if(BC.type[i] == BC_type::NONE)
				out << "BC NONE";	//se faccio << BC.type stampa solo il numero. devo fare controlli per stampare stringa
			else if(BC.type[i] == BC_type::DIR){
				out << "BC DIR " << std::fixed << std::setprecision(8) << BC.value[i];
				//for(std::size_t i = 0; i < num_bc; ++i)
				//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
				return out;
			} else if(BC.type[i] == BC_type::NEU){
				out << "BC NEU " << std::fixed << std::setprecision(8) << BC.value[i];
				//for(std::size_t i = 0; i < num_bc; ++i)
				//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
				return out;
			} else if(BC.type[i] == BC_type::MIX){
				out << "BC MIX " << std::fixed << std::setprecision(8) << BC.value[i];
				//for(std::size_t i = 0; i < num_bc; ++i)
				//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
				return out;
			} else if(BC.type[i] == BC_type::OTHER){
				out << "BC OTHER " << std::fixed << std::setprecision(8) << BC.value[i];
				//for(std::size_t i = 0; i < num_bc; ++i)
				//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
				return out;
			} else if(BC.type[i] == BC_type::INT){
				out << "BC INT";
				//for(std::size_t i = 0; i < num_bc; ++i)
				//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
				return out;
			}
			out << " ";
		}	//for
	}	//operator<<
	
};	//boundary_condition

//! Helper function to write on pts file
template <unsigned int num_bc>
void
write_BC_pts(std::ostream & out, boundary_condition<num_bc> const& BC, std::size_t i){
	if(BC.type[i] == BC_type::NONE)
		out << "BC NONE";	//se faccio << BC.type stampa solo il numero. devo fare controlli per stampare stringa
	else if(BC.type[i] == BC_type::INT){
		out << "BC INT";
		//for(std::size_t i = 0; i < num_bc; ++i)
		//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
		//return out;
	} else if(BC.type[i] == BC_type::DIR){
		out << "BC DIR " << std::fixed << std::setprecision(8) << BC.value[i];
		//for(std::size_t i = 0; i < num_bc; ++i)
		//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
		//return out;
	} else if(BC.type[i] == BC_type::NEU){
		out << "BC NEU " << std::fixed << std::setprecision(8) << BC.value[i];
		//for(std::size_t i = 0; i < num_bc; ++i)
		//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
		//return out;
	} else if(BC.type[i] == BC_type::MIX){
		out << "BC MIX " << std::fixed << std::setprecision(8) << BC.value[i];
		//for(std::size_t i = 0; i < num_bc; ++i)
		//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
		//return out;
	} else if(BC.type[i] == BC_type::OTHER){
		out << "BC OTHER " << std::fixed << std::setprecision(8) << BC.value[i];
		//for(std::size_t i = 0; i < num_bc; ++i)
		//	out << std::fixed << std::setprecision(8) << BC.value[i] << " ";
		//return out;
	}
}	//wrtie_BC_pts

}	//BGLgeom
#endif	//HH_BOUNDARY_CONDITION_HH
