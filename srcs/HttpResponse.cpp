/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkumbhan <hkumbhan@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/27 14:04:08 by hkumbhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "HttpResponse.hpp"

HttpResponse::HttpResponse() {}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::generateHtml(int code, const std::string& codeMessage) {
	std::ostringstream stream;

	std::string code_str;
	std::stringstream ss;
	ss << code;
	ss >> code_str;

	stream << "<!DOCTYPE html>"
		<< "<html lang=\"en\">"
		<< "<head>"
		<< "<meta charset=\"UTF-8\">"
		<< "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		<< "<title>Webserv - " << code_str << "</title>"
		<< "</head>"
		<< "<body class=\"background\">"
		<< "<div class=\"error\">" << code_str << " - " << codeMessage << "</div>"
		<< "<hr>"
		<< "<button onclick=\"window.history.back()\" class=\"back-button\">Back</button>"
		<< "</body>"
		<< "</html>";

	return stream.str();
}

std::string HttpResponse::statusCodes(int code) {
	std::string codeMessage;
	if (code == 400)
		codeMessage = "Bad Request";
	else if (code == 404)
		codeMessage = "Not Found";
	else if (code == 500)
		codeMessage = "Internal Server Error";
	else
		codeMessage = "Unknown Code";
	return generateHtml(code, codeMessage);
}

std::string HttpResponse::metaData(clientState &clientData) {
	std::string headerMetaData = "";
	std::map<std::string, std::string>::iterator hd = clientData.header.begin();
	while (hd != clientData.header.end()) {
		if (hd->first == "Content-Length" || hd->first == "Content-Type" || \
			hd->first == "Connection" || hd->first == "Date" \
			|| hd->first == "Server" || hd->first == "Range"){
			clientData.header.erase(hd++);
		} else {
			headerMetaData += hd->first + ": " + hd->second + "\r\n";
			++hd;
		}
	}
	headerMetaData += "\r\n";
	return headerMetaData;
}

std::string HttpResponse::webserverStamp(void) {
	time_t now = time(0);
	char buf[100];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
	return std::string(buf);
}

std::string HttpResponse::buildHttpResponse(const std::string& statusLine, const std::string& contentType, const std::string& body, const clientState& clientData) {
	std::string header;
	header = "Content-Type: " + contentType + "\r\n";
	header += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	header += "Connection: keep-alive\r\n";
	header += "Date: " + webserverStamp() + "\r\n";
	header += "Server: Webserv/harsh/oreste/v1.0\r\n";
	header += metaData(const_cast<clientState &>(clientData));

	return statusLine + header + body;
}

std::string HttpResponse::errorHandlingGet(int code, clientState &clientData) {
	std::string errorCode;
	std::stringstream ss;
	ss << code;
	ss >> errorCode;

	std::string sms;
	if (code == 400) sms = "Bad Request";
	else if (code == 404) sms = "Not Found";
	else if (code == 500) sms = "Internal Server Error";
	else sms = "Unknown Error";
	_status_line = clientData.requestLine[2] + " " + errorCode + " " + sms + "\r\n";
	std::string status_page = statusCodes(code);
	std::stringstream pageSizeStream;
	pageSizeStream << status_page.size();
	std::string pageSize;
	pageSizeStream >> pageSize;

	_header = "Content-Type: text/html\r\nContent-Length: " + pageSize + "\r\nConnection: close\r\n";
	_body = status_page;
	std::string headerMetaData = metaData(clientData);
	_header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	_response = _status_line + _header + _body;
	return _response;
}

