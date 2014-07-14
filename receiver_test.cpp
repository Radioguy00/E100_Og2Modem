
#define DEFINE_GLOBALS

#include "/usr/include/uhd/usrp/multi_usrp.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <csignal>
#include <fstream>
#include <cmath>
#include "uhd_utilities.h"
#include <pthread.h>
#include "task_sampling.h"

bool stop_signal_called = false;


void sig_int_handler(int) {stop_signal_called = true;}


#define MAIN_ERROR_SAMPLING_TASK_NOT_CREATED 1 ;


int main(int argc, char ** argv)
{
	namespace radio = uhd::usrp;
	using namespace uhd;
	
	// Start of program
	printf("\n-----> Start of Test Program\n");


	// Set up the signal handler for CTRL+C
	std::signal(SIGINT, &sig_int_handler);
	
	//-----------------------------------------------
	// Create the USRP Hardware object
	//-----------------------------------------------
	size_t mboard = 0;

	uhd::device_addr_t args;
	std::cout << std::endl << "-----> Creating device" << std::endl;
	radio::multi_usrp::sptr usrp = radio::multi_usrp::make(args);
	
	// Configure the board as desired
	// Sample rate
	usrp->set_rx_rate(125000);
	std::cout << "Rx Sample rate: "  << usrp->get_rx_rate() << std::endl;
	// Initial receive frequency
	tune_request_t tune_request(135e6, 55e3);
	tune_result_t tune_result = usrp->set_rx_freq(tune_request);
	std::cout << "Target RF frequency: " << tune_result.target_rf_freq << std::endl;
	std::cout << "Actual RF frequency: " << tune_result.actual_rf_freq << std::endl;
	std::cout << "Target DSP frequency: " << tune_result.target_dsp_freq << std::endl;
	std::cout << "Actual DSP frequency: " << tune_result.actual_dsp_freq << std::endl;
	// Display the board configuration
	get_rx_parameters(usrp, 0, std::cout);	
	
	//-----------------------------------------------
	// Initialize sample buffers
	//-----------------------------------------------
	const int samps_per_buf = 10000;
	g_inbuf[0].assign(samps_per_buf, 0);
	g_inbuf[1].assign(samps_per_buf, 0);

	//-----------------------------------------------
	// Start the rx sampling task
	//-----------------------------------------------
	task_sampling rx_task(usrp);
	if(rx_task.start())
	{
		// An error occurred
		std::cout << "Rx sampling task could not be created" << std::endl;
		return MAIN_ERROR_SAMPLING_TASK_NOT_CREATED;
	}
	
	//------------------------------------------------
	// Check and process input commands
	//------------------------------------------------
	std::string s;
	for (;;)
	{
	std::cout << "Waiting for commands: ";
	std::cin >> s;
	//std::cout << s << std::endl;
	if (s == "quit")
		{
		rx_task.stop();
		break;
		}
	
	}
	
	
	//------------------------------------------------
	//  Wait for thread completion
	//------------------------------------------------
	void * exit_status;
	int res = pthread_join(rx_task.get_tid(), & exit_status); // Exit status in *status_ptr

	
	return 0;
	
}



