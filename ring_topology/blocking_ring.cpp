#include<iostream>
#include<mpi.h>

int main(int argc,char *argv[]){
	int numtasks,rank,next, prev, buf[2]={100,100}, itag_rank=1, itag_rec_prev=2, itag_rec_next=2;

	MPI_Status stats[2]; // 

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//determine the right and left neighbors

	prev = rank -1;
	next = rank + 1;

	int msgleft = -rank, msgright = rank;

	if(rank==0) prev= numtasks-1;
	if(rank== (numtasks-1)) next = 0;
	int cond=1000; // in the cluster, we are allow to use 24 processes, using this number, we are sure 
		// that any process will have this rank.  So it help to solve deadlock at the first step 
		// of the program 
	
	itag_rank = rank*10;
	itag_rec_prev= prev*10;
	itag_rec_next = next*10;

	
	if(rank %2==0){ // this condition is used to avoid deadlock. at the begining 
		

		while(cond !=rank){

			MPI_Recv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&stats[0]); // buf[0] -> msgleft

			MPI_Send(&msgright,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD);
		
		// post non blocking receives and sends for neighbord

			MPI_Recv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &stats[1]); //buf[1] -> msgright
	
			MPI_Send(&msgleft,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD);

	// The message receive from the next process become the message to send  to the previous process,inversely. 
			msgleft =  buf[0];
			msgright = buf[1];	

			cond =msgleft;
		}
	
		std::cout<<" My msgright is " <<  msgright << " and my msgleft is " << msgleft << ". My rank is " << rank  <<std::endl;
	}
	else
	{	
		while( cond!= rank){
			MPI_Send(&msgleft,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD);
			MPI_Recv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&stats[0]); // buf[0] -> msgleft

			
		// post non blocking receives and sends for neighbord
	
				 
			MPI_Send(&msgright,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD);
		
			MPI_Recv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &stats[1]); // buef[1]-> msgright

		// wait for all non-blocking operations to complete

			msgleft=buf[0];
			msgright= buf[1];
			cond= msgleft;
		}

			std::cout<<" My msgright is " <<  msgright << " and my msgleft is " << msgleft << ". My rank is " << rank  <<std::endl;
	}
	




	MPI_Finalize();	

	std::cout<<"Process ID " << rank << " Done..."<<std::endl;
}