std::string HttpResponse::deleteListing(clientState &clientData) {
	std::string directoryPath = clientData.serverData.root + clientData.requestLine[1];
	
	if (std::filesystem::is_directory(directoryPath) == false) {
		return genericHttpCodeResponse(404, "Not Found");;
	}
	
	std::ostringstream html;
	html << "<table style=\"width: 100%; text-align: center;\">\n"
			<< "    <thead>\n"
			<< "        <tr>\n"
			<< "            <th colspan=\"3\" style=\"font-weight: bold; color: #ADD8E6; text-align: center;\">Directory: " + directoryPath + "</th>\n"
			<< "        </tr>\n"
			<< "        <tr>\n"
			<< "            <th>Icon</th>\n"
			<< "            <th>Name</th>\n"
			<< "            <th>Action</th>\n"
			<< "        </tr>\n"
			<< "    </thead>\n";

	
	for(const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
		const auto &path = entry.path();
		std::string filename = path.filename().string();
		std::string icon = entry.is_directory() ? "📁" : "📄";
		std::string deleteLink = "/delete?file=" + filename;

		html << "<tr>\n"
					<< "    <td>" << icon << "</td>\n"
					<< "    <td><a href=\"" << clientData.requestLine[1] + "/" + filename << "\">" << filename << "</a></td>\n"
					<< "    <td><button class=\"delete-style\" onclick=\""
					<< "fetch('" << deleteLink << "', {method: 'DELETE'})"
					<< ".then(function(response) { "
					<< "if (response.ok) { "
					<< "loadDirectoryListing('/upload');"  // Reload the directory listing without closing it
					<< "} else { "
					<< "alert('Delete failed with status: ' + response.status);"
					<< "}"
					<< "})"
					<< ".catch(function(error) {"
					<< "alert('Network error or no response from server');"
					<< "})\">"
					<< "Delete</button></td>\n"
					<< "</tr>\n";
		}
	_status_line = clientData.requestLine[2] + " 200 OK\r\n";
	_header = "Content-Type: text/html\r\nContent-Length: " + std::to_string(html.str().size()) + "\r\nConnection: keep-alive\r\n";
	std::string headerMetaData = metaData(clientData);
	_header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	_response = _status_line + _header + html.str();
	return _response;
}

std::string HttpResponse::directoryListing(clientState &clientData) {
	std::string directoryPath = clientData.serverData.root + clientData.requestLine[1];
	
	if (!std::filesystem::is_directory(directoryPath)) {
		return genericHttpCodeResponse(404, "Not Found");
	}
	
	std::ostringstream html;
	html << "<table style=\"width: 100%; text-align: center;\">\n"
		 << "    <thead>\n"
		 << "        <tr>\n"
		 << "            <th colspan=\"2\" style=\"font-weight: bold; color: black; text-align: center;\">Directory: " + directoryPath + "</th>\n"
		 << "        </tr>\n"
		 << "        <tr>\n"
		 << "            <th>Icon</th>\n"
		 << "            <th>Name</th>\n"
		 << "        </tr>\n"
		 << "    </thead>\n";

	for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
		const auto &path = entry.path();
		std::string filename = path.filename().string();
		std::string icon = entry.is_directory() ? "📁" : "📄";

		html << "<tr>\n"
			 << "    <td>" << icon << "</td>\n"
			 << "    <td><a href=\"" << clientData.requestLine[1] + "/" + filename << "\">" << filename << "</a></td>\n"
			 << "</tr>\n";
	}

	_status_line = clientData.requestLine[2] + " 200 OK\r\n";
	_header = "Content-Type: text/html\r\nContent-Length: " + std::to_string(html.str().size()) + "\r\nConnection: keep-alive\r\n";
	std::string headerMetaData = metaData(clientData);
	_header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	_response = _status_line + _header + html.str();
	return _response;
}

std::string HttpResponse::handleGetFile(clientState &clientData) {
	static int i = 0;
	static std::vector<std::string> getImageFiles;

	if (getImageFiles.empty() == true) {
		for(const auto &entry : std::filesystem::directory_iterator("www/getimage")){
			const auto &path = entry.path();
			std::string filename = clientData.serverData.root + "/getimage/" + path.filename().string();
			getImageFiles.push_back(filename);
		}
	}
	
	if (i >= static_cast<int>(getImageFiles.size())) {
		i = 0;
	}
	
	std::string route = getImageFiles[i++];
	size_t pos = route.find_last_of('.');
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];
	std::ifstream route_file(route.c_str());
	if (route_file.fail())
		return errorHandlingGet(404, clientData);
	else {
		struct stat statFile;
		if(stat(route.c_str(), &statFile) != 0) {
			WARNING("Unable to get file properties");
			exit(42);
		}
		std::string buffer((std::istreambuf_iterator<char>(route_file)), std::istreambuf_iterator<char>());
		_status_line = clientData.requestLine[2] + " 200 OK\r\n";

		_header = "Content-Type: " + contentType + "\r\n"
							"Content-Length: " + std::to_string(statFile.st_size) + "\r\n"
							"Connection: keep-alive\r\n";
		_body = buffer;
		route_file.close();
	}
	std::string headerMetaData = metaData(clientData);
	_header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	_response = _status_line + _header + _body;
	return _response;
}

