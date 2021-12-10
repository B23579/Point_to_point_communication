#include<iostream>

int main(int arg, char** argv){
	double*** p;
	int n= 3,m=7,k=7; // 3D matrix nxmxk

	p = new double**[n];

	for(int i=0; i<n;++i){

		p[i] = new double*[m];

		for(int j=0; j<m; ++j)
			{

			p[i][j]=new double[k];

			for(int l=0;l<k;l++)
			 	{

				p[i][j][l] = rand();
				}
			}
	}


		// print out the matrix
	
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j){
			for(int l=0;l<k;++l){
				std::cout<< p[i][j][l] << " " ;}
		std::cout<<"\n";}

	std::cout<<" \n";
	}

	// delete the dynamic memory 

	for(int i=0; i<n; i++){ 
		for(int j=0; j<m; j++){delete[] p[i][j];}
		delete p[i]; }

	return 0;
}
