//
// Created by jipeng on 5/22/18.
//

//
// Created by jipeng on 5/22/18.
//

#include "wjp.h"
//  Receive 0MQ string from socket and convert into string
static std::string s_recv (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char*>(message.data()), message.size());
}

//  Convert string to 0MQ string and send to socket
static bool s_send (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message);
    return (rc);
}

//  Sends string as 0MQ string, as multipart non-terminal
static bool s_sendmore (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message, ZMQ_SNDMORE);
    return (rc);
}
int main (int argc, char** argv)
{
    try
    {
        zmq::context_t context(1);

        zmq::socket_t requester(context, ZMQ_REQ);
        requester.connect("tcp://localhost:"+boost::lexical_cast<String>(CONTROL_PORT));

        for( int request = 0 ; request < 10 ; request++) {

            s_send (requester, "Hello");
            std::string string = s_recv (requester);

            std::cout << "Received reply " << request
                      << " [" << string << "]" << std::endl;
        }


    }
    catch(const std::exception& e)
    {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}