std::string HttpResponse::respond_Get(clientState &clientData) {

	std::string route = clientData.serverData.root + (clientData.requestLine[1] == "/" ? "/index.html" : clientData.requestLine[1]);
	if (clientData.requestLine[1].substr(0, 7) == "/upload" && std::filesystem::is_directory(route)) {
		if (clientData.requestLine[1] == "/upload")
			return deleteListing(clientData);
		else
			return directoryListing(clientData);
	}
	if (clientData.requestLine[1] == "/get-files") {
		return handleGetFile(clientData);
	}
	clientData.header["X-File-Type"] = "file";
	size_t pos = route.find_last_of('.');
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];
	std::ifstream route_file(route.c_str());
	if (route_file.fail())
		return errorHandlingGet(404, clientData);
	else {
		struct stat statFile;
		if(stat(route.c_str(), &statFile) != 0) {
			WARNING("Unable to get file properties");
			exit(42);
		}
		std::string buffer((std::istreambuf_iterator<char>(route_file)), std::istreambuf_iterator<char>());
		_status_line = clientData.requestLine[2] + " 200 OK\r\n";

		std::stringstream ss;
		ss << statFile.st_size;
		std::string fileSize;
		ss >> fileSize;

		_header = "Content-Type: " + contentType + "\r\nContent-Length: " + fileSize + "\r\nConnection: keep-alive\r\n";
		_body = buffer;
		route_file.close();
	}
	std::string headerMetaData = metaData(clientData);
	_header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
	_response = _status_line + _header + _body;
	return _response;
}

bool HttpResponse::is_valid_char(char c) {
	return isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '/';
}

bool HttpResponse::is_valid_str(const std::string &str) {
	for (size_t i = 0; i < str.length(); ++i) {
		if (!is_valid_char(str[i]))
			return false;
	}
	return true;
}

std::string generateHttpResponse(const std::string &httpVersion, int statusCode, const std::string &statusMessage, const std::string &body, const std::string &contentType = "text/html") {
	std::string statusLine = httpVersion + " " + std::to_string(statusCode) + " " + statusMessage + "\r\n";
	std::stringstream ss;
	ss << body.size();
	std::string contentLength;
	ss >> contentLength;
	std::string headers = "Content-Type: " + contentType + "\r\nContent-Length: " + contentLength + "\r\nConnection: close\r\n";
	return statusLine + headers + "\r\n" + body;
}

std::string HttpResponse::successHandling(int statusCode, clientState &clientData, const std::string &messageBody) {
	std::string statusMessage;
	switch (statusCode) {
		case 200:
			statusMessage = "OK";
			break;
		case 201:
			statusMessage = "Created";
			break;
		case 202:
			statusMessage = "Accepted";
			break;
		default:
			statusCode = 200;
			statusMessage = "OK";
			break;
	}
	std::string responseBody = messageBody.empty() ? "<html><body><h1>" + statusMessage + "</h1></body></html>" : messageBody;
	return generateHttpResponse(clientData.requestLine[2], statusCode, statusMessage, responseBody);
}

std::string HttpResponse::errorHandlingPost(int statusCode, clientState &clientData) {
	return generateHttpResponse(clientData.requestLine[2], statusCode,
			httpErrorMap.at(statusCode), generateErrorPage(statusCode, httpErrorMap.at(statusCode)));
}

bool HttpResponse::write_to_file(clientState &clientData, const std::string& path, const std::string& content) {
	std::ofstream outFile(path.c_str(), std::ios::binary);
	if (!outFile){
		WARNING("Error: Unable to open file for writing: " + path);
		clientData.flagFileStatus = true;
	}
	outFile.write(content.c_str(), content.size());
	outFile.close();
	if (!outFile){
		WARNING("Error: Failed to write file: " + path);
		clientData.flagFileStatus = true;
	}
	return (clientData.flagFileStatus);
}

