#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <vector>

#include "client.h"

using namespace std;
using namespace boost::asio;


void start(string ip, string path, string startTIme, string endTime, string pictureID) {

	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	auto endpointIterator = resolver.resolve({ "127.0.0.1", ip });
	Client client(ioService, endpointIterator, path, startTIme, endTime, pictureID);
	ioService.run();

}

class MultiClient {
public:
	MultiClient(vector<string> ipArr, string path, string startTime, string endTime, string pictureID) {
		this->ipArr = ipArr;
		this->path = path;
		this->pictureID = pictureID;
		this->startTime = startTime.replace(10, 1, "*");
		this->endTime = endTime.replace(10, 1, "*");
		startSingleClient();
	}

private:
	vector<string> ipArr;
	vector<boost::thread> threads;
	std::string startTime;
	std::string endTime;
	std::string pictureID;
	std::string path;


	void startSingleClient() {
		
		
		for (auto ip : ipArr) {
			threads.push_back(boost::thread(boost::bind(&start, ip, path, startTime, endTime, pictureID)));
		}

		

		for (auto& thread : threads) {
			thread.join();
		}

		/*boost::thread thrd(boost::bind(&start, ipArr[0], "第二个参数"));
		boost::thread thrd1(boost::bind(&start, ipArr[1], "第二个参数"));
		thrd.join();
		thrd1.join();*/
	}
	

};



int main(int argc, char* argv[])
{
	
	/*std::string path = "D://test/test_copy.txt";
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	auto endpointIterator = resolver.resolve({ "127.0.0.1", "1000" });
	Client client(ioService, endpointIterator, path);
	ioService.run();*/

	

	vector<string> ipArr;
	ipArr.push_back("1000");
	//ipArr.push_back("1000");

	//cout << ipArr[0] << std::endl;
	

	
	MultiClient multiClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
	

	system("pause");

	return 0;
}