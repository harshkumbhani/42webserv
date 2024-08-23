#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

namespace fs = std::filesystem;

std::string urlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }

    return escaped.str();
}

std::string generateDirectoryTreeHTML(const std::string& directoryPath, int depth = 0) {
    std::ostringstream html;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        const auto& path = entry.path();
        std::string filename = path.filename().string();
        std::string encodedFilename = urlEncode(filename);

        if (entry.is_directory()) {
            // Directory: Create a collapsible node
            html << std::string(depth * 4, ' ') << "<div>\n"
                 << std::string(depth * 4 + 2, ' ') << "<span style=\"cursor:pointer;\" onclick=\"toggleDirectory('dir-" << encodedFilename << "')\">📁 " << filename << "</span>\n"
                 << std::string(depth * 4 + 2, ' ') << "<div id=\"dir-" << encodedFilename << "\" style=\"display:none; margin-left:20px;\">\n"
                 << generateDirectoryTreeHTML(path.string(), depth + 1) // Recursively generate for subdirectories
                 << std::string(depth * 4 + 2, ' ') << "</div>\n"
                 << std::string(depth * 4, ' ') << "</div>\n";
        } else {
            // File: Just list the file with a delete link
            html << std::string(depth * 4 + 4, ' ') << "<div>📄 <a href=\"" << encodedFilename << "\">" << filename << "</a>\n"
                 << std::string(depth * 4 + 4, ' ') << "<a href=\"/delete?file=" << encodedFilename << "\" style=\"margin-left:10px;\" onclick=\"return confirm('Are you sure you want to delete " << filename << "?');\">Delete</a></div>\n";
        }
    }

    return html.str();
}

int main() {
    // Specify the directory you want to list
    std::string directoryPath = "./www/upload/"; // Current directory
    std::string outputFileName = "./www/pages/delete.html";

    std::ofstream htmlFile(outputFileName);

    if (!htmlFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 42;
    }

    // Generate the directory listing HTML
    htmlFile << generateDirectoryTreeHTML(directoryPath);

    // Output the HTML to be served (for demonstration, we'll just print it to the console)
    // htmlFile << html;
    htmlFile.close();

    return 0;
}