void HttpResponse::parse_headers(std::istringstream& contentStream, std::string& fileName, std::string& fileContent) {
	std::string line;
	bool headerParsed = false;

	while (std::getline(contentStream, line)) {
		if (line == "\r" || line.empty()) {
			headerParsed = true;
			break;
		}
		if (line.find("filename=\"") != std::string::npos) {
			std::size_t startPos = line.find("filename=\"") + 10;
			std::size_t endPos = line.find("\"", startPos);
			if (endPos != std::string::npos) {
				fileName = line.substr(startPos, endPos - startPos);
			}
		}
	}
	if (!headerParsed) {
		std::cerr << "Error: Incomplete headers, waiting for more data." << std::endl;
		return;
	}
	fileContent.assign((std::istreambuf_iterator<char>(contentStream)), std::istreambuf_iterator<char>());
}

std::string HttpResponse::findBoundary(const std::map<std::string, std::string>& headers) {
	std::map<std::string, std::string>::const_iterator contentTypeIt = headers.find("Content-Type");
	if (contentTypeIt == headers.end())
		return "";
	const std::string& contentType = contentTypeIt->second;
	std::string::size_type boundaryPos = contentType.find("boundary=");
	if (boundaryPos == std::string::npos)
		return "";
	boundaryPos += 9;
	std::string::size_type boundaryEnd = contentType.find(";", boundaryPos);
	std::string boundary = contentType.substr(boundaryPos, boundaryEnd - boundaryPos);
	if (!boundary.empty() && boundary.front() == '"' && boundary.back() == '"')
		boundary = boundary.substr(1, boundary.size() - 2);
	return boundary;
}

void HttpResponse::parseRequestBody(clientState &clientData) {
	std::string boundary = "--" + clientData.boundary;
	std::size_t boundaryStart = clientData.bodyString.find(boundary);
	if (boundaryStart == std::string::npos)
		return;
	std::size_t boundaryEnd = clientData.bodyString.find("\r\n", boundaryStart);
	if (boundaryEnd == std::string::npos)
		return;
	boundaryEnd += 2;
	std::size_t nextBoundaryStart = clientData.bodyString.find(boundary, boundaryEnd);
	if (nextBoundaryStart == std::string::npos)
		return;
	std::string bodyPart = clientData.bodyString.substr(boundaryEnd, nextBoundaryStart - boundaryEnd);
	std::istringstream contentStream(bodyPart);
	std::string fileName;
	std::string fileContent;

	parse_headers(contentStream, fileName, fileContent);
	std::string filePath = "./www/upload/" + fileName;
	write_to_file(clientData, filePath, fileContent);
	clientData.fileName = fileName;
	clientData.bodyString.erase(0, nextBoundaryStart);
	clientData.flagBodyRead = true;
	return;
}

std::string HttpResponse::response_Post(clientState &clientData) {
	std::string route = "./www" + clientData.requestLine[1];
	if (!is_valid_str(route))
		return errorHandlingPost(400, clientData);

	size_t pos = route.find_last_of('.');
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];

	clientData.boundary = findBoundary(clientData.header);
	parseRequestBody(clientData);
	clientData.bodyString.clear();
	if (clientData.flagFileStatus == true)
		return (genericHttpCodeResponse(400, httpErrorMap.at(400)));
	
	return genericHttpCodeResponse(201, httpErrorMap.at(201));
}


