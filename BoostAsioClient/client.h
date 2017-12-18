#pragma once

#include <array>
#include <fstream>

#include <boost/asio.hpp>


class Client
{
public:
    using IoService = boost::asio::io_service;
    using TcpResolver = boost::asio::ip::tcp::resolver;
    using TcpResolverIterator = TcpResolver::iterator;
    using TcpSocket = boost::asio::ip::tcp::socket;

    Client(IoService& t_ioService, TcpResolverIterator t_endpointIterator, 
        std::string const& t_path);

private:
    void openFile(std::string const& t_path);
    void doConnect();
    void doWriteFile(const boost::system::error_code& t_ec);
    template<class Buffer>
    void writeBuffer(Buffer& t_buffer);
	void repeatRecv(boost::system::error_code ec, size_t bytes);

	//接收所需函数
	void doRead();
	void processRead(size_t t_bytesTransferred);
	void createFile();
	void readData(std::istream &stream);
	void doReadFileContent(size_t t_bytesTransferred);
	void handleError(std::string const& t_functionName, boost::system::error_code const& t_ec);



    TcpResolver m_ioService;
    TcpSocket m_socket;
    TcpResolverIterator m_endpointIterator;
    enum { MessageSize = 1024 };
    std::array<char, MessageSize> m_buf;
    boost::asio::streambuf m_request;
    std::ifstream m_sourceFile;
    std::string m_path;
	char bufRecv[128];

	//接收所需变量
	enum { MaxLength = 40960 };
	std::array<char, MaxLength> m_bufforRecv;
	boost::asio::streambuf m_requestBuf_;
	std::ofstream m_outputFile;
	size_t m_fileSize;
	std::string m_data;
	std::string m_fileName;

};


template<class Buffer>
void Client::writeBuffer(Buffer& t_buffer)
{
    boost::asio::async_write(m_socket,
        t_buffer,
        [this](boost::system::error_code ec, size_t /*length*/)
        {
            doWriteFile(ec);
        });
}
