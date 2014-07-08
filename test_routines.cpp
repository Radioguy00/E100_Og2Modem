#include "/usr/include/uhd/usrp/multi_usrp.hpp"
#include <cstdio>
#include <iostream>
#include <vector>


#define UHD_MAJOR_VERSION 3
#define UHD_CENTER_VERSION  5
#define UHD_MINOR_VERSION 3

#define GPIO_EXISTS (UHD_MAJOR_VERSION >= 3 && UHD_CENTER_VERSION > 5 && UHD_MINOR_VERSION >3)


namespace radio = uhd::usrp;
void get_rx_specification(radio::multi_usrp::sptr , size_t);
void get_tx_parameters(radio::multi_usrp::sptr , size_t);

#if GPIO_EXISTS == 1
void get_gpio(size_t mboard);
#endif

main()
{
	printf("\n-----> Start of Test Program\n");
	size_t mboard = 0;

	uhd::device_addr_t args;
	std::cout << std::endl << "-----> Creating device" << std::endl;
	radio::multi_usrp::sptr usrp = radio::multi_usrp::make(args);

	// General routines
	std::cout << std::endl << "********** General ***********" << std::endl;

	std::cout << std::endl << "-----> Get Number of Mother Boards" << std::endl;
	int num_mb = usrp->get_num_mboards();
	std::cout << num_mb;

	std::cout << std::endl << "-----> Get MotherBoard Name" << std::endl;
	std::string mb_name = usrp->get_mboard_name();
	std::cout << mb_name;
	
	std::cout << std::endl << "-----> Get String" << std::endl;
	std::string pp_string = usrp->get_pp_string();
	std::cout << pp_string;

	// Clock Handling routines
	
	std::cout << std::endl << "********** Clock ***********" << std::endl;

	std::cout << std::endl << "-----> Get Clock Rate" << std::endl;
	double clock_rate = usrp->get_master_clock_rate();
	printf("Clock Rate is: %f", clock_rate);
	
	std::cout << std::endl << "-----> Get Clock Source List" << std::endl;
	std::vector<std::string> clock_sources = usrp->get_clock_sources(mboard);
	std::cout << "Clock Source options: " << std::endl;
	for (int index =0; index < clock_sources.size(); index++)
		std::cout << "\t" << clock_sources[index] << std::endl;
		
	std::cout << std::endl << "-----> Get Clock Source" << std::endl;
	std::string clock_source = usrp->get_clock_source(mboard);
	std::cout << "Clock Source: " << clock_source << std::endl;
			
	
	// Time handling routines
	std::cout << std::endl << "********** Time ***********" << std::endl;

	std::cout << std::endl << "-----> Get Time Source Options" << std::endl;
	std::vector<std::string> time_sources = usrp->get_time_sources(mboard);
	std::cout << "Time Source options: " << std::endl;
	for (int index =0; index < time_sources.size(); index++)
		std::cout << "\t" << time_sources[index] << std::endl;

	std::cout << std::endl << "-----> Get Time Source" << std::endl;
	std::string time_source = usrp->get_time_source(mboard);
	std::cout << "Time Source: " << time_source << std::endl;
	
	std::cout << std::endl << "-----> Get Current Time in usrp time registers" << std::endl;
	uhd::time_spec_t usrp_time = usrp->get_time_now();
	std::cout << "Full Seconds: " << usrp_time.get_full_secs() << std::endl << "Fractional Seconds: " << usrp_time.get_frac_secs() <<  std::endl;
	std::cout << "Real Seconds " << usrp_time.get_real_secs() << std::endl;

	std::cout << std::endl << "-----> Get Last PPS Time" << std::endl;
	uhd::time_spec_t usrp_pps_time = usrp->get_time_last_pps();
	std::cout <<  "Full Seconds " << usrp_pps_time.get_full_secs() << std::endl << "Fractional seconds  " << usrp_pps_time.get_frac_secs()  << std::endl;
	std::cout << "Real Seconds " << usrp_pps_time.get_real_secs() << std::endl;


	std::cout << std::endl << "********** MBoard Sensors  ***********" << std::endl;

	// List of all available sensors
	std::cout << std::endl << "-----> Get MBoard Sensors Name" << std::endl;
	std::vector<std::string> sensor_names = usrp->get_mboard_sensor_names(mboard);
	std::cout << "Sensor Names: " << std::endl;
	for (int index =0; index < sensor_names.size(); index++)
	{
		// Name
		std::cout << "\t" << sensor_names[index] << std::endl;
		// Value
		//uhd::sensor_value_t sensor_value = usrp->get_mboard_sensor(sensor_names[index]);
		//std::cout << sensor_value.to_pp_string()<< std::endl;
	}
		
		
	get_rx_specification(usrp, mboard);
	get_tx_parameters(usrp,mboard);
	#if GPIO_EXISTS == 1
	get_gpio(mboard);
	#endif
}