//================================DELETE=====================================
std::string HttpResponse::generateErrorPage(int code, const std::string& message) {
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Http Code )" + std::to_string(code) + R"(</title>
	<style>
		body {
			margin: 0;
			padding: 0;
			display: flex;
			justify-content: center;
			align-items: center;
			height: 100vh;
			background: linear-gradient(135deg, #1a1a1a, #333);
			color: white;
			font-family: Arial, sans-serif;
		}
		.container {
			text-align: center;
		}
		h1 {
			font-size: 6em;
			margin: 0;
		}
		p {
			font-size: 1.5em;
			margin: 0;
		}
		.back-button {
			margin-top: 20px;
			padding: 10px 20px;
			font-size: 1em;
			color: #333;
			background-color: white;
			border: none;
			cursor: pointer;
			border-radius: 5px;
			text-decoration: none;
		}
		.back-button:hover {
			background-color: #ddd;
		}
	</style>
	<script>
		function goBack() {
			window.history.back()
		}
	</script>
</head>
<body>
	<div class="container">
		<h1>)" + std::to_string(code) + R"(</h1>
		<p>)" + message + R"(</p>
		<button class="back-button" onclick="goBack() ">Go Back</button>
	</div>
</body>
</html>
			)";
}

std::string HttpResponse::genericHttpCodeResponse(int statusCode, const std::string& message) {
	std::string msg = generateErrorPage(statusCode, message);
	std::ostringstream response;
	response << "HTTP/1.1 " << statusCode << " " << message;
	response << "\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Content-Length: " << msg.length() << "\r\n";
	response << "\r\n";

	response << msg;
	return response.str();
}

std::string urlDecode(const std::string& value) {
    std::string decoded;
		decoded.reserve(value.size());

		for(size_t i = 0; i < value.size(); i++) {
			if (value[i] == '%') {
				if (i + 2 < value.size() && value[i + 1] == '2' && value[i + 2] == '0') {
					decoded += ' ';
					i += 2;
				} else {
					decoded += value[i];
				}
			} else {
				decoded += value[i];
			}
		}
	return decoded;
}

std::string HttpResponse::responseDelete(clientState &clientData) {
	std::string filename = urlDecode(clientData.requestLine[1].substr(clientData.requestLine[1].find("=") + 1));
	const std::string& filePath = clientData.serverData.root + "/upload/" + filename;
	std::cout << "\n\n\n\n\nfile name: " << filename << "\n\n\n";

	FILE* file = std::fopen(filePath.c_str(), "r");
	if (!file)
		return genericHttpCodeResponse(404, "File not found.");
	std::fclose(file);

	if (std::remove(filePath.c_str()) == 0)
		return genericHttpCodeResponse(200, "File deleted successfully.");
	else
		return genericHttpCodeResponse(500, std::strerror(errno));
}

std::string HttpResponse::respondRedirect(clientState &clientData) {
	for (auto &location : clientData.serverData.location) {
		if (!location.redirect.empty()) {
			if (location.redirect.substr(0, 7) != "http://" && location.redirect.substr(0, 8) != "https://") {
				clientData.header["Location"] = "https://" + location.redirect;
			} else {
				clientData.header["Location"] = location.redirect;
			}
			_status_line = clientData.requestLine[2] + " 302 Found\r\n";
			std::string headerMetaData = metaData(clientData);
			_header += "Date: " + webserverStamp() + "\r\nServer: Webserv/harsh/oreste/v1.0\r\n" + headerMetaData;
			_response = _status_line + _header;

			return _response;
		}
	}
	return genericHttpCodeResponse(404, "Page Not Found");
}

/*--------   CGI   -----------*/

std::string HttpResponse::processCgi(clientState &clientData) {
	if (clientData.isForked == true)
		return parentProcess(clientData);
	INFO("CGI start");
	if (pipe(clientData.fd) == -1) {
		ERROR("Pipe failed");
		return genericHttpCodeResponse(500, httpErrorMap.at(500));
	}
	clientData.pid = fork();
	if (clientData.pid == -1) {
		ERROR("Fork Failed");
		close(clientData.fd[0]);
		close(clientData.fd[1]);
		return genericHttpCodeResponse(500, httpErrorMap.at(500));
	}
	
	clientData.isForked = true;
	
	if (clientData.pid == 0) {
		close(clientData.fd[0]);
		dup2(clientData.fd[1], STDOUT_FILENO);
		close(clientData.fd[1]);
		execute(clientData);
		exit(1);
	} else {
		close(clientData.fd[1]);
		return parentProcess(clientData);
	}
}

