#include <iostream>
#include <future>
#include <thread>
#include <boost/asio.hpp>

bool is_connected = false;

class Writer{
public:
    void write(std::string ip, unsigned port){
        boost::asio::ip::tcp::endpoint e(
                boost::asio::ip::address::from_string(ip), port);
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::socket s(io_service, e.protocol());

        s.connect(e);
        std::string message = "Connected!";
        for (
            boost::asio::write(s, boost::asio::buffer(message + "\n"));
            std::getline(std::cin, message);
            boost::asio::write(s, boost::asio::buffer(message + "\n"))
        );
        
        boost::asio::write(s, boost::asio::buffer("EXIT\n"));
    }
};

class Reader{
public:
    void read(std::string ip, unsigned port){
        boost::asio::ip::tcp::endpoint e(boost::asio::ip::address::from_string(ip), port);

        boost::asio::io_service serv;

        boost::asio::ip::tcp::acceptor a(serv, e.protocol());
        a.bind(e);
        a.listen();

        boost::asio::ip::tcp::socket s(serv);
        a.accept(s);

        std::string message;

        while (read_one_message(s, message)) {
            if (message == "Connected!"){
                is_connected = true;
            }
            std::cout << message << std::endl;
        }
    }
private:
    bool read_one_message(boost::asio::ip::tcp::socket& s, std::string& message) {
        boost::asio::streambuf buffer;

        boost::asio::read_until(s, buffer, '\n');

        std::istream stream(&buffer);

        std::getline(stream, message);
        return message != "EXIT";
    }
};


int main() {
    auto port_1 = 65001u;
    std::string ip = "127.0.0.1";

    Reader reader;
    std::future<void> reader_future = std::async(std::launch::async, &Reader::read,
                                                  &reader, ip, port_1);

    while (!is_connected){
        sleep(1);
    }
    
    auto port_2 = 65002u;
    Writer writer;
    std::future<void> writer_future = std::async(std::launch::async, &Writer::write,
                                                  &writer, ip, port_2);
    
    writer_future.get();
}
