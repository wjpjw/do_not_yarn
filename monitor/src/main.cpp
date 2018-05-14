#include <boost/program_options.hpp>
#include "wjp.h"
#include "poll/Poller.h"

namespace po = boost::program_options;

int main (int argc, char** argv)
{
    try
    {
        po::options_description desc{"Options"};
        desc.add_options()
                ("version,v", "print version number")
                ("help,h",    "print help message")
                ("datasrc,d", po::value<Vector<String>>()->multitoken()->composing(),
                 "specify ip:port for at least one data source");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, (const char** const)argv, desc), vm);
        po::notify(vm);
        if ( vm.count("help") )
        {
            std::cout << PROGRAM_NAME <<"\n\nusage: monitor -d [ip1:port1] [ip2:port2] [ip3:port3] \n"
                    <<"or you can repeat -d: monitor -d [ip1:port1] -d [ip2:port2] \n\n"
                    << desc << std::endl;
            return EXIT_SUCCESS;
        }
        if( vm.count("version")){
            std::cout << PROGRAM_NAME << ": "<< VERSION <<std::endl;
            return EXIT_SUCCESS;
        }
        auto nr_datasrc=vm.count("datasrc");
        if(nr_datasrc){
            /* application entry */
            Poller{HEARTBEAT_INTERVAL, vm["datasrc"].as<Vector<String>>()};
        }
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
