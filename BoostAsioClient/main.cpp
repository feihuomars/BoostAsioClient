
#include <boost/thread/thread.hpp>

#include "client.h"
//#include "MultiClient.h"

using namespace std;
using namespace boost::asio;


void start(string ip, string path, string startTIme, string endTime, string pictureID) {

	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	auto endpointIterator = resolver.resolve({ ip, "1000" });
	Client client(ioService, endpointIterator, path, startTIme, endTime, pictureID);
	ioService.run();

}

void startSingleClient(vector<string> ipArr, string path, string startTime, string endTime, string pictureID) {
	vector<boost::thread> threads;
	for (auto ip : ipArr) {
		threads.push_back(boost::thread(boost::bind(&start, ip, path, startTime.replace(10, 1, "*"), endTime.replace(10, 1, "*"), pictureID)));
	}

	for (auto& thread : threads) {
		thread.join();
	}
}


int main(int argc, char* argv[])
{
	
	vector<string> ipArr;
	ipArr.push_back("127.0.0.1");
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
	//������ip������ͼƬ·������ʼʱ�䣬����ʱ�䣬ͼƬid
	//MultiClient multiClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
	startSingleClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
	system("pause");

	return 0;
}