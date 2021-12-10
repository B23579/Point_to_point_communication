#include<iostream> 
#include<mpi.h>
#include<random>

int main( int arg, char **argv)
{
	int cols=4, rank, size;

	MPI_Init(&arg, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	double *my_value;
	
	my_value = new double[int(size*cols)];	
	
	int root_rank =0;
	double *table= new double[cols];
	
	srand(23);	
	
	for(int i=0; i<cols; i++){
	
		table[i] = i; //double( rand())/double(RAND_MAX);
	}



		// Let's scatter the table
	int count = int(cols);
	MPI_Gather(table,count, MPI_DOUBLE,my_value,count,MPI_DOUBLE,root_rank,MPI_COMM_WORLD);

	if(rank==root_rank){
	std::cout<<"My rank is " << rank <<std::endl;

	
	for(int  i=0;i<count*size;++i){
		std::cout<<  *(my_value+i) << " " ;}
	
	std::cout<< std::endl;
		
	}

	MPI_Finalize();
	delete[] my_value;
	delete[] table;
	return 0;

}
