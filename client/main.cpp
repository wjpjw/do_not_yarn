//
// Created by jipeng on 5/22/18.
//

//
// Created by jipeng on 5/22/18.
//

#include "wjp.h"
#include <future>

/*
 测试200个客户端，连续对proxy进行10次请求，形成潮涌。
 客户端测试线程会分别记录每次请求的回应消息与延时。
 */
void test_200_flux() {
    Vector<std::future<String>> tasks;
    for(int task_id=0;task_id<200;task_id++)
    {
        auto ten_requests=[task_id](){
            StringStream ss;
            zmq::context_t context(1);
            zmq::socket_t requester(context, ZMQ_REQ);
            requester.connect("tcp://localhost:5559");
            String id="Task"+boost::lexical_cast<String>(task_id);
            for( int request_id = 0; request_id < 10; request_id++) {
                auto start=linux_clock();
                s_send (requester, id+" says: "+boost::lexical_cast<String>(request_id));
                auto recvd = s_recv (requester);
                ss << "Response No. "<< request_id << " [" << recvd << "] "
                   << linux_clock()-start<<"ms\n";
            }
            return ss.str();
        };
        tasks.push_back(std::async(std::launch::async, ten_requests));
    }
    for(int i=0;i<tasks.size();i++)
    {
        std::cout<<i<<":"<<std::endl<<tasks[i].get()<<std::endl<<std::endl<<std::endl;
    }
}

/*
 测试400个客户端，每隔0~0.3秒（随机取值）对proxy进行一次访问，每个客户端进行10次请求。
 客户端测试线程会分别记录每次请求的回应消息与延时。
 */
void test_400_intense() {
    Vector<std::future<String>> tasks;
    for(int task_id=0;task_id<400;task_id++)
    {
        auto ten_requests=[task_id](){
            StringStream ss;
            zmq::context_t context(1);
            zmq::socket_t requester(context, ZMQ_REQ);
            requester.connect("tcp://localhost:5559");
            String id="Task"+boost::lexical_cast<String>(task_id);
            for( int request_id = 0; request_id < 10; request_id++) {
                auto start=linux_clock();
                s_send (requester, id+" says: "+boost::lexical_cast<String>(request_id));
                auto recvd = s_recv (requester);
                ss << "Response No. "<< request_id << " [" << recvd << "] "
                   << linux_clock()-start<<"ms\n";
                linux_sleep_msecs(within(300));
            }
            return ss.str();
        };
        tasks.push_back(std::async(std::launch::async, ten_requests));
    }
    for(int i=0;i<tasks.size();i++)
    {
        std::cout<<i<<":"<<std::endl<<tasks[i].get()<<std::endl<<std::endl<<std::endl;
    }
}

/*
 测试200个客户端，每隔0~0.3秒（随机取值）对proxy进行一次访问，每个客户端进行10次请求。
 客户端测试线程会分别记录每次请求的回应消息与延时。
 */
void test_200_intense(){
    Vector<std::future<String>> tasks;
    for(int task_id=0;task_id<200;task_id++)
    {
        auto ten_requests=[task_id](){
            StringStream ss;
            zmq::context_t context(1);
            zmq::socket_t requester(context, ZMQ_REQ);
            requester.connect("tcp://localhost:5559");
            String id="Task"+boost::lexical_cast<String>(task_id);
            for( int request_id = 0; request_id < 10; request_id++) {
                auto start=linux_clock();
                s_send (requester, id+" says: "+boost::lexical_cast<String>(request_id));
                auto recvd = s_recv (requester);
                ss << "Response No. "<< request_id << " [" << recvd << "] "
                   << linux_clock()-start<<"ms\n";
                linux_sleep_msecs(within(300));
            }
            return ss.str();
        };
        tasks.push_back(std::async(std::launch::async, ten_requests));
    }
    for(int i=0;i<tasks.size();i++)
    {
        std::cout<<i<<":"<<std::endl<<tasks[i].get()<<std::endl<<std::endl<<std::endl;
    }
}


/*
 测试200个客户端，每隔0~3秒（随机取值）对proxy进行一次访问，每个客户端进行10次请求。
 客户端测试线程会分别记录每次请求的回应消息与延时。
 */
void test_200(){
    Vector<std::future<String>> tasks;
    for(int task_id=0;task_id<200;task_id++)
    {
        auto ten_requests=[task_id](){
            StringStream ss;
            zmq::context_t context(1);
            zmq::socket_t requester(context, ZMQ_REQ);
            requester.connect("tcp://localhost:5559");
            String id="Task"+boost::lexical_cast<String>(task_id);
            for( int request_id = 0; request_id < 10; request_id++) {
                auto start=linux_clock();
                s_send (requester, id+" says: "+boost::lexical_cast<String>(request_id));
                auto recvd = s_recv (requester);
                ss << "Response No. "<< request_id << " [" << recvd << "] "
                   << linux_clock()-start<<"ms\n";
                linux_sleep_msecs(within(3000));
            }
            return ss.str();
        };
        tasks.push_back(std::async(std::launch::async, ten_requests));
    }
    for(int i=0;i<tasks.size();i++)
    {
        std::cout<<i<<":"<<std::endl<<tasks[i].get()<<std::endl<<std::endl<<std::endl;
    }
}



void test_tmp() {
    Vector<std::future<String>> tasks;
    for(int task_id=0;task_id<120;task_id++)
    {
        auto ten_requests=[task_id](){
            StringStream ss;
            zmq::context_t context(1);
            zmq::socket_t requester(context, ZMQ_REQ);
            requester.connect("tcp://localhost:5559");
            String id="Task"+boost::lexical_cast<String>(task_id);
            for( int request_id = 0; request_id < 10; request_id++) {
                auto start=linux_clock();
                s_send (requester, id+" says: "+boost::lexical_cast<String>(request_id));
                auto recvd = s_recv (requester);
                ss << "Response No. "<< request_id << " [" << recvd << "] "
                   << linux_clock()-start<<"ms\n";
                //linux_sleep_msecs(within(1000));
            }
            return ss.str();
        };
        tasks.push_back(std::async(std::launch::async, ten_requests));
    }
    for(int i=0;i<tasks.size();i++)
    {
        std::cout<<i<<":"<<std::endl<<tasks[i].get()<<std::endl<<std::endl<<std::endl;
    }
}

int main (int argc, char** argv)
{
    try
    {
        test_tmp();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}