std::string HttpResponse::parentProcess(clientState &clientData) {
	std::string result;
	std::vector<char> buffer(4096);
	
	int status;
	ssize_t count;

	pid_t resultPid = waitpid(clientData.pid, &status, WNOHANG);
	if (resultPid == 0) {
		time_t currentTime = 0;

		std::time(&currentTime);
		if (std::difftime(currentTime, clientData.lastEventTime) > clientData.serverData.send_timeout) {
			ERROR("CGI script timed out");
			kill(clientData.pid, SIGKILL);
			close(clientData.fd[0]);
			clientData.isForked = false;
			return genericHttpCodeResponse(504, httpErrorMap.at(504));
		}
		return result; // result is empty at this stage
	} else if (resultPid == -1) {
		ERROR("waitpid failed");
		close(clientData.fd[0]);
		clientData.isForked = false;
		return genericHttpCodeResponse(502, httpErrorMap.at(502));
	}
	
	if (WIFEXITED(status)) {
		int exitStatus = WEXITSTATUS(status);
		if (exitStatus == 0) {
			while ((count = read(clientData.fd[0], buffer.data(), buffer.size())) > 0) {
				result.append(buffer.data(), count);
			}
			
			close(clientData.fd[0]);
			clientData.isForked = false;
			
			if (result.empty()) {
				return genericHttpCodeResponse(502, httpErrorMap.at(502));
			}
			_status_line = clientData.requestLine[2] + " 200 OK\r\n";
			return buildHttpResponse(_status_line, "text/html", result, clientData);
		} else {
			ERROR("CGI Script exited with error status: " + std::to_string(exitStatus));
			close(clientData.fd[0]);
			clientData.isForked = false;
			return genericHttpCodeResponse(500, httpErrorMap.at(500));
		}
	} else {
		ERROR("CGI script did not exit normally");
		close(clientData.fd[0]);
		clientData.isForked = false;
		return genericHttpCodeResponse(500, httpErrorMap.at(500));
	}
}

void	HttpResponse::execute(clientState &clientData) {
	std::string scriptname;
	std::string query;
	
	if (clientData.method == POST) {
		scriptname = clientData.serverData.root + clientData.requestLine[1];
		query = clientData.bodyString;
	} else if (clientData.method == GET) {
		size_t pos = clientData.requestLine[1].find('?');
		if (pos != std::string::npos) {
			scriptname = clientData.serverData.root + clientData.requestLine[1].substr(0, pos);
			query = clientData.requestLine[1].substr(pos + 1);
		}
	}

	std::vector<std::string> env_strings = {
	"QUERY_STRING=" + query,
    "REQUEST_METHOD=" + clientData.requestLine[0],
    "CONTENT_LENGTH=" + std::to_string(clientData.contentLength),
    "GATEWAY_INTERFACE=CGI/1.1",
    "SCRIPT_NAME=" + scriptname,
    "SERVER_NAME=" + clientData.serverData.server_name,
    "SERVER_PORT=" + std::to_string(clientData.serverData.listen),
    "SERVER_PROTOCOL=HTTP/1.1"
	};

	std::vector<char*> env;
	for (auto& str : env_strings) {
		env.push_back(const_cast<char*>(str.c_str()));
	}
	env.push_back(nullptr);

	char *args[] = {
		const_cast<char *>("/usr/bin/python3"),
		const_cast<char *>(scriptname.c_str()),
		NULL
  };
	
	execve(args[0], args, env.data());
	ERROR("execve failed");
}


std::string HttpResponse::respond(clientState &clientData) {
	if (clientData.requestLine[1] == "/redirect") {
		return respondRedirect(clientData);
	} else if (clientData.requestLine[1].substr(0, 4) == "/cgi") {
		return processCgi(clientData);
	} else if (clientData.requestLine[0] == "GET") {
		return respond_Get(clientData);
	} else if (clientData.requestLine[0] == "POST") {
		if (clientData.flagFileSizeTooBig)
			return (genericHttpCodeResponse(413, httpErrorMap.at(413)));
		return response_Post(clientData);
	} else if (clientData.requestLine[0] == "DELETE") {
		return responseDelete(clientData);
	} else {
		return genericHttpCodeResponse(405, httpErrorMap.at(405));
	}
}
