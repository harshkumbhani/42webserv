#include <iostream>
#include <vector>
#include <cstring>
#include <sys/poll.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <sys/stat.h>

// std::string read_until_content_length(int sock_fd, int content_length) {
// 	std::vector<char> buffer(1024);
// 	std::string body;

// 	struct pollfd pfd;
// 	pfd.fd = sock_fd;
// 	pfd.events = POLLIN;

// 	int total_bytes_read = 0;

// 	while (total_bytes_read < content_length) {
// 		int poll_count = poll(&pfd, 1, -1);
// 		if (poll_count > 0) {
// 			if (pfd.revents & POLLIN) {
// 				ssize_t bytes_read = read(sock_fd, &buffer[0], buffer.size());
// 				if (bytes_read > 0) {
// 					ssize_t bytes_to_append = std::min(static_cast<int>(bytes_read), content_length - total_bytes_read);
// 					body.append(&buffer[0], bytes_to_append);
// 					total_bytes_read += bytes_to_append;
// 				} else if (bytes_read == 0) {
// 					std::cerr << "Connection closed unexpectedly." << std::endl;
// 					break;
// 				} else {
// 					std::cerr << "Error reading from socket." << std::endl;
// 					break;
// 				}
// 			}
// 		} else if (poll_count < 0) {
// 			std::cerr << "Polling error." << std::endl;
// 			break;
// 		}
// 	}

// 	return body;
// }

// void test_read_until_content_length() {
// 	int pipe_fds[2];
// 	if (pipe(pipe_fds) == -1) {
// 		std::cerr << "Pipe creation failed" << std::endl;
// 		return;
// 	}
// 	const std::string test_data = "Hello, this is the body of the HTTP response.";
// 	int content_length = test_data.size();

// 	std::vector<std::string> chunks;
// 	chunks.push_back(test_data.substr(0, 10));
// 	chunks.push_back(test_data.substr(10, 15));
// 	chunks.push_back(test_data.substr(25, 5));
// 	chunks.push_back(test_data.substr(30));

// 	for (size_t i = 0; i < chunks.size(); ++i) {
// 		write(pipe_fds[1], chunks[i].c_str(), chunks[i].size());
// 		usleep(100000);
// 	}

// 	close(pipe_fds[1]);

// 	std::string result = read_until_content_length(pipe_fds[0], content_length);

// 	close(pipe_fds[0]);

// 	std::cout << "Read data: " << result << std::endl;
// 	if (result == test_data) {
// 		std::cout << "Test passed: data matches expected content." << std::endl;
// 	} else {
// 		std::cout << "Test failed: data does not match expected content." << std::endl;
// 	}
// }

// int main() {
// 	test_read_until_content_length();
// 	return 0;
// }

// ================================================delete==============================================
// #include <cstdio>
// #include <iostream>
// #include <cstring>
// #include <string>
// #include <sstream>

// std::string generateHttpResponse(int statusCode, const std::string& message) {
// 	std::ostringstream response;
// 	response << "HTTP/1.1 " << statusCode << " ";
// 	if (statusCode == 200) {
// 		response << "OK";
// 	} else if (statusCode == 404) {
// 		response << "Not Found";
// 	} else if (statusCode == 400) {
// 		response << "Bad Request";
// 	} else if (statusCode == 500) {
// 		response << "Internal Server Error";
// 	}
// 	response << "\r\n";
// 	response << "Content-Type: text/plain\r\n";
// 	response << "Content-Length: " << message.length() << "\r\n";
// 	response << "\r\n";
// 	response << message;
// 	return response.str();
// }

// std::string processRequest(const std::string& method, const std::string& filePath) {
// 	if (method != "DELETE") {
// 		return generateHttpResponse(400, "Invalid request method. Only DELETE is supported.");
// 	}
// 	FILE* file = std::fopen(filePath.c_str(), "r");
// 	if (!file) {
// 		return generateHttpResponse(404, "File not found.");
// 	}
// 	std::fclose(file);
// 	if (std::remove(filePath.c_str()) == 0) {
// 		return generateHttpResponse(200, "File deleted successfully.");
// 	} else {
// 		return generateHttpResponse(500, std::strerror(errno));
// 	}
// }

// int main() {
// 	std::string method = "DELETE";
// 	std::string filePath = "example.txt";
// 	std::string response = processRequest(method, filePath);
// 	std::cout << response << std::endl;

// 	return 0;
// }

// // ======================================check=======================================

//===========================cgi===============================

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

int main() {
	// Get the query string from the environment
	const char* query = getenv("QUERY_STRING");

	// Command to execute the Python script
	std::string command = "python3 /path/to/your_script.py";
	
	if (query && strlen(query) > 0) {
		command += " ";
		command += query;  // Pass the query string to the Python script as an argument
	}

	// Open the command for reading
	FILE* pipe = popen(command.c_str(), "r");
	if (!pipe) {
		std::cerr << "Failed to run Python script!" << std::endl;
		return 1;
	}

	// Output the HTTP headers
	std::cout << "Content-type: text/html\n\n";

	// Read the Python script's output and print it to the web server's response
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
		std::cout << buffer;
	}

	pclose(pipe);
	return 0;
}
