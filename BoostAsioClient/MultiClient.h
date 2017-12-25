#pragma once
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

class MultiClient {
public:
	MultiClient(vector<string> ipArr, string path, string startTime, string endTime, string pictureID);

private:
	vector<string> ipArr;
	vector<boost::thread> threads;
	std::string startTime;
	std::string endTime;
	std::string pictureID;
	std::string path;

	void startSingleClient();
};