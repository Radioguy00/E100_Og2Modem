/***********************************************************************//**
@file

Declaration ot type required to perform the sampling of data


***************************************************************************/

#ifndef TASK_SAMPLING_H
#define TASK_SAMPLING_H

#include <vector>
#include <complex>
#include "/usr/include/uhd/usrp/multi_usrp.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>

#ifdef DEFINE_GLOBALS
	#define EXTERN
#else
	#define EXTERN extern
#endif	


typedef short sampling_type ; // samples are 16 bits signed I and Q
typedef std::vector<std::complex<sampling_type> > input_buf_t;

// Buffers to store the input samples in a ping-pong fashion
EXTERN input_buf_t g_inbuf[2];
EXTERN pthread_cond_t  in_available;   // Condition vairable to trigger the demod when data is available
EXTERN int demod_buf_index;						// Indicates which buffer must be processed by the demodulator		
EXTERN pthread_mutex_t demod_buf_index_mutex;   // Mutex on the variable which indicates the next buffer for the demod to work on



/***********************************************************************//**
This class represents the task which is running the sampling of the
data and filling the buffers

This class is a singleton

***************************************************************************/
class task_sampling
{
public:
	task_sampling(uhd::usrp::multi_usrp::sptr & usrp);
	bool start();
	void stop() { exit_task = true;}
	/// Returns a reference to the buffer which can be processed by other tasks
	input_buf_t &get_buffer() {current_buf == 0? g_inbuf[1]:g_inbuf[0];}
	/// Returns the  thread identifier
	pthread_t get_tid() {return thread_id;}
	~task_sampling();

private:
	static void * helper(void * arg) {return static_cast<task_sampling*>(arg)->run();}
	uhd::usrp::multi_usrp::sptr & usrp;/// Hardware interface
	uhd::rx_streamer::sptr rx_stream;  /// rx_streamer object to control the stream
	void * run();			/// Main routine of the task
	std::ofstream  rx_log;		/// ostream to write the metadata associated with each buffer
	std::ofstream  rx_data;		/// osstream to write the sample data
	pthread_t thread_id;	/// ID of the thread
	bool exit_task;		/// Set to true to stop the task
	int current_buf;		/// Buffer curretnly filled by the task
	
};


#endif
