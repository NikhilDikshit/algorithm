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
#define MAIN_RaceExample
#endif

#ifndef FUNC_RaceExample
#define FUNC_RaceExample

#include <thread>
#include "utils.h"

template <typename T>
void RaceExampleLoop(T* x) {
	(*x) = (*x) + 1; 
}

template <typename T>
T RaceExample() {
	T x = 0; 
	std::thread t1(RaceExampleLoop<T>, &x); 
	x = x + 1; 
	t1.join(); 
	return x; 
}
#endif

#ifdef MAIN_RaceExample
int main(int argc, char *argv[]) {
	size_t n = get_argv(argc, argv, 1, 100); 
	for (size_t i = 0; i < n; i++)
		std::cout << RaceExample<size_t>() << std::endl; 
	return 0; 
}
#endif

