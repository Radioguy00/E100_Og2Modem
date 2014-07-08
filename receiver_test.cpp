#include "/usr/include/uhd/usrp/multi_usrp.hpp"
#include <cstdio>
#include <iostream>
#include <vector>
#include <csignal>
#include <fstream>
#include "uhd_utilities.h"

bool stop_signal_called = false;

void sig_int_handler(int) {stop_signal_called = true;}


int main(int argc, char ** argv)
{
	namespace radio = uhd::usrp;
	using namespace uhd;
	

	// Set up the signal handler for CTRL+C
	std::signal(SIGINT, &sig_int_handler);

	// Create a file to log the metadata
	std::ofstream rx_log_file("rx_log.txt", std::ofstream::out);
	if(rx_log_file.fail())
	{
		std::cout << " Metadata file could not be opened" << std::endl;
		exit(1);
	}
	// Create a file to log the samples
	std::ofstream rx_data_file("rx_data.txt", std::ofstream::out);
	if(rx_data_file.fail())
	{
		std::cout << " Sample file could not be opened" << std::endl;
		exit(2);
	}

	// Create a USP object
	printf("\n-----> Start of Test Program\n");
	size_t mboard = 0;

	uhd::device_addr_t args;
	std::cout << std::endl << "-----> Creating device" << std::endl;
	radio::multi_usrp::sptr usrp = radio::multi_usrp::make(args);
	
	// Configure the baord as desired
	// Sample rate
	usrp->set_rx_rate(125000);
	tune_request_t tune_request(137.5e6);
	tune_result_t tune_result = usrp->set_rx_freq(tune_request);
	std::cout << "Target RF frequency: " << tune_result.target_rf_freq << std::endl;
	std::cout << "Actual RF frequency: " << tune_result.actual_rf_freq << std::endl;
	std::cout << "Target DSP frequency: " << tune_result.target_dsp_freq << std::endl;
	std::cout << "Actual DSP frequency: " << tune_result.actual_dsp_freq << std::endl;
	// Display the board configuration
	get_rx_parameters(usrp, 0, rx_log_file);
	
	
	// Create a streamer object
	//using  sample_type = short;
	typedef short sample_type; // Each I and Q is a short
	const size_t samps_per_buff = 8192; // Number of complex samples
	stream_args_t rx_stream_args("sc16", "sc16");
	rx_streamer::sptr rx_stream = usrp->get_rx_stream(rx_stream_args);
	rx_metadata_t md;
	std::vector<std::complex<sample_type> > buff(samps_per_buff);
	
	stream_cmd_t stream_cmd(stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
	stream_cmd.num_samps = samps_per_buff;
	stream_cmd.stream_now = true;
	stream_cmd.time_spec = time_spec_t();
	usrp->issue_stream_cmd(stream_cmd);
	
	std::cout << "Sample size is " << sizeof(sample_type) << std::endl;
	std::cout << "Complex size is " << sizeof(std::complex<sample_type>) << std::endl;
	
	
	// Infinite loop which fills each buffer alternatively
	while(!stop_signal_called)
	{
		size_t rx_num = rx_stream->recv(&buff.front(), buff.size(), md, 5,false);
		// We write the info to the log file
		rx_log_file << std::endl;
		rx_log_file << "Samples Received: " << rx_num <<std::endl;
		display_rx_metadata(md, rx_log_file);
		// We write the data to the binary data file in an unformatted way
		// Fromat of data is I16Q16I16Q16....
		rx_data_file.write(reinterpret_cast<const char*>(&buff.front()), buff.size()*2*sizeof(sample_type)); 
		//for(int index = 0; index < buff.size(); index ++)
		//	rx_data_file << buff[index] << std::endl;
		
	}
	
	rx_log_file.close();
	rx_data_file.close();
	
	
	return 0;
	
}


