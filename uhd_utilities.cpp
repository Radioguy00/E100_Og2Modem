#include "uhd_utilities.h"


/*************************************************************************//**
@brief Display an uhd::rx_metadata_t structure

This structure provides information on the data received from the USRP

@param md metadata to display
@param os output stream when the data is displayed. std::cout is the default

*****************************************************************************/

void display_rx_metadata(uhd::rx_metadata_t & md, std::ostream & os)
{
	using namespace uhd;
	os << "Has time spec? " << md.has_time_spec <<std::endl;
	if(md.has_time_spec)
	{
		//os << "\tFull Seconds: " << md.time_spec.get_full_secs() << std::endl << "Fractional Seconds: " << md.time_spec.get_frac_secs() <<  std::endl;
		os << "\tSeconds " << md.time_spec.get_real_secs() << std::endl;

	}
	os << "More fragments? " << md.more_fragments <<std::endl;
	os << "Fragment Offset: " << md.fragment_offset <<std::endl;
	os << "Start of Burst? " << md.start_of_burst <<std::endl;
	os << "End of Burst? " << md.end_of_burst <<std::endl;
	//os << "Out of Sequence? " << md.out_of_sequence <<std::endl;
	switch(md.error_code)
	{
	case rx_metadata_t::ERROR_CODE_NONE:
		os << "Error : " << "None" <<std::endl;
		break;
	case rx_metadata_t::ERROR_CODE_TIMEOUT:
		os << "Error : " << "Timeout" <<std::endl;
		break;
	case rx_metadata_t::ERROR_CODE_LATE_COMMAND:
		os << "Error : " << "Late Command" <<std::endl;
		break;
	case rx_metadata_t::ERROR_CODE_BROKEN_CHAIN:
		os << "Error : " << "Broken Chain" <<std::endl;
		break;
	case rx_metadata_t::ERROR_CODE_OVERFLOW:
		os << "Error : " << "Overflow" <<std::endl;
		break;
	case rx_metadata_t::ERROR_CODE_ALIGNMENT:
		os << "Error : " << "Alignment" <<std::endl;
		break;
	case rx_metadata_t::ERROR_CODE_BAD_PACKET:
		os << "Error : " << "Bad Packet" <<std::endl;
		break;
	}
}


/*************************************************************************//**
@brief Display all the receiver settings of an USRP board


@param usrp Pointer to the multi_usrp object which can represents multiple boards
@param mboard Board number. 0 is the default value
@param os output stream when the data is displayed. std::cout is the default value

*****************************************************************************/

