#include <iostream>
#include <thread>
#include <sstream>

#include <boost/asio.hpp>

//
// must go after boost/assio !!!
// Boost::asio winsock and winsock 2 compatibility issue
//
#include <windows.h>

//#include <QtCore/QString>

using namespace boost::asio;


namespace
{
	bool run = true;
	void shutdown(int)
	{
		run = false;
	}

	typedef int (__stdcall *f_funci)();
	int load_dll_and_get_version(int version)
	{
		char buffer[256];
		snprintf(buffer, 256, "artifacts\\shared_lib_v%d.dll", version);

		std::string fpath_dll = std::string(buffer);

		HINSTANCE hGetProcIDDLL = LoadLibraryA(fpath_dll.c_str());

		if (!hGetProcIDDLL) 
		{
			std::cout << "could not load the dynamic library" << std::endl;
			return EXIT_FAILURE;
		}

		// resolve function address here
		f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "get_version");
		if (!funci) 
		{
			std::cout << "could not locate the function" << std::endl;
			return EXIT_FAILURE;
		}

		return funci();

		//return EXIT_SUCCESS;
	}

	void pool() 
	{
		boost::asio::io_service io_service;

		int version = 1;

		while(run)
		{
			ip::tcp::socket socket(io_service);

			try {
				socket.connect( ip::tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 1234 ));

				// request/message from client
				const std::string msg = "version request\n";
				boost::system::error_code error;
				boost::asio::write( socket, boost::asio::buffer(msg), error );

				if( !error )
					std::cout << "client sent: version request" << std::endl;
				else
					std::cout << "send failed: " << error.message() << std::endl;

				// getting response from server
				boost::asio::streambuf receive_buffer;
				
				boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);

				if( error && error != boost::asio::error::eof )
					std::cout << "receive failed: " << error.message() << std::endl;
				else 
				{
					const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());

					std::cout << "client received: server version: " << data << std::endl;

					std::cout << "current client version: " << version << std::endl;

					int version_received;
					std::stringstream s(data);
					s >> version_received;

					if( version_received != version )
					{
						version = version_received;
						int dll_version = load_dll_and_get_version(version);
						std::cout << "new dll loaded: version: " << dll_version << std::endl << std::endl;
					}
					else
						std::cout << "NO new dll available" << std::endl << std::endl;
				}
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}

			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
		std::cout << "gracefull shutdown" << std::endl;
	}
}

int main() 
{
	//QString s = "QString";
	//std::cout << s.toStdString() << std::endl;

	signal(SIGINT, shutdown);

	std::thread t1(pool);
	t1.join();
	return 0;
}