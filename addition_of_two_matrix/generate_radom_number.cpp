#include<iostream>
#include<random>

int main(int arg, char** argv)
{
	// to generate a random number, we  need to  declare a variable type mt19937_64
	// we supply the seed s when we initialize it 

	// this will initialize a Mersenne Twister that generates integers between 0 and 2^{64} -1
	// this are two large for int, which cant hold values up to 2^{31} - 1
	// we would need e.g unsigned long long to store them 


	const int s = 31;

	std:: mt19937_64 mtrand(s), bast(s); // mt19937_64 is a variable type like double or int and mtrand is 
		//the name choosing for particular variable

	std::uniform_real_distribution<double> unif(0.0,1.0); // this is another type of variable

	const double u=unif(bast);


	for(int i=0; i<5; i++) 
//		std::cout <<double( mtrand()) <<std::endl;
		std::cout<< u(i) << std::endl;

	
	

	return 0;
	

}
