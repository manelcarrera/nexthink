#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

#include <thread>

namespace
{
    std::string read_(ip::tcp::socket & socket) 
    {
        boost::asio::streambuf buf;
        boost::asio::read_until( socket, buf, "\n" );
        std::string data = boost::asio::buffer_cast<const char*>(buf.data());
        return data;
    }

    void send_(ip::tcp::socket & socket, const std::string& message) 
    {
        const std::string msg = message + "\n";
        boost::asio::write( socket, boost::asio::buffer(message) );
    }

    void listen_() 
    {
        boost::asio::io_service io_service;

        int version = 1;

        int count = 1;

        while(1)
        {
            //listen for new connection
            ip::tcp::acceptor acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), 1234));

            ip::tcp::socket socket_(io_service);

            //waiting for connection
            acceptor_.accept(socket_);

            std::string msg = read_(socket_);

            msg.erase(msg.length()-1);

            std::cout << "server received: " << msg << std::endl;

            send_(socket_, std::to_string(version));
            
            std::cout << "server sent: " << version << std::endl << std::endl;

            version = count++ < 3 ? 1 : 2;
        }
        std::cout << "gracefull shutdown" << std::endl;
    }
}

int main() 
{
    std::thread t1(listen_);
    t1.join();
    return 0;
}