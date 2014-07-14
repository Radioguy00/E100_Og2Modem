
#include "task_sampling.h"
#include "/usr/include/uhd/usrp/multi_usrp.hpp"
#include <cstdio>
#include <iostream>
#include <vector>
#include <csignal>
#include <fstream>
#include <cmath>
#include "uhd_utilities.h"
#include <pthread.h>


/***********************************************************************//**
Constructor: Creates the resources required for the task


***************************************************************************/

task_sampling::task_sampling(uhd::usrp::multi_usrp::sptr & usrp_ref)
:usrp(usrp_ref), exit_task(false), current_buf(0)
{
	// Open the log file for the metadata
	rx_log.open("rx_log.txt", std::ofstream::out);
	if(rx_log.fail())
	{
		std::cout << " Metadata file could not be opened" << std::endl;
		exit(1);
	}
	// Create a file to log the samples
	rx_data.open("rx_data.txt", std::ofstream::out);
	if(rx_data.fail())
	{
		std::cout << " Sample file could not be opened" << std::endl;
		exit(2);
	}


}


/***********************************************************************//**
Destructor: Deallocate the resources required for the task


***************************************************************************/

task_sampling::~task_sampling()
{
	rx_log.close();
	rx_data.close();

}



/***********************************************************************//**
Starts the new thread

@return true if an error occurred, false otherwise


***************************************************************************/

bool task_sampling::start()
	{
	 
	// Create a streamer object - This defines the size of the samples
	uhd::stream_args_t rx_stream_args("sc16", "sc16");
	rx_stream = usrp->get_rx_stream(rx_stream_args);
	
	// Create the thread atttributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	size_t stack_size;
	pthread_attr_getstacksize(&attr, &stack_size);
	std::cout << "Stack Size: " << stack_size << std::endl;	
			
	// Start the thread
	exit_task = false;
	current_buf = 0;
	int res = pthread_create (&thread_id, &attr, &task_sampling::helper, this);
	if(res)
	{
		// an error in creating the thread occurred
		// A message is displayed and the process aborts.
		std::cout << "Thread for sampling_task could not be created" << std::endl;
		return true;
	}
	return false;				
}


/***********************************************************************//**
Main function of the RX sampling task. This is the function which effectively
runs in a different thread.



***************************************************************************/
void * task_sampling::run()
{

	using namespace uhd;	
	namespace radio = uhd::usrp;	

	std::cout << "Inside thread id  " << thread_id << std::endl;	

	// Structure to store the metadata of each received buffer
	rx_metadata_t md;
	// Send the command to start receiving data	
	stream_cmd_t stream_cmd(stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
	stream_cmd.num_samps = g_inbuf[0].size();
	stream_cmd.stream_now = true;
	stream_cmd.time_spec = time_spec_t();
	usrp->issue_stream_cmd(stream_cmd);

	current_buf = 0;
	// Infinite loop which fills each buffer alternatively
	size_t rx_num;
	while(!exit_task)
	{
		// Lock the mutex correspondin to the current buffer
		pthread_mutex_lock(&demod_buf_index_mutex);
		demod_buf_index = current_buf ^1;
		pthread_mutex_unlock(&demod_buf_index_mutex);
		// Get the samples
		size_t buf_size = g_inbuf[current_buf].size();		
		rx_num = rx_stream->recv(&g_inbuf[current_buf].front(), buf_size, md, 5,false);
		// Signal the denod task to start
		pthread_cond_signal(&in_available);
		
		// We write the info to the log file
		rx_log << std::endl;
		rx_log << "Samples Received: " << rx_num <<std::endl;
		display_rx_metadata(md, rx_log);

		// We write the data to the binary data file in an unformatted way
		// Fromat of data is I16Q16I16Q16....
		//rx_data.write(reinterpret_cast<const char*>(&(g_inbuf[current_buf].front())), g_inbuf[current_buf].size()*sizeof(input_buf_t::value_type)); 
		
		// Switch buffer at each iteration
		current_buf ^= 1;
	}
	
	return NULL;
}