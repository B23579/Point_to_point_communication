#include<iostream>
#include<mpi.h>

int main(int argc, char **argv){
	MPI::Init(argc,argv);
	int rank = MPI::COMM_WORLD.Get_rank();
	int nprocs = MPI::COMM_WORLD.Get_size();
	int a= 56;
	int rec=0;
	if(rank ==0){
		std:: cout<< "Process rank " <<rank << std::endl;
		MPI::COMM_WORLD.Send(&a,1,MPI_INT,1,0);
		std:: cout <<" Process rank "<< rank << "send "<< a<<std::endl;
		MPI::COMM_WORLD.Recv(&rec,1,MPI_INT,1,0);
		std:: cout<< " Process  " << rank << "receive "<< rec << " from process "<< 1<< std::endl;

			}
	else{ 
		std::cout<< "Process rank "<< rank <<std::endl;
		int rec=0,a=23;
		MPI::COMM_WORLD.Recv(&rec,1,MPI_INT,0,0);
		std::cout<<"Process " << rank << " received "<< rec << "from process " << 0<<std::endl;
		MPI::COMM_WORLD.Send(&a,1,MPI_INT,0,0);
		}

	MPI::Finalize();

	return 0;
}
