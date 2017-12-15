#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/system/error_code.hpp>
#include <boost/bind/bind.hpp>
#include <fstream>
#include <boost/filesystem.hpp>

#include "client.h"

using namespace std;


using namespace boost::asio;

class Client1
{
	typedef ip::tcp::socket socket_type;
	typedef std::shared_ptr<socket_type> sock_ptr;
public:
	Client1() :m_ep(ip::address::from_string("127.0.0.1"), 1000)
	{
		start();
	}
	void run()
	{
		m_io.run();
	}
private:
	void start()
	{
		sock_ptr sock(new socket_type(m_io));
		sock->async_connect(m_ep, std::bind(&Client1::connect_handler, this, std::placeholders::_1, sock));
	}

	void connect_handler(const boost::system::error_code& ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		std::cout << "receive from:" << sock->remote_endpoint().address() << std::endl;
		
		sock->async_read_some(buffer(m_buf),
			std::bind(&Client1::read_handler, this, std::placeholders::_1, sock));
	}

	void read_handler(const boost::system::error_code& ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		//std::cout << "time: " << m_buf << std::endl;
		sock->async_read_some(buffer(m_buf),
			std::bind(&Client1::read_handler2, this, std::placeholders::_1, sock));

	}

	void read_handler2(const boost::system::error_code& ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		//std::cout << "place: " << m_buf << std::endl;
		sock->async_read_some(buffer(m_buf),
			std::bind(&Client1::read_handler3, this, std::placeholders::_1, sock));

	}

	void read_handler3(const boost::system::error_code& ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		std::cout << "buffer: " << m_buf << std::endl;
		

	}

private:
	io_service m_io;
	ip::tcp::endpoint m_ep;
	enum { max_length = 1024 };
	char m_buf[max_length];
	
};


class client
{
	typedef client this_type;
	typedef ip::tcp::acceptor acceptor_type;
	typedef ip::tcp::endpoint endpoint_type;
	typedef ip::tcp::socket socket_type;
	typedef ip::address address_type;
	typedef boost::shared_ptr<socket_type> sock_ptr;
	typedef vector<char> buffer_type;

private:
	io_service m_io;
	buffer_type m_buf;
	endpoint_type m_ep;
	char buf1[1024];
	char buf2[1024];
	char buf3[1024];
	int i;
public:
	client() : m_buf(100, 0), m_ep(address_type::from_string("127.0.0.1"), 1000), i(0)
	{
		start();
	}

	void run()
	{
		m_io.run();
	}

	void start()
	{
		sock_ptr sock(new socket_type(m_io));
		sock->async_connect(m_ep, boost::bind(&this_type::conn_handler, this, boost::asio::placeholders::error, sock));
	}

	void conn_handler(const boost::system::error_code&ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		cout << "Receive from " << sock->remote_endpoint().address() << ": " << endl;
		
		sock->async_write_some(buffer("send to server"), boost::bind(&client::write_handler, this, boost::asio::placeholders::error, sock));
		sock->async_read_some(buffer(buf1), boost::bind(&client::read_handler, this, boost::asio::placeholders::error, sock));
		cout << "buf1 before handle: " << buf1 << endl;
	}

	void read_handler(const boost::system::error_code&ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		sock->async_read_some(buffer(buf2), boost::bind(&client::read_handler2, this, boost::asio::placeholders::error, sock));
		cout << "buf2 before handle: " << buf2 << endl;
		cout << "buf1: "<< buf1 << endl;
	}

	void read_handler2(const boost::system::error_code&ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		sock->async_read_some(buffer(buf3), boost::bind(&client::read_handler3, this, boost::asio::placeholders::error, sock));
		cout << "buf3 before handle: " << buf3 << endl;
		cout << "buf2: " << buf2 << endl;
	}

	void read_handler3(const boost::system::error_code&ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		//sock->async_read_some(buffer(buf), boost::bind(&client::read_handler4, this, boost::asio::placeholders::error, sock));
		cout << "buf3: " << buf3 <<endl;
	}

	void read_handler4(const boost::system::error_code&ec, sock_ptr sock)
	{
		if (ec)
		{
			return;
		}
		
		cout << "file received "  << endl;
	}


	void write_handler(const boost::system::error_code&ec, sock_ptr sock) {
		if (ec)
		{
			std::cout << "发送失败" << endl;
		}
		else {
			std::cout << "发送成功" << endl;
		}

	}

};



int main(int argc, char* argv[])
{
	

	//// 所有asio类都需要io_service对象
	//io_service iosev;
	//// socket对象
	//ip::tcp::socket socket(iosev);
	//// 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
	//ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"), 1000);
	//// 连接服务器
	//boost::system::error_code ec;
	//socket.connect(ep, ec);
	//// 如果出错，打印出错信息
	//if (ec)
	//{
	//	std::cout << boost::system::system_error(ec).what() << std::endl;
	//	system("pause");
	//	return -1;
	//}
	//// 接收数据
	//char buf[100];
	//size_t len = socket.read_some(buffer(buf), ec);
	//std::cout.write(buf, len);
	////std::cout << buf << std::endl;
	//getchar();
	

	//try
	//{
	//	std::cout << "client start." << std::endl;
	//	/*Client1 cl;
	//	cl.run();*/

	//	client cl;
	//	cl.run();

	//}
	//catch (std::exception& e)
	//{
	//	std::cout << e.what() << std::endl;
	//}
	//getchar();

	//std::string path = "D://test/file_test_copy.t";
	//std::ifstream fileStream;
	//fileStream.open(path, std::ios_base::binary);
	//if (fileStream.fail()) {
	//	cout << "Faiked to open file" + path << endl;
	//}
	//else {
	//	cout << "File open successfully" << endl;
	//}
	//fileStream.seekg(0, fileStream.end);	//文件指针移动至末尾
	//std::streampos sp = fileStream.tellg();
	//fileStream.seekg(0, fileStream.beg);

	//boost::filesystem::path pathBoost(path);
	//cout << "File name: " << pathBoost.string() << endl;

	//cout << "File size: " << sp << endl;
	//cout << "File content: " << fileStream.rdbuf() << endl;

	//fileStream.close();

	std::string path = "D://test/test_copy.txt";
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::resolver resolver(ioService);
	auto endpointIterator = resolver.resolve({ "127.0.0.1", "1000" });
	Client client(ioService, endpointIterator, path);
	ioService.run();

	


	system("pause");

	return 0;
}