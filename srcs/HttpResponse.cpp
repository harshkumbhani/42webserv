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
		std::string deleteLink = "/delete?file=" + clientData.requestLine[1] + "/" + filename;

		html << "<tr>\n"
					<< "    <td>" << icon << "</td>\n"
					<< "    <td><a href=\"" << clientData.requestLine[1] + "/" + filename << "\">" << filename << "</a></td>\n"
					<< "    <td><button class=\"delete-style\" onclick=\""
					<< "fetch('" << deleteLink << "', {method: 'DELETE'})"
					<< ".then(function(response) { "
					<< "if (response.ok) { "
					<< "loadDirectoryListing('" << clientData.requestLine[1] << "');"  // Reload the directory listing without closing it
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
    
    std::ostringstream html;
    // Adding a dark background style and light-colored text for the directory listing
    html << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
		 << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css\">\n"
         << "<title>Directory Listing</title>\n"
         << "<style>\n"
         << "  body { background-color: #121212; color: #f5f5f5; font-family: Arial, sans-serif; }\n"
         << "  table { width: 100%; text-align: left; border-collapse: collapse; }\n"
         << "  th, td { padding: 10px; border-bottom: 1px solid #333; }\n"
         << "  a { color: #64b5f6; text-decoration: none; }\n"
         << "  a:hover { text-decoration: underline; }\n"
         << "  th { color: #f5f5f5; font-weight: bold; }\n"
         << "</style>\n"
         << "</head>\n"
         << "<body>\n"
         << "<h2>Directory: " + directoryPath + "</h2>\n"
         << "<table>\n"
         << "    <thead>\n"
         << "        <tr>\n"
         << "            <th>Icon</th>\n"
         << "            <th>Name</th>\n"
         << "        </tr>\n"
         << "    </thead>\n"
         << "    <tbody>\n";

    for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
        const auto &path = entry.path();
        std::string filename = path.filename().string();
        // Use FontAwesome icons instead of emojis
		std::string icon = entry.is_directory() ? "<i class=\"fa-solid fa-folder\" style=\"color: #B197FC;\"></i>" : "<i class=\"fa-solid fa-file\" style=\"color: #babec5;\"></i>";


        html << "<tr>\n"
             << "    <td>" << icon << "</td>\n"
             << "    <td><a href=\"" << clientData.requestLine[1] + "/" + filename << "\">" << filename << "</a></td>\n"
             << "</tr>\n";
    }

    html << "    </tbody>\n"
         << "</table>\n"
         << "</body>\n"
         << "</html>\n";

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
		return genericHttpCodeResponse(404, httpErrorMap.at(404));
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

