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
	@file	test_writer_ASCII.cpp
	@author	Ilaria Speranza & Mattia Tantardini
	@date	Jan, 2017
	@brief	Testing class writer_ASCII to show how to produce an ASCII output
	
	Based on the graph properties and concrete writer defined in 
	test_writer_ASCII.hpp
*/

#include "test_writer_ASCII.hpp"
#include "base_properties.hpp"
#include "graph_builder.hpp"
#include "graph_access.hpp"
#include "point.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <string>

using namespace BGLgeom;

int main(){
	using Graph = boost::adjacency_list< boost::vecS, 
										 boost::vecS, 
										 boost::directedS, 
										 Vertex_base_property<2>, 
										 Edge_prop_ASCII>;
	Graph G;
	
	Vertex_desc<Graph> a, b, c;
	a = new_vertex(G);
	b = new_vertex(G);
	c = new_vertex(G);
	G[a].coordinates = point<2>(0,0);
	G[b].coordinates = point<2>(1,1);
	G[c].coordinates = point<2>(2,2);
	
	Edge_desc<Graph> e1, e2;
	e1 = new_linear_edge(a, b, G);
	e2 = new_linear_edge(b, c, G);
	
	G[e1].index = 1;
	G[e1].foo_double = 1.0123456789;
	G[e1].foo_vector.resize(4);
	G[e1].foo_vector[3] = 101;
	G[e1].foo_string = "foo.1";
	G[e2].index = 2;
	G[e2].foo_double = 2.987654321;
	G[e2].foo_vector.resize(4);
	G[e2].foo_vector[3] = 202;
	G[e2].foo_string = "foo.2";
	
	std::string filename = "../data/out_test_writer_ASCII.txt";
	test_writer_ASCII<Graph> W(filename);	// Putting headers
	W.put_headers({"Edge","SRC","TGT","foo_double","foo_vector[3]","foo_string"});
	Edge_iter<Graph> e_it, e_end;
	// Writing two lines corresponding to the two edges
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
		W.write_data(*e_it,G);
	}
	
	return 0;
}
