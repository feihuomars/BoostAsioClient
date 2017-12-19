#include <string>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include "client.h"


Client::Client(IoService& t_ioService, TcpResolverIterator t_endpointIterator, 
    std::string const& t_path)
    : m_ioService(t_ioService), m_socket(t_ioService), 
    m_endpointIterator(t_endpointIterator), m_path(t_path)
{
    
    openFile(m_path);
	doConnect();
}


void Client::openFile(std::string const& t_path)
{
    m_sourceFile.open(t_path, std::ios_base::binary | std::ios_base::ate);
	if (m_sourceFile.fail()) {
		//throw std::fstream::failure("Failed while opening file " + t_path);
		std::cout << "Failed while opening file " + t_path << std::endl;
	}
        
    
    m_sourceFile.seekg(0, m_sourceFile.end);
    auto fileSize = m_sourceFile.tellg();
    m_sourceFile.seekg(0, m_sourceFile.beg);

    std::ostream requestStream(&m_request);
    boost::filesystem::path p(t_path);
	size_t number = 20172222;
    //requestStream << p.filename().string() << "\n" << fileSize << "\n\n";
	requestStream << p.filename().string() << "\n" << fileSize << "\n" << "data" << "\n\n";
    //BOOST_LOG_TRIVIAL(trace) << "Request size: " << m_request.size();
}


void Client::doConnect()
{
    boost::asio::async_connect(m_socket, m_endpointIterator, 
        [this](boost::system::error_code ec, TcpResolverIterator)
        {
            if (!ec) {
				std::string sendToServer;
				//sendToServer = "D://test/test.txt\n10\ndata\n\n";
				sendToServer = "hello";
                writeBuffer(m_request);
            } else {
                std::cout << "Coudn't connect to host. Please run server "
                    "or check network connection.\n";
                //BOOST_LOG_TRIVIAL(error) << "Error: " << ec.message();
            }
        });
}


void Client::doWriteFile(const boost::system::error_code& t_ec)
{
    if (!t_ec) {
        if (m_sourceFile) {
            m_sourceFile.read(m_buf.data(), m_buf.size()); 
            if (m_sourceFile.fail() && !m_sourceFile.eof()) {
                auto msg = "Failed while reading file";
                //BOOST_LOG_TRIVIAL(error) << msg;
                throw std::fstream::failure(msg);
            }
            std::stringstream ss;
            ss << "Send " << m_sourceFile.gcount() << " bytes, total: "
                << m_sourceFile.tellg() << " bytes" << "   content: " << m_buf.data();
           // BOOST_LOG_TRIVIAL(trace) << ss.str();
            std::cout << ss.str() << std::endl;

            auto buf = boost::asio::buffer(m_buf.data(), static_cast<size_t>(m_sourceFile.gcount()));
            writeBuffer(buf);
		}
		else {
			//发送完成位置
			std::cout << "send file completed." << std::endl;
			/*m_socket.async_read_some(boost::asio::buffer(bufRecv), [this](boost::system::error_code ec, size_t bytes) {
				if (ec) {
					std::cout << "extra recv failed" << std::endl;
				}
				else {
					
					std::cout << "extra data: " << bufRecv << std::endl;
					repeatRecv(ec, bytes);
				}

			});*/
			doRead();
		}
    } else {
        //BOOST_LOG_TRIVIAL(error) << "Error: " << t_ec.message();
    }
}

void Client::repeatRecv(boost::system::error_code ec, size_t bytes) {
	if (ec) {
		std::cout << "extra recv failed" << std::endl;
	}
	else {
		m_socket.async_read_some(boost::asio::buffer(bufRecv),
			boost::bind(&Client::repeatRecv, this, ec, bytes));
		std::cout << "file data: " << bufRecv << std::endl;
	}
}

void Client::doRead()
{
	auto currentPath = boost::filesystem::path("D://test/clientRecv");
	current_path(currentPath);
	async_read_until(m_socket, m_requestBuf_, "\n\n",
		[this](boost::system::error_code ec, size_t bytes)
	{
		if (!ec)
			processRead(bytes);
		else
			handleError(__FUNCTION__, ec);
	});
}

