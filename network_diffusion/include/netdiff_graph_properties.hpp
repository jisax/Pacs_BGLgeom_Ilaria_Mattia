/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	netdiff_graph_properties.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date 	Jan, 2017
	@brief	Properties that the graph for diffusion on network appliocation will have
	
	All the data structures have to be provided with at least 
	the default constructor.
*/

#ifndef HH_NETDIFF_GRAPH_PROPERTIES_HH
#define HH_NETDIFF_GRAPH_PROPERTIES_HH

#include "data_structure.hpp"
#include "linear_geometry.hpp"

namespace NetDiff{

//! Properties that vertices will have
using Vertex_prop = BGLgeom::Vertex_base_property<3>;

using Edge_prop = BGLgeom::Edge_base_property< BGLgeom::linear_geometry<3>, 3>;
}	//NetDiff

#endif	//HH_NETDIFF_GRAPH_PROPERTIES_HH
