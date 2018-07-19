//  
//  algorithm - some algorithms in "Introduction to Algorithms", third edition
//  Copyright (C) 2018  lxylxy123456
//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Affero General Public License as
//  published by the Free Software Foundation, either version 3 of the
//  License, or (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Affero General Public License for more details.
//  
//  You should have received a copy of the GNU Affero General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//  

#ifndef MAIN
#define MAIN
#define MAIN_DFS
#endif

#ifndef FUNC_DFS
#define FUNC_DFS

#include "utils.h"

#include "Graph.cpp"

const size_t INF = -1; 

enum DFSColor { white, gray, black }; 

enum DFSEdgeType { tree, back, forward, cross }; 

template <typename T>
class DFSInfo {
	public:
		DFSInfo(): color(white), d(INF), f(INF), pi_exist(false) {}
		void set_color(DFSColor c, size_t& time) {
			color = c; 
			switch (color) {
				case white: 
					break; 
				case gray: 
					d = time++; 
					break; 
				case black: 
					f = time++; 
					break; 
			}
		}
		void set_pi(T p) {
			pi = p; 
			pi_exist = true; 
		}
		enum DFSColor color; 
		size_t d, f; 
		T pi; 
		bool pi_exist; 
}; 

std::ostream& operator<<(std::ostream& os, const DFSEdgeType& rhs) {
	switch (rhs) {
		case tree: 
			return os << "T"; 
		case back: 
			return os << "B"; 
		case forward: 
			return os << "F"; 
		case cross: 
			return os << "C"; 
		default: 
			return os; 
	}
}

template <typename T>
bool is_ancestor(umap<T, DFSInfo<T>>& ans, T u, T v) {
	if (u == v)
		return true; 
	DFSInfo<T>& info = ans[v]; 
	if (info.pi_exist)
		return is_ancestor(ans, u, info.pi); 
	else
		return false; 
}

template <typename GT, typename T, typename VT, typename ET>
void DFSVisit(GT& G, VT& ans, ET& edge_ans, T u, size_t& time) {
	DFSInfo<T>& info = ans[u]; 
	info.set_color(gray, time); 
	for (auto i = G.edges_from(u); !i.end(); i++) {
		T v = i.d(); 
		DFSInfo<T>& vinfo = ans[v]; 
		switch (vinfo.color) {
			case white: 
				edge_ans[*i] = tree; 
				vinfo.set_pi(u); 
				DFSVisit(G, ans, edge_ans, v, time); 
				break; 
			case gray: 
				edge_ans[*i] = back; 
				break; 
			case black: 
				if (is_ancestor(ans, u, v))
					edge_ans[*i] = forward; 
				else
					edge_ans[*i] = cross; 
				break; 
		}
	}
	info.set_color(black, time); 
}

template <typename GT, typename VT, typename ET>
void DFS(GT& G, VT& ans, ET& edge_ans) {
	using T = typename GT::vertix_type; 
	for (auto i = G.V.begin(); i != G.V.end(); i++)
		ans[*i] = DFSInfo<T>(); 
	size_t time = 0; 
	for (auto i = G.V.begin(); i != G.V.end(); i++)
		if (ans[*i].color == white)
			DFSVisit(G, ans, edge_ans, *i, time); 
	if (!G.dir)
		for (auto i = G.all_edges(); !i.end(); i++)
			if (i.s() < i.d()) {
				Edge<T> j = (*i).reverse(); 
				if (edge_ans[*i] != edge_ans[j]) {
					edge_ans[j] = std::min(edge_ans[*i], edge_ans[j]); 
					edge_ans[*i] = edge_ans[j]; 
				}
			}
}
#endif

#ifdef MAIN_DFS
int main(int argc, char *argv[]) {
	const size_t v = get_argv(argc, argv, 1, 5); 
	const size_t e = get_argv(argc, argv, 2, 10); 
	const bool dir = get_argv(argc, argv, 3, 0); 
	GraphAdjList<size_t> G(dir); 
	random_graph(G, v, e); 
	umap<size_t, DFSInfo<size_t>> inf; 
	umap<Edge<size_t>, DFSEdgeType, EdgeHash<size_t>> edge_inf; 
	DFS(G, inf, edge_inf); 
	auto f1 = [inf](size_t v) mutable {
		DFSInfo<size_t>& i = inf[v]; 
		std::cout << " ["; 
		switch (i.color) {
			case white: 
				break; 
			case black: 
				std::cout << "color=black style=filled fontcolor=white "; 
				break; 
			case gray: 
				std::cout << "color=gray style=filled "; 
				break; 
		}
		std::cout << "label=\"" << v << "\\n"; 
		std::cout << "d = " << i.d << "; f = " << i.f; 
		if (i.pi_exist)
			std::cout << "; pi = " << i.pi; 
		std::cout << "\"]"; 
		return true; 
	}; 
	auto f2 = [edge_inf](Edge<size_t> e) mutable {
		std::cout << " [" << "label=\"" << edge_inf[e] << "\"]"; 
	}; 
	graphviz(G, f1, f2); 
	return 0; 
}
#endif

