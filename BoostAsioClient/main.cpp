
#include <boost/thread/thread.hpp>

#include "client.h"
//#include "MultiClient.h"

using namespace std;
using namespace boost::asio;


void start(string ip, string path, string startTIme, string endTime, string pictureID, string errorCode, string zipCode) {

	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	auto endpointIterator = resolver.resolve({ "ngrok.cloudserver.xin", ip });
	Client client(ioService, endpointIterator, path, startTIme, endTime, pictureID, errorCode, zipCode);
	ioService.run();

}

void startSingleClient(vector<string> ipArr, string path, string startTime, string endTime, string pictureID, string errorCode, string zipCode) {
	vector<boost::thread> threads;
	for (auto ip : ipArr) {
		threads.push_back(boost::thread(boost::bind(&start, ip, path, startTime.replace(10, 1, "-"), endTime.replace(10, 1, "-"), pictureID, errorCode, zipCode)));
	}

	for (auto& thread : threads) {
		thread.join();
	}
}


int main(int argc, char* argv[])
{
	
	vector<string> ipArr;
	//ipArr.push_back("8082");
	ipArr.push_back("9090");
	ipArr.push_back("9094");
	ipArr.push_back("9092");
	ipArr.push_back("9091");
	//ipArr.push_back("127.0.0.1");
	//ipArr.push_back("10.59.13.178");
	//ipArr.push_back("10.59.13.212");
	//ipArr.push_back("10.59.13.193");
	//ipArr.push_back("10.59.13.222");
	//ipArr.push_back("10.59.13.219");
	//ipArr.push_back("1000");
	/*while (true) {
		char x = getchar();
		if (x == 'q') {
			startSingleClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
		}
		if (x == 'w') {
			startSingleClient(ipArr, "D:/test/server.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
		}
	}*/
	//参数：ip向量，图片路径，开始时间，截至时间，图片id
	//MultiClient multiClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
	startSingleClient(ipArr, "D:/test/xiaohan.jpg", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233", "0", "5768444");
	system("pause");

	return 0;
}