void get_rx_specification(radio::multi_usrp::sptr usrp, size_t mboard)
{
	using namespace std;
	size_t nchan = 0;

	// Get sub device specification
	std::cout << std::endl << "********** RX Sub Device ***********" << std::endl;
	std::cout << std::endl << "-----> Get Rx Subdevice" << std::endl;
	radio::subdev_spec_t rx_subdev = usrp->get_rx_subdev_spec(mboard);
	cout << "RX Subdevice Specification:" << endl;
	cout << rx_subdev.to_pp_string() << endl;
	
	std::cout << std::endl << "-----> Get number of RX channels" << std::endl;
	size_t num_rx = usrp->get_rx_num_channels();
	cout << "Number of RX channels:" << endl;
	cout << num_rx << endl;
	
	std::cout << std::endl << "-----> Get RX Subdevice Name" << std::endl;
	string rx_name = usrp->get_rx_subdev_name(nchan);
	cout << "RX Subdevice Name:" << endl;
	cout << rx_name << endl;

	std::cout << std::endl << "********** RX Sample Rate ***********" << std::endl;
	std::cout << std::endl << "-----> Get RX Rate" << std::endl;
	double rx_rate = usrp->get_rx_rate(nchan);
	cout << "RX Rate:" << endl;
	cout << rx_rate << endl;
	
	std::cout << std::endl << "-----> Get RX Rate List" << std::endl;
	uhd::meta_range_t rx_rates = usrp->get_rx_rates(nchan);
	cout << "RX Rate List:" << endl;
	cout << "Start: " << rx_rates.start() << "   Stop: " << rx_rates.stop() << "   Step: " << rx_rates.step() << endl;
	cout << rx_rates.to_pp_string() << endl;
	
	// RX FREQUENCIES
	
	std::cout << std::endl << "********** RX Frequencies ***********" << std::endl;
	std::cout << std::endl << "-----> Get RX Center Frequency" << std::endl;
	double rx_freq = usrp->get_rx_freq(nchan);
	cout << "RX Freq:" << endl;
	cout << rx_freq << endl;

	std::cout << std::endl << "-----> Get RX Center Frequency Range" << std::endl;
	uhd::freq_range_t rx_freq_range = usrp->get_rx_freq_range(nchan);
	cout << "RX Frequency Range:" << endl;
	cout << "Start: " << rx_freq_range.start() << "   Stop: " << rx_freq_range.stop() << "   Step: " << rx_freq_range.step() << endl;
	cout << rx_freq_range.to_pp_string() << endl;
	
	// RX Front end frequencies
	std::cout << std::endl << "-----> Get RX RF Front End Center Frequency Range" << std::endl;
	try
	{
		cout << "RX Front End Frequency Range:" << endl;
		uhd::freq_range_t rx_fe_freq_range = usrp->get_fe_rx_freq_range(nchan);
		cout << "Start: " << rx_fe_freq_range.start() << "   Stop: " << rx_fe_freq_range.stop() << "   Step: " << rx_fe_freq_range.step() << endl;
		cout << rx_fe_freq_range.to_pp_string() << endl;
	}
		catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}
	
	// RX GAIN
	
	std::cout << std::endl << "********** RX Gain  ***********" << std::endl;
	
	// Total combined gain
	cout << endl << "-----> Get RX Total Gain" << endl;
	cout << "RX Total Gain: " ;	
	try
	{
		double rx_total_gain = usrp->get_rx_gain(nchan);
		cout << rx_total_gain << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() << endl;
	}
	
	// List of all gain elements
	std::cout << std::endl << "-----> Get RX gain names" << std::endl;
	std::vector<std::string> rx_gain_names = usrp->get_rx_gain_names(nchan);
	std::cout << "Rx Gain Names: " << std::endl;
	for (int index =0; index < rx_gain_names.size(); index++)
	{
		// Name
		cout << "\t" << rx_gain_names[index] << endl;
	}
	for (int index =0; index < rx_gain_names.size(); index++)
	{
		// Name
		cout << "\t" << "Name: " << rx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		double element_gain = usrp->get_rx_gain(rx_gain_names[index], nchan);
		cout << element_gain << endl;
		}
		catch(uhd::runtime_error &e)
		{
			cout << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Gain ranges for each of the gain elements
	std::cout << std::endl << "-----> Get RX element gain ranges" << std::endl;
	for (int index =0; index < rx_gain_names.size(); index++)
	{
		// Name
		cout << "\t" << "Name: " << rx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		uhd::gain_range_t element_gain_range = usrp->get_rx_gain_range(rx_gain_names[index], nchan);
		cout << "Start: " << element_gain_range.start() << " End: " << element_gain_range.stop() << " Step: " << element_gain_range.step() << endl;
		}
		catch(uhd::runtime_error &e)
		{
			cout << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Total Gain range
	try
	{
		cout << endl << "-----> Get RX Total Gain Range" << endl;
		uhd::gain_range_t rx_total_gain_range = usrp->get_rx_gain_range(nchan);
		cout << "RX Total Gain Range: " ;
		cout << "Start: " << rx_total_gain_range.start() << " End: " << rx_total_gain_range.stop() << " Step: " << rx_total_gain_range.step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() <<endl;
	}
	
	// ANTENNA FUNCTIONS
	
	std::cout << std::endl << "********** RX ANTENNA ***********" << std::endl;
	// Current Rx Antenna
	std::cout << std::endl << "-----> Get RX Antenna" << std::endl;
	string rx_antenna = usrp->get_rx_antenna(nchan);
	cout << "RX Antenna: " ;
	cout << rx_antenna << endl;

	// RX Antenna choices
	std::cout << std::endl << "-----> Get Rx Antenna List" << std::endl;
	std::vector<std::string> rx_antennas = usrp->get_rx_antennas(nchan);
	std::cout << "RX Antennas : " << std::endl;
	for (int index =0; index < rx_antennas.size(); index++)
		std::cout << "\t" << rx_antennas[index] << std::endl;
		
		
	// RX BANDWIDTH FUNCTIONS

	std::cout << std::endl << "********** RX BANDWIDTH ***********" << std::endl;
	// Current RX Bandwidth
	cout << endl << "-----> Get RX Bandwidth" << endl;
	try
	{
		cout << "RX Bandwidth " ;
		double rx_bandwidth = usrp->get_rx_bandwidth(nchan);
		cout << rx_bandwidth << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << "Exception occured " << e.code() << endl;
	}
	
	// RX Bandwidth Range
	cout << endl << "-----> Get RX Bandwidth Range" << endl;
	try
	{
		cout << "RX Bandwidth Range: " ;
		uhd::gain_range_t rx_bandwidth_range = usrp->get_rx_bandwidth_range(nchan);
		cout << "Start: " << rx_bandwidth_range .start() << " End: " << rx_bandwidth_range .stop() << " Step: " << rx_bandwidth_range .step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() <<endl;
	}
	
	// RX DBOARD INTERFACE OBJECT

	std::cout << std::endl << "********** RX DBOARD INTERFACE ***********" << std::endl;
	// RX Dboard Interface
	cout << endl << "-----> Get rx_dboard_iface()" << endl;
	try
	{
		cout << "RX Dboard Interface " ;
		uhd::usrp::dboard_iface::sptr rx_dboard_iface = usrp->get_rx_dboard_iface(nchan);
		cout << rx_dboard_iface << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << "Exception occured " << e.code() << endl;
	}
	
	// RX _SENSORS

	std::cout << std::endl << "********** RX Sensors  ***********" << std::endl;

	// List of all available sensors
	std::cout << std::endl << "-----> Get RX Sensors Name" << std::endl;
	std::vector<std::string> rx_sensor_names = usrp->get_rx_sensor_names(nchan);
	std::cout << "Sensor Names: " << std::endl;
	for (int index =0; index < rx_sensor_names.size(); index++)
	{
		// Name
		std::cout << "\t" << rx_sensor_names[index] << ":  ";
		// Value
		try
		{
			uhd::sensor_value_t rx_sensor_value = usrp->get_rx_sensor(rx_sensor_names[index], nchan);
			std::cout << rx_sensor_value.to_pp_string()<< std::endl;
		}
		catch(uhd::runtime_error &e)
		{
			cout << "Exception occured " << e.code() << endl;
		}
	}
	

}

void get_tx_parameters(radio::multi_usrp::sptr usrp, size_t mboard)
{
	using namespace std;
	size_t nchan = 0;

	// CONFIGURATION SUB_DEVICE
	
	std::cout << std::endl << "********** TX Sub Device ***********" << std::endl;
	// Get sub device specification
	std::cout << std::endl << "-----> Get TX Subdevice" << std::endl;
	try
	{
	cout << "TX Subdevice Specification: ";
	radio::subdev_spec_t tx_subdev = usrp->get_tx_subdev_spec(mboard);
	cout << tx_subdev.to_pp_string() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}
	
	// Number of tx channels
	std::cout << std::endl << "-----> Get number of TX channels" << std::endl;
	try
	{
	size_t num_tx = usrp->get_tx_num_channels();
	cout << "Number of TX channels: " ;
	cout << num_tx << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}
	
	// TX Device Name
	std::cout << std::endl << "-----> Get TX Subdevice Name" << std::endl;
	try
	{
	cout << "TX Subdevice Name: ";
	string tx_name = usrp->get_tx_subdev_name(nchan);
	cout << tx_name << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}
	
	// TX SAMPLE RATE
	
	std::cout << std::endl << "********** TX Sample Rate ***********" << std::endl;
	
	// Get Current TX rate
	std::cout << std::endl << "-----> Get TX Rate" << std::endl;
	try
	{
		cout << "TX Rate: " ;
		double tx_rate = usrp->get_tx_rate(nchan);
		cout << tx_rate << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}
	
	// Get list of TX rates
	std::cout << std::endl << "-----> Get TX Rate List" << std::endl;
	try
	{
		cout << "TX Rate List: " ;
		uhd::meta_range_t tx_rates = usrp->get_tx_rates(nchan);
		cout << "Start: " << tx_rates.start() << "   Stop: " << tx_rates.stop() << "   Step: " << tx_rates.step() << endl;
		cout << tx_rates.to_pp_string() << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}

	// TX FREQUENCIES
	
	std::cout << std::endl << "********** TX Frequencies ***********" << std::endl;
	
	// Current TX frequency
	std::cout << std::endl << "-----> Get TX Center Frequency" << std::endl;
	try
	{
		cout << "TX Freq: ";
		double tx_freq = usrp->get_tx_freq(nchan);
		cout << tx_freq << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}

	// TX Frequency Range
	std::cout << std::endl << "-----> Get TX Center Frequency Range" << std::endl;
	try
	{
		cout << "TX Frequency Range: ";
		uhd::freq_range_t tx_freq_range = usrp->get_tx_freq_range(nchan);
		cout << "Start: " << tx_freq_range.start() << "   Stop: " << tx_freq_range.stop() << "   Step: " << tx_freq_range.step() << endl;
		cout << tx_freq_range.to_pp_string() << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}
	
	// Front end TX frequency range
	std::cout << std::endl << "-----> Get TX RF Front End Center Frequency Range" << std::endl;
	try
	{
		cout << "TX Front End Frequency Range: ";
		uhd::freq_range_t tx_fe_freq_range = usrp->get_fe_tx_freq_range(nchan);
		cout << "Start: " << tx_fe_freq_range.start() << "   Stop: " << tx_fe_freq_range.stop() << "   Step: " << tx_fe_freq_range.step() << endl;
		cout << tx_fe_freq_range.to_pp_string() << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << " Exception occurred : " << e.code() << endl;
	}

	// TX GAIN

	std::cout << std::endl << "********** TX Gain  ***********" << std::endl;
	
	// Total combined gain
	cout << endl << "-----> Get TX Total Gain" << endl;
	cout << "TX Total Gain: " ;	
	try
	{
		double tx_total_gain = usrp->get_tx_gain(nchan);
		cout << tx_total_gain << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() << endl;
	}
	
	// List of all gain elements
	std::cout << std::endl << "-----> Get TX gain names" << std::endl;
	std::vector<std::string> tx_gain_names = usrp->get_tx_gain_names(nchan);
	std::cout << "Tx Gain Names: " << std::endl;
	for (int index =0; index < tx_gain_names.size(); index++)
	{
		// Name
		cout << "\t" << tx_gain_names[index] << endl;
	}
	for (int index =0; index < tx_gain_names.size(); index++)
	{
		// Name
		cout << "\t" << "Name: " << tx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		double element_gain = usrp->get_tx_gain(tx_gain_names[index], nchan);
		cout << element_gain << endl;
		}
		catch(uhd::runtime_error &e)
		{
			cout << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Gain ranges for each of the gain elements
	std::cout << std::endl << "-----> Get TX element gain ranges" << std::endl;
	for (int index =0; index < tx_gain_names.size(); index++)
	{
		// Name
		cout << "\t" << "Name: " << tx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		uhd::gain_range_t element_gain_range = usrp->get_tx_gain_range(tx_gain_names[index], nchan);
		cout << "Start: " << element_gain_range.start() << " End: " << element_gain_range.stop() << " Step: " << element_gain_range.step() << endl;
		}
		catch(uhd::runtime_error &e)
		{
			cout << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Total Gain range
	try
	{
		cout << endl << "-----> Get TX Total Gain Range" << endl;
		uhd::gain_range_t tx_total_gain_range = usrp->get_tx_gain_range(nchan);
		cout << "TX Total Gain Range: " ;
		cout << "Start: " << tx_total_gain_range.start() << " End: " << tx_total_gain_range.stop() << " Step: " << tx_total_gain_range.step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() <<endl;
	}

	// ANTENNA FUNCTIONS
	
	std::cout << std::endl << "********** TX ANTENNA ***********" << std::endl;
	// Current Tx Antenna
	std::cout << std::endl << "-----> Get TX Antenna" << std::endl;
	try
	{
	cout << "TX Antenna: " ;
	string tx_antenna = usrp->get_tx_antenna(nchan);
	cout << tx_antenna << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() <<endl;
	}

	// TX Antenna choices
	std::cout << std::endl << "-----> Get Tx Antenna List" << std::endl;
	try
	{
	std::cout << "TX Antennas : " << std::endl;
	std::vector<std::string> tx_antennas = usrp->get_tx_antennas(nchan);
	for (int index =0; index < tx_antennas.size(); index++)
		std::cout << "\t" << tx_antennas[index] << std::endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() <<endl;
	}
		
	// TX BANDWIDTH FUNCTIONS

	std::cout << std::endl << "********** TX BANDWIDTH ***********" << std::endl;
	// Current TX Bandwidth
	cout << endl << "-----> Get TX Bandwidth" << endl;
	try
	{
		cout << "TX Bandwidth " ;
		double tx_bandwidth = usrp->get_tx_bandwidth(nchan);
		cout << tx_bandwidth << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << "Exception occured " << e.code() << endl;
	}
	
	// TX Bandwidth Range
	cout << endl << "-----> Get TX Bandwidth Range" << endl;
	try
	{
		cout << "TX Bandwidth Range: " ;
		uhd::gain_range_t tx_bandwidth_range = usrp->get_tx_bandwidth_range(nchan);
		cout << "Start: " << tx_bandwidth_range .start() << " End: " << tx_bandwidth_range .stop() << " Step: " << tx_bandwidth_range .step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		cout << "Exception code: " << e.code() <<endl;
	}
		
	// TX DBOARD INTERFACE OBJECT

	std::cout << std::endl << "********** TX DBOARD INTERFACE ***********" << std::endl;
	// TX Dboard Interface
	cout << endl << "-----> Get tx_dboard_iface()" << endl;
	try
	{
		cout << "TX Dboard Interface " ;
		uhd::usrp::dboard_iface::sptr tx_dboard_iface = usrp->get_tx_dboard_iface(nchan);
		cout << tx_dboard_iface << endl;
	}
	catch (uhd::runtime_error &e)
	{
		cout << "Exception occured " << e.code() << endl;
	}
	
	// TX _SENSORS

	std::cout << std::endl << "********** TX Sensors  ***********" << std::endl;

	// List of all available sensors
	std::cout << std::endl << "-----> Get TX Sensors Name" << std::endl;
	std::vector<std::string> tx_sensor_names = usrp->get_tx_sensor_names(nchan);
	std::cout << "Sensor Names: " << std::endl;
	for (int index =0; index < tx_sensor_names.size(); index++)
	{
		// Name
		std::cout << "\t" << tx_sensor_names[index] << ":  ";
		// Value
		try
		{
			uhd::sensor_value_t tx_sensor_value = usrp->get_tx_sensor(tx_sensor_names[index], nchan);
			std::cout << tx_sensor_value.to_pp_string()<< std::endl;
		}
		catch(uhd::runtime_error &e)
		{
			cout << "Exception occured " << e.code() << endl;
		}
	}
	
}


#if GPIO_EXISTS == 1

void get_gpio(radio::multi_usrp::sptr usrp, size_t mboard)
{
	using namespace std;

	// GPIOS BANKS

	std::cout << std::endl << "********** GPIO BANKS ***********" << std::endl;
	// Gpio Bank list
	cout << endl << "-----> Get GPIO Banks" << endl;
	vector<string> gpio_bank_list =	usrp->get_gpio_banks(mboard);
	cout << "GPIO Banks: " << std::endl;
	vector<string> attr_list;
	attr_list.push_back("CTRL");
	for (int index =0; index < gpio_bank_list.size(); index++)
	{
		// Name
		std::cout << "\t" << gpio_bank_list[index] << ":  ";
		// Value
		for (int aindex =0; aindex < attr_list.size(); aindex++)
		{
			try
			{
				cout << attr_list[aindex] << "  ";
				boost::uint32_t attribute =	usrp->get_gpio_attr(gpio_bank_list[index],attr_list[aindex], mboard);
				std::cout << attribute << std::endl;
			}
			catch(uhd::runtime_error &e)
			{
				cout << "Exception occured " << e.code() << endl;
			}
		}
	}
}
#endif