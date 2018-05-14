#include <boost/program_options.hpp>
#include "wjp.h"
#include "world/Pusher.h"

namespace po = boost::program_options;

int main(int argc, char** argv) {
    try
    {
        po::options_description desc{"Options"};
        desc.add_options()
                ("version,v",                      "print version number")
                ("help,h",                         "print help message")
                ("port,p",     po::value<int>(),   "set the port number")
                ("interval,i", po::value<int>(),   "set query interval in second");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, (const char** const)argv, desc), vm);
        po::notify(vm);
        if( vm.count("version") ){
            std::cout << PROGRAM_NAME << ": "<< VERSION <<std::endl;
            return EXIT_SUCCESS;
        }
        if ( vm.count("help") )
        {
            std::cout << PROGRAM_NAME <<"\n\nusage: gpurealtime --port [port] --interval [query_interval] \n\n" << desc << std::endl;
            return EXIT_SUCCESS;
        }
        int port = 5555;    // default port
        int interval = 1;   // default query-and-push interval
        if(vm.count("port")) port = vm["port"].as<int>();
        if(vm.count("interval")) interval = vm["interval"].as<int>();
        /* application entry */
        Pusher{port, interval};
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

