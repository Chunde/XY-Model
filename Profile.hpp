
#include <cmath>
#include <string>
#include <chrono>
#include <algorithm>
#include <iostream>

template<typename Duration = std::chrono::microseconds,
	typename F,
	typename ... Args>
	typename Duration::rep profile(F&& fun, Args&&... args) {
	const auto beg = std::chrono::high_resolution_clock::now();
	std::forward<F>(fun)(std::forward<Args>(args)...);
	const auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<Duration>(end - beg).count();
}

template <typename F, typename ... Ts>
double Time_function(F&& f, Ts&&...args)
{
	std::clock_t start = std::clock();
	std::forward<F>(f)(std::forward<Ts>(args)...);
	return static_cast<double>(std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
}