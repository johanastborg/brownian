/* 
 * brownian.cpp
 *
 * Copyright Johan Astborg 2014
 * https://github.com/joastbg
 *
 * A short demo program of how to use lazy generators, in particular
 * generators for random numbers and brownian motion. Use gen.sh to
 * plot result using gnuplot.
 */

#include <iostream>
#include <vector>
#include <ctime> 

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

// Definitions for random number generation
typedef boost::mt19937                                      T_base_prng;
typedef boost::normal_distribution<>                        T_norm_dist;
typedef boost::variate_generator<T_base_prng&, T_norm_dist> T_norm_varg;

using namespace std;

/* Sample generator for fibonacci numbers */
class fib_generator
{
public:
	typedef int result_type;
	fib_generator() : one(0), two(1), i(0) { }
	int operator()() { 

		if (++i < 2) return i;

		int res = one + two;

		one = two;
		two = res;

		return res;
	}
private:
	int one, two, i;
};

/* Random number generator, taking a seed as input */
class lazy_random_generator
{
public:
	typedef double result_type;
lazy_random_generator(double mean, double stdev, int seed) : base_prng(seed), norm_dist(mean, stdev), norm_varg(base_prng, norm_dist) { }
	double operator()() {
		return norm_varg();
	}
private:
	T_base_prng  base_prng;
	T_norm_dist  norm_dist;
	T_norm_varg  norm_varg;
};

/* Brownian motion generator */
class lazy_brownian_motion
{
public:
	typedef double result_type;
lazy_brownian_motion(double mean, double stdev, int seed) : base_prng(seed), norm_dist(mean, stdev), norm_varg(base_prng, norm_dist) { }
	double operator()() {
		memo+= norm_varg();
		return memo;
	}
private:
	double       memo;
	T_base_prng  base_prng;
	T_norm_dist  norm_dist;
	T_norm_varg  norm_varg;
};

/* TODO: Geometric brownian motion */

int main()
{
	cout << "Lazy generators v. 0.0.1" << endl;

	// Lazy generators

	// fib_generator gen;
	// boost::generator_iterator_generator<fib_generator>::type it = boost::make_generator_iterator(gen);
	// for (int i = 0; i < 15; ++i, ++it) {
	// 	std::cout << *it << std::endl;
	// }

	// lazy_random_generator zgen(0,1,static_cast<unsigned int>(std::time(0)));
	// boost::generator_iterator_generator<lazy_random_generator>::type it2 = boost::make_generator_iterator(zgen);
	// for (int i = 0; i < 15; ++i, ++it2) {
	// 	std::cout << *it2 << std::endl;
	// }

    lazy_brownian_motion bgen(0,1,static_cast<unsigned int>(std::time(0)));
	boost::generator_iterator_generator<lazy_brownian_motion>::type it3 = boost::make_generator_iterator(bgen);
	for (int i = 0; i < 1500; ++i, ++it3) {
		std::cout << *it3 << std::endl;
	}
  
	return 0;
}
