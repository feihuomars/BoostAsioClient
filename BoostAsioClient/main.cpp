
#include "client.h"
#include "MultiClient.h"

using namespace std;
using namespace boost::asio;


int main(int argc, char* argv[])
{
	
	vector<string> ipArr;
	ipArr.push_back("127.0.0.1");
	//ipArr.push_back("1000");
	/*while (true) {
		char x = getchar();
		if (x != 'q') {
			MultiClient multiClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");
		}
	}*/
	//参数：ip向量，图片路径，开始时间，截至时间，图片id
	MultiClient multiClient(ipArr, "D:/test/test.txt", "2017-11-11 11:11:11", "2017-12-12 12:12:12", "233");

	system("pause");

	return 0;
}