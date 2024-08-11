#include <iostream>
#include <vector>
#include <cstring>
#include <sys/poll.h>
#include <unistd.h>

std::string read_until_content_length(int sock_fd, int content_length) {
	std::vector<char> buffer(1024);
	std::string body;

	struct pollfd pfd;
	pfd.fd = sock_fd;
	pfd.events = POLLIN;

	int total_bytes_read = 0;

	while (total_bytes_read < content_length) {
		int poll_count = poll(&pfd, 1, -1);
		if (poll_count > 0) {
			if (pfd.revents & POLLIN) {
				ssize_t bytes_read = read(sock_fd, &buffer[0], buffer.size());
				if (bytes_read > 0) {
					ssize_t bytes_to_append = std::min(static_cast<int>(bytes_read), content_length - total_bytes_read);
					body.append(&buffer[0], bytes_to_append);
					total_bytes_read += bytes_to_append;
				} else if (bytes_read == 0) {
					std::cerr << "Connection closed unexpectedly." << std::endl;
					break;
				} else {
					std::cerr << "Error reading from socket." << std::endl;
					break;
				}
			}
		} else if (poll_count < 0) {
			std::cerr << "Polling error." << std::endl;
			break;
		}
	}

	return body;
}

void test_read_until_content_length() {
	int pipe_fds[2];
	if (pipe(pipe_fds) == -1) {
		std::cerr << "Pipe creation failed" << std::endl;
		return;
	}
	const std::string test_data = "Hello, this is the body of the HTTP response.";
	int content_length = test_data.size();

	std::vector<std::string> chunks;
	chunks.push_back(test_data.substr(0, 10));
	chunks.push_back(test_data.substr(10, 15));
	chunks.push_back(test_data.substr(25, 5));
	chunks.push_back(test_data.substr(30));

	for (size_t i = 0; i < chunks.size(); ++i) {
		write(pipe_fds[1], chunks[i].c_str(), chunks[i].size());
		usleep(100000);
	}

	close(pipe_fds[1]);

	std::string result = read_until_content_length(pipe_fds[0], content_length);

	close(pipe_fds[0]);

	std::cout << "Read data: " << result << std::endl;
	if (result == test_data) {
		std::cout << "Test passed: data matches expected content." << std::endl;
	} else {
		std::cout << "Test failed: data does not match expected content." << std::endl;
	}
}

int main() {
	test_read_until_content_length();
	return 0;
}
