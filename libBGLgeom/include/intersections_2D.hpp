/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file intersections_2D.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date Sept, 2016
	@brief Classes and functions to compute intersections between two linear edges
	
	@detail We need to define some additional functionality for std::array just because
			the storage type for the points in the library is an Eigen std::array<double,2>, while
			this code (provided by professor Formaggia) is suitable for std::array.
			That's why we provide also some interface classes to transform data in a
			format that can be processed by this code.
*/

#ifndef HH_INTERSECTIONS_2D_HH
#define HH_INTERSECTIONS_2D_HH

#include <array>
#include <limits>
#include <cmath>
#include <utility>
#include <tuple>
#include <iostream>

#include "linear_edge_geometry_srctgt.hpp"

//! Helper functions for the algorithms. Using Unnamed namespace
namespace {

//! Scalar product between two std::array<double,2>s
inline double dot(std::array<double,2> const& a, std::array<double,2> const& b){
	return a[0]*b[0] + a[1]*b[1];
}

//! Norm of a std::array<double,2>
inline double norm(std::array<double,2> const& a){
	return std::sqrt(dot(a,a));
}

//! Overload of operator+ for std::array<double,2>
inline std::array<double,2> operator+ (std::array<double,2> const& a, std::array<double,2> const& b){
	return std::array<double,2>{a[0]+b[0], a[1]+b[1]};
}

//! Overload of operator- for std::array<double,2>
inline std::array<double,2> operator- (std::array<double,2> const& a, std::array<double,2> const& b){
	return std::array<double,2>{a[0]-b[0], a[1]-b[1]};
}	

//Both the overloading needed!
//! Overloading of operator* to represent the multiplication of a std::array<double,2> for a scalar
inline std::array<double,2> operator* (double const& k, std::array<double,2> const& a){
	return std::array<double,2>{k*a[0], k*a[1]};
}

/*!
	@brief Solves Ax=b	
	
	@param A The matrix of the system
	@param b Vector of the known terms
	@param tol Tolerance on the determinant
	
	@return std::pair<bool,std::array<double,2>> The bool indicates if there is an intersection.
					In the array there are the parametric coordinates of the intersection
					(defaulted to (0,0)) if there is no intersection)
*/
std::pair<bool, std::array<double,2>> solve 	(std::array<std::array<double,2>,2> const& A,
        										std::array<double,2> const& b,	//qui prima il const& non c'era
        										double const& tol) {
	auto D = A[0][0]*A[1][1] - A[1][0]*A[0][1];	//determinant
	if (std::abs(D) <= tol)
	    return std::make_pair(false, std::array<double,2>{0,0});
	D=1./D;
	std::array<double,2> res;
	res[0] = D * (A[1][1]*b[0] - A[0][1]*b[1]);
	res[1] = D * (A[0][0]*b[1] - A[1][0]*b[0]);
	return std::make_pair(true, res);
}	//solve

}	//namespace

namespace BGLgeom{

/*! 
	@brief An interface for a linear edge
	@detail This class is thought to manage the description of the geometry
			of a linear edge, in order to compute intersections.
	@remarks The class must have an overload of operator[] in order to run in the 
				function that computes intersections
*/
class linear_edge_interface{
	public:
		//! Default constructor
		linear_edge_interface	(linear_edge_geometry_srctgt const& edge) : 
								 		extremes{ {edge.get_source()(0,0), edge.get_source()(0,1)},
								 				{edge.get_target()(0,0), edge.get_target()(0,1)} } {};
	
		//! Constructor
		//linear_edge_interface(BGLgeom::point<2> const& SRC, BGLgeom::point<2> const& TGT) : extremes{SRC, TGT}, extremes_are_set(true) {};
		
		//! Copy constructor
		linear_edge_interface(linear_edge_interface const&) = default;
		
		//! Assignment operator
		linear_edge_interface & operator=(linear_edge_interface const&) = default;
		
		/*
		//! Setting the two end points (extremes) of the edge
		void set(point<2> const& SRC, point<2> const& TGT){
			extremes[0] = SRC;
			extremes[1] = TGT;
			extremes_are_set = true;
		}
		*/
		/*! 
			@brief Overloading of operator[] to access each of the two end points. Usefull in algorithms
			@detail extremes[0] = source, extremes[1] = target of the edge
		*/
		std::array<double,2> operator[](std::size_t i){ return extremes[i]; }
		std::array<double,2> operator[](std::size_t i) const { return extremes[i]; }
				
