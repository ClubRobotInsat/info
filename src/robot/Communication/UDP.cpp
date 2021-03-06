//
// Created by terae on 07/11/18.
//

#include "UDP.h"

#include <log/Log.h>

namespace Communication {

	UDP::UDP(const std::string& addr, uint16_t local_port, uint16_t remote_port) {
		const std::string address = (addr == "localhost" ? "127.0.0.1" : addr);

		using asio::ip::udp;
		asio::error_code err;

		// Sender socket creation
		{
			asio::io_service io_service;
			_send_socket = std::make_unique<udp::socket>(io_service);
			_send_socket->open(udp::v4(), err);

			if(err) {
				throw ErrorUDPCommunication("Failed to open the sending socket.");
			}

			_send_remote_endpoint = udp::endpoint(asio::ip::make_address(address), remote_port);
		}

		// Client socket creation
		{
			asio::io_service io_service;
			_recv_socket = std::make_unique<udp::socket>(io_service);
			_recv_socket->open(udp::v4(), err);

			if(err) {
				throw ErrorUDPCommunication("Failed to open the receiving socket.");
			}

			const std::string local_address = "0.0.0.0";
			_recv_socket->bind(udp::endpoint(asio::ip::make_address(local_address), local_port), err);

			if(err) {
				throw ErrorUDPCommunication("Failed to bind the receiving socket with " + local_address + ":" +
				                            std::to_string(local_port) + ".");
			}
		}

		_connected = true;
	}

	void UDP::write_bytes(const uint8_t* bytes, std::size_t bytes_number) {
		if(_connected) {
			asio::error_code err;
			ssize_t sent = _send_socket->send_to(asio::buffer(bytes, bytes_number), _send_remote_endpoint, 0, err);

			if(err || sent < static_cast<ssize_t>(bytes_number)) {
				logError("Error while sending data. Closure of the communication.");
				close_socket();
			}
		}
	}

	size_t UDP::read_bytes(uint8_t* bytes, std::size_t bytes_number) {
		if(_connected) {
			asio::error_code err;
			size_t recv = _recv_socket->receive_from(asio::buffer(bytes, bytes_number), _recv_remote_endpoint, 0, err);

			if(err) {
				logError("Error while receiving data. Closure of the communication.");
				close_socket();
			}
			return recv;
		}
		return 0;
	}

	UDP::~UDP() {
		close_socket();
	}

	bool UDP::is_connected() {
		return _connected;
	}

	void UDP::close_socket() {
		if(is_connected()) {
			_send_socket->close();
			_recv_socket->close();

			_connected = false;
		}
	}

} // namespace Communication
