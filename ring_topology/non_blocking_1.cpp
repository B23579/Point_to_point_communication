#include<iostream>
#include<mpi.h>

int main(int argc,char *argv[]){
	int numtasks,rank,next, prev, buf[2], itag_rank=1, itag_rec_prev=2, itag_rec_next=2;
	MPI_Request reqs[4]; // required variable for non-blocking calls
	MPI_Status stats[4]; // required varaible  for Waitall routine

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	//determine the right and left neighbors

	prev = rank -1;
	next = rank + 1;

	int send = -rank;
	if(rank==0) prev= numtasks-1;
	if(rank== (numtasks-1)) next = 0;
	int compt =0;
	
	itag_rank = rank*10;
	itag_rec_prev= prev*10;
	itag_rec_next = next*10;

	
	if(rank %2==0){
		

		while(compt<3){
		compt++;
		MPI_Irecv(&buf[1],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&reqs[0]);

		MPI_Isend(&rank,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD,&reqs[1]);
		
		// post non blocking receives and sends for neighbord
		MPI_Irecv(&buf[0],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &reqs[2]);
	
		MPI_Isend(&send,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD,&reqs[3]);
	
		// wait for all non-blocking operations to complete

		MPI_Waitall(4, reqs, stats);
		std::cout<<" I received " <<  buf[0] << " and " << buf[1]<< ". My rank is " << rank  <<std::endl;
		}

	}
	else
	{	
		while(compt<3){
		MPI_Isend(&rank,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD,&reqs[0]);
		MPI_Irecv(&buf[1],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&reqs[1]);

			
		// post non blocking receives and sends for neighbord
	
		
		MPI_Irecv(&buf[0],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &reqs[3]);
		MPI_Isend(&send,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD,&reqs[2]);
		// wait for all non-blocking operations to complete

		MPI_Waitall(4, reqs, stats);
		
		std::cout<< " I received " << buf[0] << " and " << buf[1] << ". My rank is " << rank << std::endl;
		compt= compt+1;
		}
	}
	

	// do some work while send/received progress in background 


	//continue - do more work

	MPI_Finalize();	

	std::cout<<"Done..."<<std::endl;
}