void Client::processRead(size_t t_bytesTransferred)
{
	/*BOOST_LOG_TRIVIAL(trace) << __FUNCTION__ << "(" << t_bytesTransferred << ")"
	<< ", in_avail = " << m_requestBuf_.in_avail() << ", size = "
	<< m_requestBuf_.size() << ", max_size = " << m_requestBuf_.max_size() << ".";*/

	std::istream requestStream(&m_requestBuf_);
	readData(requestStream);

	auto pos = m_fileName.find_last_of('\\');
	if (pos != std::string::npos)
		m_fileName = m_fileName.substr(pos + 1);

	createFile();

	// write extra bytes to file
	do {
		requestStream.read(m_bufforRecv.data(), m_bufforRecv.size());
		//BOOST_LOG_TRIVIAL(trace) << __FUNCTION__ << " write " << requestStream.gcount() << " bytes.";
		m_outputFile.write(m_bufforRecv.data(), requestStream.gcount());
		std::cout << "extra data: " << m_bufforRecv.data() << " count: " << requestStream.gcount() << std::endl;
	} while (requestStream.gcount() > 0);

	//同步接收文件
	while (true) {

		if (m_outputFile.tellp() >= static_cast<std::streamsize>(m_fileSize)) {
			std::cout << "接收完成" << std::endl;
			break;
		}
		boost::system::error_code ec;
		size_t len = m_socket.read_some(boost::asio::buffer(m_bufforRecv.data(), m_bufforRecv.size()), ec);
		if (ec) {
			std::cout << boost::system::system_error(ec).what() << std::endl;
			break;
		}
		
		
		m_outputFile.write(m_bufforRecv.data(), static_cast<std::streamsize>(len));

	}


	//异步接收文件
	/*m_socket.async_read_some(boost::asio::buffer(m_bufforRecv.data(), m_bufforRecv.size()),
		[this](boost::system::error_code ec, size_t bytes)
	{
		if (!ec) {
			std::cout << "start recving " << bytes <<std::endl;
			doReadFileContent(bytes);
		}
			
		else
			handleError(__FUNCTION__, ec);
	});*/
}

void Client::readData(std::istream &stream)
{
	stream >> m_fileName;
	stream >> m_fileSize;
	stream >> m_data;
	stream.read(m_bufforRecv.data(), 2);

	/*BOOST_LOG_TRIVIAL(trace) << m_fileName << " size is " << m_fileSize
	<< ", tellg = " << stream.tellg();*/
}

void Client::createFile()
{
	m_outputFile.open(m_fileName, std::ios_base::binary);
	if (!m_outputFile) {
		//BOOST_LOG_TRIVIAL(error) << __LINE__ << ": Failed to create: " << m_fileName;
		return;
	}
}

void Client::doReadFileContent(size_t t_bytesTransferred)
{
	if (t_bytesTransferred > 0) {
		m_outputFile.write(m_bufforRecv.data(), static_cast<std::streamsize>(t_bytesTransferred));

		//BOOST_LOG_TRIVIAL(trace) << __FUNCTION__ << " recv " << m_outputFile.tellp() << " bytes";
		std::cout << "Recving...content: " << m_bufforRecv.data();
		if (m_outputFile.tellp() >= static_cast<std::streamsize>(m_fileSize)) {
			//接收完成位置
			std::cout << "Received file: " << m_fileName << " size: " << m_fileSize << m_data << std::endl;
			


			return;
		}
	}
	
	m_socket.async_read_some(boost::asio::buffer(m_bufforRecv.data(), m_bufforRecv.size()),
		[this](boost::system::error_code ec, size_t bytes)
	{
		doReadFileContent(bytes);
	});
}

void Client::handleError(std::string const& t_functionName, boost::system::error_code const& t_ec)
{
	/*BOOST_LOG_TRIVIAL(error) << __FUNCTION__ << " in " << t_functionName << " due to "
	<< t_ec << " " << t_ec.message() << std::endl;*/
}
