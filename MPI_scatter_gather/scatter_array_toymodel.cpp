#include<iostream> 
#include<mpi.h>
#include<random>

int main( int arg, char **argv)
{
	int cols=10, rank, size;

	MPI_Init(&arg, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	double *my_value;
	my_value = new double[int(cols/2)];
	int root_rank =0;
	double *table= new double[cols];
	
	if(rank==0)
	{
		srand(23);	
		for(int i=0; i<cols; i++){
		
			table[i] = i; //double( rand())/double(RAND_MAX);
		}
	}


		// Let's scatter the table
	int count = int(cols/2);
	MPI_Scatter(table,count, MPI_DOUBLE,my_value,count,MPI_DOUBLE,root_rank,MPI_COMM_WORLD);

//	delete[] table;
	std::cout<<"My rank is " << rank <<std::endl;
	

	for(int  i=0;i<count;++i){
			std::cout<<  *(my_value+i) << " " ;}
	std::cout<< std::endl;
//	delete[] my_value;
	

	MPI_Finalize();

	delete[] my_value;
	delete[] table;
	return 0;

}