void get_rx_parameters(uhd::usrp::multi_usrp::sptr usrp, size_t mboard , std::ostream & os)
{
	using namespace std;
	namespace radio = uhd::usrp;
	size_t nchan = 0;

	// Get sub device specification
	os << std::endl << "********** RX Sub Device ***********" << std::endl;
	os << std::endl << "-----> Get Rx Subdevice" << std::endl;
	radio::subdev_spec_t rx_subdev = usrp->get_rx_subdev_spec(mboard);
	os << "RX Subdevice Specification:" << endl;
	os << rx_subdev.to_pp_string() << endl;
	
	os << std::endl << "-----> Get number of RX channels" << std::endl;
	size_t num_rx = usrp->get_rx_num_channels();
	os << "Number of RX channels:" << endl;
	os << num_rx << endl;
	
	os << std::endl << "-----> Get RX Subdevice Name" << std::endl;
	string rx_name = usrp->get_rx_subdev_name(nchan);
	os << "RX Subdevice Name:" << endl;
	os << rx_name << endl;

	os << std::endl << "********** RX Sample Rate ***********" << std::endl;
	os << std::endl << "-----> Get RX Rate" << std::endl;
	double rx_rate = usrp->get_rx_rate(nchan);
	os << "RX Rate:" << endl;
	os << rx_rate << endl;
	
	os << std::endl << "-----> Get RX Rate List" << std::endl;
	uhd::meta_range_t rx_rates = usrp->get_rx_rates(nchan);
	os << "RX Rate List:" << endl;
	os << "Start: " << rx_rates.start() << "   Stop: " << rx_rates.stop() << "   Step: " << rx_rates.step() << endl;
	os << rx_rates.to_pp_string() << endl;
	
	// RX FREQUENCIES
	
	os << std::endl << "********** RX Frequencies ***********" << std::endl;
	os << std::endl << "-----> Get RX Center Frequency" << std::endl;
	double rx_freq = usrp->get_rx_freq(nchan);
	os << "RX Freq:" << endl;
	os << rx_freq << endl;

	os << std::endl << "-----> Get RX Center Frequency Range" << std::endl;
	uhd::freq_range_t rx_freq_range = usrp->get_rx_freq_range(nchan);
	os << "RX Frequency Range:" << endl;
	os << "Start: " << rx_freq_range.start() << "   Stop: " << rx_freq_range.stop() << "   Step: " << rx_freq_range.step() << endl;
	os << rx_freq_range.to_pp_string() << endl;
	
	// RX Front end frequencies
	os << std::endl << "-----> Get RX RF Front End Center Frequency Range" << std::endl;
	try
	{
		os << "RX Front End Frequency Range:" << endl;
		uhd::freq_range_t rx_fe_freq_range = usrp->get_fe_rx_freq_range(nchan);
		os << "Start: " << rx_fe_freq_range.start() << "   Stop: " << rx_fe_freq_range.stop() << "   Step: " << rx_fe_freq_range.step() << endl;
		os << rx_fe_freq_range.to_pp_string() << endl;
	}
		catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}
	
	// RX GAIN
	
	os << std::endl << "********** RX Gain  ***********" << std::endl;
	
	// Total combined gain
	os << endl << "-----> Get RX Total Gain" << endl;
	os << "RX Total Gain: " ;	
	try
	{
		double rx_total_gain = usrp->get_rx_gain(nchan);
		os << rx_total_gain << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() << endl;
	}
	
	// List of all gain elements
	os << std::endl << "-----> Get RX gain names" << std::endl;
	std::vector<std::string> rx_gain_names = usrp->get_rx_gain_names(nchan);
	os << "Rx Gain Names: " << std::endl;
	for (int index =0; index < rx_gain_names.size(); index++)
	{
		// Name
		os << "\t" << rx_gain_names[index] << endl;
	}
	for (int index =0; index < rx_gain_names.size(); index++)
	{
		// Name
		os << "\t" << "Name: " << rx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		double element_gain = usrp->get_rx_gain(rx_gain_names[index], nchan);
		os << element_gain << endl;
		}
		catch(uhd::runtime_error &e)
		{
			os << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Gain ranges for each of the gain elements
	os << std::endl << "-----> Get RX element gain ranges" << std::endl;
	for (int index =0; index < rx_gain_names.size(); index++)
	{
		// Name
		os << "\t" << "Name: " << rx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		uhd::gain_range_t element_gain_range = usrp->get_rx_gain_range(rx_gain_names[index], nchan);
		os << "Start: " << element_gain_range.start() << " End: " << element_gain_range.stop() << " Step: " << element_gain_range.step() << endl;
		}
		catch(uhd::runtime_error &e)
		{
			os << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Total Gain range
	try
	{
		os << endl << "-----> Get RX Total Gain Range" << endl;
		uhd::gain_range_t rx_total_gain_range = usrp->get_rx_gain_range(nchan);
		os << "RX Total Gain Range: " ;
		os << "Start: " << rx_total_gain_range.start() << " End: " << rx_total_gain_range.stop() << " Step: " << rx_total_gain_range.step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() <<endl;
	}
	
	// ANTENNA FUNCTIONS
	
	os << std::endl << "********** RX ANTENNA ***********" << std::endl;
	// Current Rx Antenna
	os << std::endl << "-----> Get RX Antenna" << std::endl;
	string rx_antenna = usrp->get_rx_antenna(nchan);
	os << "RX Antenna: " ;
	os << rx_antenna << endl;

	// RX Antenna choices
	os << std::endl << "-----> Get Rx Antenna List" << std::endl;
	std::vector<std::string> rx_antennas = usrp->get_rx_antennas(nchan);
	os << "RX Antennas : " << std::endl;
	for (int index =0; index < rx_antennas.size(); index++)
		os << "\t" << rx_antennas[index] << std::endl;
		
		
	// RX BANDWIDTH FUNCTIONS

	os << std::endl << "********** RX BANDWIDTH ***********" << std::endl;
	// Current RX Bandwidth
	os << endl << "-----> Get RX Bandwidth" << endl;
	try
	{
		os << "RX Bandwidth " ;
		double rx_bandwidth = usrp->get_rx_bandwidth(nchan);
		os << rx_bandwidth << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << "Exception occured " << e.code() << endl;
	}
	
	// RX Bandwidth Range
	os << endl << "-----> Get RX Bandwidth Range" << endl;
	try
	{
		os << "RX Bandwidth Range: " ;
		uhd::gain_range_t rx_bandwidth_range = usrp->get_rx_bandwidth_range(nchan);
		os << "Start: " << rx_bandwidth_range .start() << " End: " << rx_bandwidth_range .stop() << " Step: " << rx_bandwidth_range .step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() <<endl;
	}
	
	// RX DBOARD INTERFACE OBJECT

	os << std::endl << "********** RX DBOARD INTERFACE ***********" << std::endl;
	// RX Dboard Interface
	os << endl << "-----> Get rx_dboard_iface()" << endl;
	try
	{
		os << "RX Dboard Interface " ;
		uhd::usrp::dboard_iface::sptr rx_dboard_iface = usrp->get_rx_dboard_iface(nchan);
		os << rx_dboard_iface << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << "Exception occured " << e.code() << endl;
	}
	
	// RX _SENSORS

	os << std::endl << "********** RX Sensors  ***********" << std::endl;

	// List of all available sensors
	os << std::endl << "-----> Get RX Sensors Name" << std::endl;
	std::vector<std::string> rx_sensor_names = usrp->get_rx_sensor_names(nchan);
	os << "Sensor Names: " << std::endl;
	for (int index =0; index < rx_sensor_names.size(); index++)
	{
		// Name
		os << "\t" << rx_sensor_names[index] << ":  ";
		// Value
		try
		{
			uhd::sensor_value_t rx_sensor_value = usrp->get_rx_sensor(rx_sensor_names[index], nchan);
			os << rx_sensor_value.to_pp_string()<< std::endl;
		}
		catch(uhd::runtime_error &e)
		{
			os << "Exception occured " << e.code() << endl;
		}
	}
	

}

/*************************************************************************//**
@brief Display all the transmitter settings of an USRP board


@param usrp Pointer to the multi_usrp object which can represents multiple boards
@param mboard Board number - 0 is the default value
@param os output stream when the data is displayed. std::cout is the default value

*****************************************************************************/

void get_tx_parameters(uhd::usrp::multi_usrp::sptr usrp, size_t mboard, std::ostream & os)
{
	using namespace std;
	namespace radio = uhd::usrp;

	size_t nchan = 0;

	// CONFIGURATION SUB_DEVICE
	
	os << std::endl << "********** TX Sub Device ***********" << std::endl;
	// Get sub device specification
	os << std::endl << "-----> Get TX Subdevice" << std::endl;
	try
	{
	os << "TX Subdevice Specification: ";
	radio::subdev_spec_t tx_subdev = usrp->get_tx_subdev_spec(mboard);
	os << tx_subdev.to_pp_string() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}
	
	// Number of tx channels
	os << std::endl << "-----> Get number of TX channels" << std::endl;
	try
	{
	size_t num_tx = usrp->get_tx_num_channels();
	os << "Number of TX channels: " ;
	os << num_tx << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}
	
	// TX Device Name
	os << std::endl << "-----> Get TX Subdevice Name" << std::endl;
	try
	{
	os << "TX Subdevice Name: ";
	string tx_name = usrp->get_tx_subdev_name(nchan);
	os << tx_name << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}
	
	// TX SAMPLE RATE
	
	os << std::endl << "********** TX Sample Rate ***********" << std::endl;
	
	// Get Current TX rate
	os << std::endl << "-----> Get TX Rate" << std::endl;
	try
	{
		os << "TX Rate: " ;
		double tx_rate = usrp->get_tx_rate(nchan);
		os << tx_rate << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}
	
	// Get list of TX rates
	os << std::endl << "-----> Get TX Rate List" << std::endl;
	try
	{
		os << "TX Rate List: " ;
		uhd::meta_range_t tx_rates = usrp->get_tx_rates(nchan);
		os << "Start: " << tx_rates.start() << "   Stop: " << tx_rates.stop() << "   Step: " << tx_rates.step() << endl;
		os << tx_rates.to_pp_string() << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}

	// TX FREQUENCIES
	
	os << std::endl << "********** TX Frequencies ***********" << std::endl;
	
	// Current TX frequency
	os << std::endl << "-----> Get TX Center Frequency" << std::endl;
	try
	{
		os << "TX Freq: ";
		double tx_freq = usrp->get_tx_freq(nchan);
		os << tx_freq << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}

	// TX Frequency Range
	os << std::endl << "-----> Get TX Center Frequency Range" << std::endl;
	try
	{
		os << "TX Frequency Range: ";
		uhd::freq_range_t tx_freq_range = usrp->get_tx_freq_range(nchan);
		os << "Start: " << tx_freq_range.start() << "   Stop: " << tx_freq_range.stop() << "   Step: " << tx_freq_range.step() << endl;
		os << tx_freq_range.to_pp_string() << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}
	
	// Front end TX frequency range
	os << std::endl << "-----> Get TX RF Front End Center Frequency Range" << std::endl;
	try
	{
		os << "TX Front End Frequency Range: ";
		uhd::freq_range_t tx_fe_freq_range = usrp->get_fe_tx_freq_range(nchan);
		os << "Start: " << tx_fe_freq_range.start() << "   Stop: " << tx_fe_freq_range.stop() << "   Step: " << tx_fe_freq_range.step() << endl;
		os << tx_fe_freq_range.to_pp_string() << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << " Exception occurred : " << e.code() << endl;
	}

	// TX GAIN

	os << std::endl << "********** TX Gain  ***********" << std::endl;
	
	// Total combined gain
	os << endl << "-----> Get TX Total Gain" << endl;
	os << "TX Total Gain: " ;	
	try
	{
		double tx_total_gain = usrp->get_tx_gain(nchan);
		os << tx_total_gain << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() << endl;
	}
	
	// List of all gain elements
	os << std::endl << "-----> Get TX gain names" << std::endl;
	std::vector<std::string> tx_gain_names = usrp->get_tx_gain_names(nchan);
	os << "Tx Gain Names: " << std::endl;
	for (int index =0; index < tx_gain_names.size(); index++)
	{
		// Name
		os << "\t" << tx_gain_names[index] << endl;
	}
	for (int index =0; index < tx_gain_names.size(); index++)
	{
		// Name
		os << "\t" << "Name: " << tx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		double element_gain = usrp->get_tx_gain(tx_gain_names[index], nchan);
		os << element_gain << endl;
		}
		catch(uhd::runtime_error &e)
		{
			os << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Gain ranges for each of the gain elements
	os << std::endl << "-----> Get TX element gain ranges" << std::endl;
	for (int index =0; index < tx_gain_names.size(); index++)
	{
		// Name
		os << "\t" << "Name: " << tx_gain_names[index] << "  Value: ";
		// Value
		try
		{
		uhd::gain_range_t element_gain_range = usrp->get_tx_gain_range(tx_gain_names[index], nchan);
		os << "Start: " << element_gain_range.start() << " End: " << element_gain_range.stop() << " Step: " << element_gain_range.step() << endl;
		}
		catch(uhd::runtime_error &e)
		{
			os << "Exception code while getting value: " << e.code() << endl;
		}
	}

	// Total Gain range
	try
	{
		os << endl << "-----> Get TX Total Gain Range" << endl;
		uhd::gain_range_t tx_total_gain_range = usrp->get_tx_gain_range(nchan);
		os << "TX Total Gain Range: " ;
		os << "Start: " << tx_total_gain_range.start() << " End: " << tx_total_gain_range.stop() << " Step: " << tx_total_gain_range.step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() <<endl;
	}

	// ANTENNA FUNCTIONS
	
	os << std::endl << "********** TX ANTENNA ***********" << std::endl;
	// Current Tx Antenna
	os << std::endl << "-----> Get TX Antenna" << std::endl;
	try
	{
	os << "TX Antenna: " ;
	string tx_antenna = usrp->get_tx_antenna(nchan);
	os << tx_antenna << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() <<endl;
	}

	// TX Antenna choices
	os << std::endl << "-----> Get Tx Antenna List" << std::endl;
	try
	{
	os << "TX Antennas : " << std::endl;
	std::vector<std::string> tx_antennas = usrp->get_tx_antennas(nchan);
	for (int index =0; index < tx_antennas.size(); index++)
		os << "\t" << tx_antennas[index] << std::endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() <<endl;
	}
		
	// TX BANDWIDTH FUNCTIONS

	os << std::endl << "********** TX BANDWIDTH ***********" << std::endl;
	// Current TX Bandwidth
	os << endl << "-----> Get TX Bandwidth" << endl;
	try
	{
		os << "TX Bandwidth " ;
		double tx_bandwidth = usrp->get_tx_bandwidth(nchan);
		os << tx_bandwidth << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << "Exception occured " << e.code() << endl;
	}
	
	// TX Bandwidth Range
	os << endl << "-----> Get TX Bandwidth Range" << endl;
	try
	{
		os << "TX Bandwidth Range: " ;
		uhd::gain_range_t tx_bandwidth_range = usrp->get_tx_bandwidth_range(nchan);
		os << "Start: " << tx_bandwidth_range .start() << " End: " << tx_bandwidth_range .stop() << " Step: " << tx_bandwidth_range .step() << endl;
	}
	catch(uhd::runtime_error &e)
	{
		os << "Exception code: " << e.code() <<endl;
	}
		
	// TX DBOARD INTERFACE OBJECT

	os << std::endl << "********** TX DBOARD INTERFACE ***********" << std::endl;
	// TX Dboard Interface
	os << endl << "-----> Get tx_dboard_iface()" << endl;
	try
	{
		os << "TX Dboard Interface " ;
		uhd::usrp::dboard_iface::sptr tx_dboard_iface = usrp->get_tx_dboard_iface(nchan);
		os << tx_dboard_iface << endl;
	}
	catch (uhd::runtime_error &e)
	{
		os << "Exception occured " << e.code() << endl;
	}
	
	// TX _SENSORS

	os << std::endl << "********** TX Sensors  ***********" << std::endl;

	// List of all available sensors
	os << std::endl << "-----> Get TX Sensors Name" << std::endl;
	std::vector<std::string> tx_sensor_names = usrp->get_tx_sensor_names(nchan);
	os << "Sensor Names: " << std::endl;
	for (int index =0; index < tx_sensor_names.size(); index++)
	{
		// Name
		os << "\t" << tx_sensor_names[index] << ":  ";
		// Value
		try
		{
			uhd::sensor_value_t tx_sensor_value = usrp->get_tx_sensor(tx_sensor_names[index], nchan);
			os << tx_sensor_value.to_pp_string()<< std::endl;
		}
		catch(uhd::runtime_error &e)
		{
			os << "Exception occured " << e.code() << endl;
		}
	}
	
}


/*************************************************************************//**
@brief Display information about the GPIO outputs of the USRP

@param usrp Pointer to the multi_usrp object which can represents multiple boards
@param mboard Board number - 0 is the default value
@param os output stream when the data is displayed. std::cout is the default value

The functionnality only exists for UHD driver versions above TBD

*****************************************************************************/

#if GPIO_EXISTS == 1

void get_gpio_parameters(radio::multi_usrp::sptr usrp, size_t mboard, std::ostream & os)
{
	using namespace std;

	// GPIOS BANKS

	os << std::endl << "********** GPIO BANKS ***********" << std::endl;
	// Gpio Bank list
	os << endl << "-----> Get GPIO Banks" << endl;
	vector<string> gpio_bank_list =	usrp->get_gpio_banks(mboard);
	os << "GPIO Banks: " << std::endl;
	vector<string> attr_list;
	attr_list.push_back("CTRL");
	for (int index =0; index < gpio_bank_list.size(); index++)
	{
		// Name
		os << "\t" << gpio_bank_list[index] << ":  ";
		// Value
		for (int aindex =0; aindex < attr_list.size(); aindex++)
		{
			try
			{
				os << attr_list[aindex] << "  ";
				boost::uint32_t attribute =	usrp->get_gpio_attr(gpio_bank_list[index],attr_list[aindex], mboard);
				os << attribute << std::endl;
			}
			catch(uhd::runtime_error &e)
			{
				os << "Exception occured " << e.code() << endl;
			}
		}
	}
}

#endif