	private:
		//! This container contains coordinates of source (first row), coordinates of target (second row)
		std::array<std::array<double,2>, 2> extremes;
};	//linear_edge_interface


//! A simple struct that contains the result of the intersection test
/*!
	To be able to treat the most general case each segment is allowed
	to have up to two intersections. It happens if the segments overlaps

	@todo It can be bettered by adding another attribute that indicates,
			in the case of two edges end which coincides the relative position on the 
			edge. It requires a simple modification of the function segmentIntersect
	@note Piece of code provided by prof. Formaggia
*/
struct Intersection {
	//! Segments interects! True if there is intersection
	bool intersect = false;
	//! Number of intersections (max 2, rapresenting the case in which the segments overlap)
	unsigned int numberOfIntersections = 0u;
	//! Intersection points coordinates
	std::array<point<2>,2> intersectionPoint = std::array<point<2>,2>{point<2>(), point<2>()};
	/*! Intersection may be end point:	    
	    endPointIsIntersection[i][j]=true
	    then end j of edge i is at the intersection
	*/
	std::array<std::array<bool,2>, 2> endPointIsIntersection =
	    std::array<std::array<bool,2>, 2>{std::array<bool,2>{false,false}, std::array<bool,2>{false,false} };
	/*! EdgeS join at the end.
	    In that case endPointIsIntersection will be true and the corresponding
	    entry will indicate the numering of the end of the other edge.
	    -1 indicates that the end is not joined. So
	    if endPointIsIntersection[i][j]=true we have
	    otherEdgePoint[i][j]=-1 //End point is not joined with the other edge
	    otherEdgePoint[i][j]= k //End point j of edge j is joined with end point k of other edge
	    */
	std::array<std::array<int,2>, 2> otherEdgePoint =
	    std::array<std::array<int,2>, 2>{std::array<int,2>{-1,-1}, std::array<int,2>{-1,-1}};
	//! Edges are parallel
	bool parallel = false;
	//! Edges are identical
	bool identical = false;
	//! Edges are collinear (and thus also parallel)
	bool collinear = false;
	//! Something is not ok
	bool good = true;
	//! Distance, makes sense only if parallel=true
	double distance = 0.0;
};	//Intersection


/*!
@brief Computes intersection betweeen two edges

It handles also the case of intersection at the segment ends
@note  It is not so rubust becouse it uses the tolerances in a 
different way: the tolerance tol to test the parametric coordinate
along the edge line and a scaled tolerance to check distances.
Another scaled tolerance is used to test ir edges are parallel.

@pre The edges must have non null length
@par S1 First Edge
@par S2 Second Edge
@par tol A tolerance,it should greater than epsilon for doubles
@return Intersection. A data structure containing the info about the intersection
*/
Intersection segmentIntersect	(linear_edge_geometry_srctgt const& S1,
								linear_edge_geometry_srctgt const& S2,
                           		double const
                            	tol=20*std::numeric_limits<double>::epsilon()) {
	Intersection out;
	auto v1= S1[1]-S1[0];
	auto v2= S2[1]-S2[0];
	auto len1 = norm(v1);
	auto len2 = norm(v2);
	// Tolerance for distances
	double tol_dist=tol*std::max(len1,len2);
	// I need to scale the tolerance to check parallelism correctly
	auto tol_sys=2.0*tol*len1*len1*len2*len2;
	// First check if segments meets at the end
	for (unsigned int i=0;i<2;++i){
	    auto const & P1=S1[i];
	    for (unsigned int j=0;j<2;++j){
	        auto const & P2=S2[j];
	        auto dist=norm(P1-P2);
	        if (dist<=tol_dist){
	            if(out.numberOfIntersections>=2){
	                std::cerr<<
	                    "Something wrong, cannot have more that 2 intersections"
	                            <<std::endl;
	                out.good=false;
	                return out;
	            }
	            out.intersect=true;
	            out.intersectionPoint[out.numberOfIntersections++]=P1;
	            out.endPointIsIntersection[0][i]=true;
	            out.endPointIsIntersection[1][j]=true;
	            out.otherEdgePoint[0][i]=j;
	            out.otherEdgePoint[1][j]=i;
	        }
	    }
	}
	// Handle the case where the two edges are identical!
	if (out.numberOfIntersections==2u){
	    out.identical=true;
	    out.parallel=true;
	    out.collinear=true;
	    return out;
	}
	// Now solve the linear system that returns the parametric coordinates of the intersection
	std::array<std::array<double,2>,2> A;
	// to make life easier I call A and B the ends of the two segmensts
	auto const & A1=S1[0];
	auto const & B1=S1[1];
	auto const & A2=S2[0];
	auto const & B2=S2[1];
	auto  V1 = B1-A1;
	auto  V2 = B2-A2;
	A[0][0]= dot(V1,V1);
	A[0][1]=-dot(V1,V2);
	A[1][0]= A[0][1];
	A[1][1]= dot(V2,V2);
	std::array<double,2> b;
	b[0]=    dot(A2-A1,V1);
	b[1]=    dot(A1-A2,V2);
	// find parametric coordinate of the intersection
	auto result=solve(A,b,tol_sys);
	// Standard case, the two lines are not parallel
	if(result.first==true){
	    auto const & t = result.second;
	    // Make a stupid check (only in debugging phase)
	#ifndef NDEBUG
	    auto P1 = A1+ t[0]*(B1-A1);
	    auto P2 = A2+ t[1]*(B2-A2);
	    if(norm(P1-P2)>tol_dist)
	        std::cerr<<" Something strange, intersection points not coincident. Distance= "<<norm(P1-P2);
	#endif        
	    // The two lines intersect.
	    // Check whether we are inside the segments
	    double const & t1=result.second[0];
	    double const & t2=result.second[1];
	    //double tol1=tol/len1;
	    //double tol2=tol/len2;
	    bool inside =v(t1>=-0.5*tol) && (t1<= 1.0+0.5*tol) && (t2>=-0.5*tol) && (t2<= 1.0+0.5*tol);
	    if (!inside){
	        // No intersecion, end here
	        return out;
	    } else {
	        out.intersect=true;
	        // I coud have used the mean
	        if (std::abs(t1    )<= tol){
	            if(out.endPointIsIntersection[0][0]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[0][0]=true;
	            }
	        }
	        if (std::abs(t1-1.0)<= tol){
	            if(out.endPointIsIntersection[0][1]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[0][1]=true;
	            }
	        }
	        if (std::abs(t2    )<= tol){
	            if(out.endPointIsIntersection[1][0]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[1][0]=true;
	            }
	        }
	        if (std::abs(t2-1.0)<= tol){
	            if(out.endPointIsIntersection[1][1]){
	                // already found. End here
	                return out;
	            } else {
	                out.endPointIsIntersection[1][1]=true;
	            }
	        }
	        out.intersectionPoint[out.numberOfIntersections++]=A1+ t1*(B1-A1);
	        return out;
	    }
	} else {
	    out.parallel=true;
	    // Compute distance between the two lines
	    /*
	    (x-s1(0,0))(s1(1,1)-s1(0,1))-(s1(1,0)*s1(0,0))*(y-s1(0,1))=0
	    (s1(1,1)-s1(0,1))x+(s1(0,0)*s1(1,0))y+s1(0,1)(s1(1,0)*s1(0,0))-
	    (s1(1,1)-s1(0,1))s1(0,0)=0
	    */
	    double factor=dot(B1-A1,A2-A1)/(len1*len1);
	    auto distVect = (A1-A2) + factor*(B1-A1);
	    double distance = norm(distVect);
	    out.distance=distance;
	    // Maybe here you want to use a different tolerance?
	    if (distance>tol_dist){
	        // The two segments are separated
	        // No intersection.
	        return out;
	    } else {
	        out.collinear=true;
	        double t;
	        // Segments are collinear! We need to consider all the cases!
	        // IS A2 on S1? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[1][0]){
	            t=dot((A2-A1),(B1-A1))/(len1*len1);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=A2;
	                out.endPointIsIntersection[1][0]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	        // IS B2 on S1? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[1][1]){
	            t=dot((B2-A1),(B1-A1))/(len1*len1);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=B2;
	                out.endPointIsIntersection[1][1]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	        // IS A1 on S2? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[0][0]){
	            t=dot((A1-A2),(B2-A2))/(len2*len2);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=A1;
	                out.endPointIsIntersection[0][0]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	        // IS B1 on S2? Maybe I have already considered it earlier
	        if(!out.endPointIsIntersection[0][1]){
	            t=dot((B1-A2),(B2-A2))/(len2*len2);
	            if(t>=-0.5*tol && t<=1+0.5*tol){
	                out.intersect=true;
	                out.intersectionPoint[out.numberOfIntersections++]=B1;
	                out.endPointIsIntersection[0][1]=true;
	                if(out.numberOfIntersections==2)return out;
	            }
	        }
	    }
	}
	return out;
}	//compute_intersection
	
	
	

};	//BGLgeom

#endif	//HH_INTERSECTIONS_2D_HH
