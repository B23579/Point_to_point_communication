#include<iostream>
#include<mpi.h>

int main(int argc,char *argv[]){
	int numtasks,rank,next, prev, buf[2]={100,100}, itag_rank=1, itag_rec_prev=2, itag_rec_next=2;
	MPI_Request reqs[4]; // required variable for non-blocking calls
	MPI_Status stats[4]; // required varaible  for Waitall routine

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//determine the right and left neighbors

	prev = rank -1;
	next = rank + 1;

	int msgleft = -rank, msgright = rank;

	if(rank==0) prev= numtasks-1;
	if(rank== (numtasks-1)) next = 0;
	int cond=1000;
	
	itag_rank = rank*10;
	itag_rec_prev= prev*10;
	itag_rec_next = next*10;

	
	if(rank %2==0){
		

		while(cond !=rank){
		MPI_Irecv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&reqs[0]); // buf[0] -> msgleft

		MPI_Isend(&msgright,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD,&reqs[1]);
		
		// post non blocking receives and sends for neighbord
		MPI_Irecv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &reqs[2]); //buf[1] -> msgright
	
		MPI_Isend(&msgleft,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD,&reqs[3]);
	
		// wait for all non-blocking operations to complete
		msgleft =  buf[0];
		msgright = buf[1];

		MPI_Waitall(4, reqs, stats);
	
		std::cout<<" My msgright is " <<  msgright << " and my msgleft is " << msgleft << ". My rank is " << rank  <<std::endl;
		cond =msgright;
		}

	}
	else
	{	
		while( cond!= rank){
		MPI_Isend(&msgleft,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD,&reqs[0]);
		MPI_Irecv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&reqs[1]); // buf[0] -> msgleft

			
		// post non blocking receives and sends for neighbord
	
		
		MPI_Irecv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &reqs[3]); // buef[1]-> msgright 
		MPI_Isend(&msgright,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD,&reqs[2]);
		// wait for all non-blocking operations to complete

		MPI_Waitall(4, reqs, stats);
		msgleft=buf[0];
		msgright= buf[1];
		
		std::cout<< " My msgright is " << msgright << " and my msgleft is " << msgleft << ". My rank is " << rank << std::endl;
		cond= msgright;
		}
	}
	

	// do some work while send/received progress in background 


	//continue - do more work

	MPI_Finalize();	

	std::cout<<"Done..."<<std::endl;
}
