#include "MultiClient.h"

MultiClient::MultiClient(vector<string> ipArr, string path, string startTime, string endTime, string pictureID) {
	this->ipArr = ipArr;
	this->path = path;
	this->pictureID = pictureID;
	this->startTime = startTime.replace(10, 1, "*");
	this->endTime = endTime.replace(10, 1, "*");
	startSingleClient();
}

void start(string ip, string path, string startTIme, string endTime, string pictureID) {

	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	auto endpointIterator = resolver.resolve({ ip, "1000" });
	Client client(ioService, endpointIterator, path, startTIme, endTime, pictureID);
	ioService.run();

}

void MultiClient::startSingleClient() {
	for (auto ip : ipArr) {
		threads.push_back(boost::thread(boost::bind(&start, ip, path, startTime, endTime, pictureID)));
	}

	for (auto& thread : threads) {
		thread.join();
	}
}