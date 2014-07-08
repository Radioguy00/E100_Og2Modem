#ifndef UHD_UTILITIES_H
#define UHD_UTILITIES_H

#include "/usr/include/uhd/usrp/multi_usrp.hpp"
#include <cstdio>
#include <vector>
#include <ostream>
#include <iostream>

#define UHD_MAJOR_VERSION 3
#define UHD_CENTER_VERSION  5
#define UHD_MINOR_VERSION 3

#define GPIO_EXISTS (UHD_MAJOR_VERSION >= 3 && UHD_CENTER_VERSION > 5 && UHD_MINOR_VERSION >3)


void display_rx_metadata(uhd::rx_metadata_t & md, std::ostream & os = std::cout);
void get_gpio_parameters(uhd::usrp::multi_usrp::sptr usrp, size_t mboard = 0, std::ostream & os = std::cout);
void get_rx_parameters(uhd::usrp::multi_usrp::sptr usrp, size_t mboard = 0, std::ostream & os = std::cout);
void get_tx_parameters(uhd::usrp::multi_usrp::sptr usrp, size_t mboard = 0, std::ostream & os = std::cout);

#endif