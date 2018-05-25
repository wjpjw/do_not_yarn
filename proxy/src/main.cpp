//
// Created by jipeng on 5/22/18.
//

#include "wjp.h"
#include "Session.h"
#include "Poller.h"

#include <boost/program_options.hpp>


namespace po = boost::program_options;

int main(int argc, char** argv) {
    try
    {
        po::options_description desc{"Options"};
        desc.add_options()
                ("version,v",                      "print version number")
                ("help,h",                         "print help message")
                ("port,p",     po::value<int>(),   "set the port number")
                ("accept,a",   po::value<int>(),   "set acceptor port number");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, (const char** const)argv, desc), vm);
        po::notify(vm);
        if( vm.count("version") ){
            std::cout << PROGRAM_NAME << ": "<< VERSION <<std::endl;
            return EXIT_SUCCESS;
        }
        if ( vm.count("help") )
        {
            std::cout << PROGRAM_NAME <<"\n\nusage: proxy --port [port1] --acceptor [port2] \n\n" << desc << std::endl;
            return EXIT_SUCCESS;
        }
        int data_port = 5555;       // default port
        int control_port = 12345;   // default query-and-push interval
        if(vm.count("port")) data_port = vm["port"].as<int>();
        if(vm.count("accept")) control_port = vm["accept"].as<int>();
        /* application entry */

        test_poller();

    }
    catch(po::error& e)
    {
        std::cerr << "Program Option Command Error: " << e.what() << std::endl << std::endl;
        return EXIT_FAILURE;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

