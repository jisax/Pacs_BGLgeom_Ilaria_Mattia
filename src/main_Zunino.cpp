/*!
	@file main_Zunino.cpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Source code for Zunino example.
	@detail
*/

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

//#include "io_graph.hpp"
//#include "generic_point.hpp"
//#include "maximum_flow.hpp"
//#include "compute_euclidean_distance.hpp"
//#include "topological_distance.hpp"
//#include "reader_Zunino_class.hpp"
//#include "Zunino_edge_property.hpp"
//#include "generic_edge_geometry.hpp"
//#include "disjoint_components.hpp"
//#include "dijkstra.hpp"
#include "new_reader_Zunino.hpp"
#include "graph_builder.hpp"

using namespace boost;

int main(){

	typedef adjacency_list<vecS,vecS,directedS,Zunino_vertex_data,Zunino_edge_data > Graph;
	typedef graph_traits<Graph> Traits;
	typedef Traits::edge_descriptor Edge_desc;
	typedef Traits::edge_iterator Edge_iter;
	typedef Traits::vertex_descriptor Vertex_desc;
	typedef Traits::vertex_iterator Vertex_iter;	 

	std::string filename("../data/rattm93a.txt");
	//unsigned int dummy_lines = 2;

	Graph G;
	Zunino_reader<Graph, Zunino_source_data, Zunino_target_data, Zunino_edge_data, Zunino_topological_data> R(filename);
	Zunino_topological_data Topo;
	Zunino_source_data S;
	Zunino_target_data T;
	Zunino_edge_data E;
	Edge_desc e;
	bool inserted;
	
	//reading data
	R.ignore_dummy_lines(2);
	while(!R.is_eof()){
		R.read_line();
		R.get_data_from_line();
		Topo = R.get_topological_data();
		S = R.get_source_data();
		T = R.get_target_data();
		E = R.get_edge_data();
		std::tie(e, inserted) = add_edge(Topo.src, Topo.tgt, G);
		give_edge_properties<Graph, Zunino_edge_data>(E, e, G); 
		give_source_properties<Graph, Zunino_source_data>(S, Topo.src, G);
		give_target_properties<Graph, Zunino_target_data>(T, Topo.tgt, G);		
	}	//while
	
	Vertex_iter v_it, v_end;
	for(std::tie(v_it, v_end) = vertices(G); v_it != v_end; ++v_it)
		std::cout << *v_it << " : " G[*v_it].SRC << std::endl;
	
	Edge_iter e_it, e_end;
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it)
		std::cout << *e_it << " : " G[*e_it].capacity << std::endl;
	
	//maximum_flow:
	
/*	std::map<Edge_desc, double> out_residual_capacity;
	double out_max_flow;
	Traits::vertex_descriptor s = 2;	//da leggere da qualche parte in qualche modo.
	Traits::vertex_descriptor t = 14;
	out_max_flow = maximum_flow<Graph, Edge_desc>(G,s,t,out_residual_capacity);

	std::cout << "Abbiamo ottenuto: " << out_max_flow << std::endl;

	Edge_iter e_it, e_end;  // print residual capacities
	std::cout << "Residual capacities:" << std::endl;
	for(std::tie(e_it, e_end) = edges(G); e_it != e_end; ++e_it){
		std::cout << *e_it << " " << out_residual_capacity[*e_it] << std::endl;
	}
	
	/*
	
	double d12 = compute_euclidean_distance<Graph>(1,2,G);
	double d45 = compute_euclidean_distance<Graph>(4,5,G);
	double d13 = compute_euclidean_distance<Graph>(1,3,G);

	std::cout << "eudlidean distance 1_2 = " << d12 << std::endl;
	std::cout << "eudlidean distance 4_5 = " << d45 << std::endl;
	std::cout << "eudlidean distance 1_3 = " << d13 << std::endl;
	*/

	//prova algoritmo su componenti sconnesse:
	/*
	typedef typename std::map<unsigned int, std::list<Vertex_desc> > Components_map_t;
	Components_map_t comp_map;	
	std::cout << "Prova disjoint:" << std::endl;
	disjoint_components<Graph>(G); 
	*/
	
	
	/*
	std::cout << "Graph characteristics:" << std::endl;
	std::cout << num_vertices(G) << " vertices and " << num_edges(G) << " edges."<< std::endl << std::endl;
	
	// prova Dijkstra shortest paths
	Vertex_desc src = 1;
	std::vector<int> distances(num_vertices(G));
	std::vector<Vertex_desc> predecessors(num_vertices(G));
	
	dijkstra(G, src, distances, predecessors);
	
	std::cout << "Predecessor map: " << std::endl;
	for (int i=0; i < predecessors.size(); ++i){
		std::cout << i << " <- " << predecessors[i] << std::endl;
	}
	std::cout << std::endl;
	
	// output the shortest path from source to any other vertex
	using path_t = std::vector<Edge_desc>;
	path_t path;
	
	Vertex_desc u, end_vertex, v;
	Vertex_iter v_it, v_end; 
	std::pair<Edge_desc,bool> edge_pair;
	bool path_exists;
	
	Edge_desc e_tmp;
	

	for(std::tie(v_it, v_end) = vertices(G); v_it != v_end; ++v_it){
		end_vertex = v = *v_it;
		path_exists = true;
		path.clear(); // we are considering a new path
		u = predecessors[v];
		
		do{
			if(u != v){
				path.push_back(edge(u,v,G).first);
				v = u;
				u = predecessors[v];
			}
			else{
				if(u != src){
					std::cout << "There's no (directed) path between " << src << " and " << end_vertex << std::endl;					
				}
				path_exists = false;
				break; 
			}
		} 
		while(v != src);
		
		if (path_exists){
			std::cout << "Shortest path from " << src << " to " << end_vertex << std::endl;
			for(path_t::reverse_iterator riter = path.rbegin(); riter != path.rend(); ++riter){
				e_tmp = *riter;
				std::cout << source(e_tmp, G) << " -> " << target(e_tmp, G) << " (weight: " <<  G[e_tmp].length << ")" << std::endl;
			} //for
				 
		} //if

	} //for
	*/
	return 0;
}
