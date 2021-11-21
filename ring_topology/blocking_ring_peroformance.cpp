#include<iostream>
#include<mpi.h>

#include<vector>


int main(int argc,char *argv[]){

			//" This program use a blocking communication \n" ;

	int numtasks,rank,next, prev, buf[2]={100,100}, itag_rank=1, itag_rec_prev=2, itag_rec_next=2;
	std::vector<double> walltime;


	MPI_Status stats[2]; // 

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	double starttime, endtime;
	//determine the right and left neighbors

	prev = rank -1;
	next = rank + 1;

	int msgleft = rank, msgright = -rank,sum=0, count=0; // sum collect the  the sum of the
		// rank and the receiving message at each step of the process.compt will compt the number of messages receive
		// for each process.   

	if(rank==0) prev= numtasks-1;
	if(rank== (numtasks-1)) next = 0;

	for(int i = 0; i< 18; i++)
	{
	starttime = MPI_Wtime();

	int cond=1000; // in the cluster, we are allow to use 24 processes, using this number, we are sure 
		// that any process will have this rank.  So it help to solve deadlock at the first step 
		// of the program 
	
	itag_rank = rank*10;
	itag_rec_prev= prev*10;
	itag_rec_next = next*10;


	
	if(rank %2==0){ // this condition is used to avoid deadlock. at the begining 
		

		while(cond !=rank){

			MPI_Recv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&stats[0]); // buf[0] -> msgleft

			MPI_Send(&msgleft,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD);
		
		// post non blocking receives and sends for neighbord

			MPI_Recv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &stats[1]); //buf[1] -> msgright
	
			MPI_Send(&msgright ,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD);

	// The message receive from the next process become the message to send  to the previous process,inversely. 
			msgleft =  buf[0];
			msgright = buf[1];	
			
			sum = rank + msgleft + msgright;
			count += 2 ; // because at each step, process p receive message from right and left.  

			cond =msgleft;
		}
		
	std::cout<<" I am process " << rank <<" and I have received " << count << " messages. " << " My final messages have tag  " << itag_rank;
	std::cout <<" .My final msgleft and msgright are "<< msgleft << " and " << msgright <<std::endl;
	
	std::cout << std::endl;

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
			sum = msgleft + msgright + rank;
			count += 2;
		}

	std::cout<<" I am process " << rank <<" and I have received " << count << " messages. " << " My final messages have tag " << itag_rank;
	std::cout <<" .My final msgleft and msgright are "<< msgleft<< " and "<< msgright <<std::endl;
	std::cout << std::endl;
	}
	



	endtime = MPI_Wtime();
	walltime.push_back(endtime-starttime);

	}

	MPI_Finalize();	
	


	std::cout<<"Process ID " << rank <<" .My walltime is  "<< walltime[0]<<" Done..."<<std::endl;
}