std::string HttpResponse::responseGet(clientState &clientData) {

	std::string route = clientData.serverData.root + (clientData.requestLine[1] == "/" ? "/index.html" : clientData.requestLine[1]);
	if (clientData.requestLine[1].substr(0, 7) == "/upload" && std::filesystem::is_directory(route)) {
		if (clientData.serverData.directory_listing == "off")
			return genericHttpCodeResponse(403, httpErrorMap.at(403));
		return deleteListing(clientData);
	}
	if (clientData.requestLine[1] == "/get-files") {
		clientData.header["X-File-Type"] = "file";
		return handleGetFile(clientData);
	}
	size_t pos = route.find_last_of('.');
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];
	std::ifstream route_file(route.c_str());
	if (route_file.fail())
		return genericHttpCodeResponse(404, httpErrorMap.at(404));
	else {
		if (std::filesystem::is_directory(route)) {
			return directoryListing(clientData); 
		}
		clientData.header["X-File-Type"] = "file";

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

bool HttpResponse::writeToFile(clientState &clientData, const std::string& path, const std::string& content) {
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

void HttpResponse::parseHeaders(std::istringstream& contentStream, std::string& fileName, std::string& fileContent) {
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

bool HttpResponse::parseRequestBody(clientState &clientData) {
	std::string boundary = "--" + clientData.boundary;
	std::size_t boundaryStart = clientData.bodyString.find(boundary);
	if (boundaryStart == std::string::npos)
		return true;
	std::size_t boundaryEnd = clientData.bodyString.find("\r\n", boundaryStart);
	if (boundaryEnd == std::string::npos)
		return true;
	boundaryEnd += 2;
	std::size_t nextBoundaryStart = clientData.bodyString.find(boundary, boundaryEnd);
	if (nextBoundaryStart == std::string::npos)
		return true;
	std::string bodyPart = clientData.bodyString.substr(boundaryEnd, nextBoundaryStart - boundaryEnd);
	std::istringstream contentStream(bodyPart);
	std::string fileName;
	std::string fileContent;

	parseHeaders(contentStream, fileName, fileContent);
	std::string filePath = "./www/upload/" + fileName;
	clientData.fileName = filePath;
	if (fileName.empty()) {
		clientData.flagFileStatus = true;
		return true;
	}
	if (access(clientData.fileName.c_str(), F_OK) == 0)
		return false;
	writeToFile(clientData, filePath, fileContent);
	clientData.bodyString.erase(0, nextBoundaryStart);
	clientData.flagBodyRead = true;
	return true;
}

std::string HttpResponse::responsePost(clientState &clientData) {
	std::string route = "./www" + clientData.requestLine[1];
	if (!is_valid_str(route))
		return genericHttpCodeResponse(400, httpErrorMap.at(400));

	size_t pos = route.find_last_of('.');
	std::string contentType = g_mimeTypes[route.substr(pos + 1)];

	clientData.boundary = findBoundary(clientData.header);
	if (!parseRequestBody(clientData)) {
		std::ifstream file(clientData.fileName.c_str());
		std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		size_t pos = clientData.fileName.find_last_of('.');
		std::string contentType = g_mimeTypes[clientData.fileName.substr(pos + 1)];
		_status_line = clientData.requestLine[2] + " 302 Found\r\n";
		return buildHttpResponse(_status_line, contentType, buffer, clientData);
	}
	clientData.bodyString.clear();
	if (clientData.flagFileStatus == true)
		return genericHttpCodeResponse(400, httpErrorMap.at(400));
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
	const std::string& filePath = clientData.serverData.root + filename;

	FILE* file = std::fopen(filePath.c_str(), "r");
	if (!file)
		return genericHttpCodeResponse(404, "File not found.");
	std::fclose(file);
	if (std::remove(filePath.c_str()) == 0)
		return genericHttpCodeResponse(200, "File deleted successfully.");
	else
		return genericHttpCodeResponse(500, std::strerror(errno));
}

std::string HttpResponse::responseRedirect(clientState &clientData) {
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

bool HttpResponse::checkSuffix(const std::string &str, const std::string &suffix) {
	if (str.size() >= suffix.size()) {
		return (str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0);
	}
	return false;
}

std::string HttpResponse::processCgi(clientState &clientData) {
	if (clientData.isForked == true)
		return parentProcess(clientData);
	INFO("CGI start on socket: " << clientData.socketFd);
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
		exit(42);
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
			ERROR("CGI script timed out on socket: " << clientData.socketFd);
			kill(clientData.pid, SIGKILL);
			close(clientData.fd[0]);
			clientData.isForked = false;
			return genericHttpCodeResponse(504, httpErrorMap.at(504));
		}
		return result; // result is empty at this stage
	} else if (resultPid == -1) {
		ERROR("waitpid failed on socket: " << clientData.socketFd);
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
			ERROR("CGI Script exited with error status: " + std::to_string(exitStatus) + " on socket: " << clientData.socketFd);
			close(clientData.fd[0]);
			clientData.isForked = false;
			return genericHttpCodeResponse(500, httpErrorMap.at(500));
		}
	} else {
		ERROR("CGI script did not exit normally on socket: " << clientData.socketFd);
		close(clientData.fd[0]);
		clientData.isForked = false;
		return genericHttpCodeResponse(500, httpErrorMap.at(500));
	}
}

void	HttpResponse::execute(clientState &clientData) {
	std::string scriptname;
	std::string query;
	
	scriptname = clientData.serverData.root + clientData.requestLine[1];
	if (clientData.method == POST) {
		query = clientData.bodyString;
	} else if (clientData.method == GET) {
		size_t pos = clientData.requestLine[1].find('?');
		if (pos != std::string::npos) {
			scriptname = clientData.serverData.root + clientData.requestLine[1].substr(0, pos);
			query = clientData.requestLine[1].substr(pos + 1);
		}
	}

	if (checkSuffix(scriptname, ".py") == true) {
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
	}

	if (checkSuffix(scriptname, ".sh")) {
		char *args[] = {
			const_cast<char *>(scriptname.c_str()),
			NULL
		};
		execve(args[0], args, environ);
	}
	
	ERROR("execve failed");
}

bool isMethodsAllowed(clientState &clientData) {
	std::vector<Location> locations = clientData.serverData.location;
	std::string path;
	size_t slashPos = clientData.requestLine[1].find_first_of("/", 1);
	if (slashPos != std::string::npos)
		path = clientData.requestLine[1].substr(0, slashPos);
	else
		path = clientData.requestLine[1];

	for (auto &loc : clientData.serverData.location) {
		if (loc.path == path) {
			for (auto &method : loc.methods) {
				if (method == clientData.requestLine[0])
					return true;
			}
			return false;
		}
	}

	std::string locationPath = (clientData.method == DELETE ? "/upload" : "/");
	auto loc = std::find_if(locations.begin(), locations.end(), [&locationPath](const Location& loc) {
		return loc.path == locationPath;
	});

	if (loc == locations.end() && clientData.method == DELETE) {
		locationPath = "/";
		loc = std::find_if(locations.begin(), locations.end(), [&locationPath](const Location& loc) {
			return loc.path == locationPath;
		});
	}

	if (loc != locations.end()) {
		for (auto &method : loc->methods) {
			if (method == clientData.requestLine[0]) {
				return true;
			}
		}
	}
	return false;
}

std::string HttpResponse::respond(clientState &clientData) {
	if (isMethodsAllowed(clientData) == false)
		return genericHttpCodeResponse(405, httpErrorMap.at(405));

	if (clientData.requestLine[1] == "/redirect") {
		return responseRedirect(clientData);
	} else if (clientData.requestLine[1].size() > 4 && clientData.requestLine[1].substr(0, 4) == "/cgi") {
		return processCgi(clientData);
	} else if (clientData.requestLine[0] == "GET") {
		return responseGet(clientData);
	} else if (clientData.requestLine[0] == "POST") {
		if (clientData.flagFileSizeTooBig)
			return (genericHttpCodeResponse(413, httpErrorMap.at(413)));
		return responsePost(clientData);
	} else if (clientData.requestLine[0] == "DELETE") {
		return responseDelete(clientData);
	} else {
		return genericHttpCodeResponse(405, httpErrorMap.at(405));
	}
}
