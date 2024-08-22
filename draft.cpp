#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

std::string generateDirectoryListingHTML(const std::string& directoryPath) {
    std::ostringstream html;

    // Start the table
    html << "<tr>\n"
         << "    <th>Icon</th>\n"
         << "    <th>Name</th>\n"
         << "</tr>\n";

    // Iterate over the directory contents
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        const auto& path = entry.path();
        std::string filename = path.filename().string();
        std::string icon = entry.is_directory() ? "📁" : "📄"; // Folder or file icon

        // Generate the HTML row for each file or directory
        html << "<tr>\n"
             << "    <td>" << icon << "</td>\n"
             << "    <td><a href=\"" << filename << "\">" << filename << "</a></td>\n"
             << "</tr>\n";
    }

    return html.str();
}

int main() {
    // Specify the directory you want to list
    std::string directoryPath = "./www/upload/"; // Current directory

    // Generate the directory listing HTML
    std::string html = generateDirectoryListingHTML(directoryPath);

    // Output the HTML to be served (for demonstration, we'll just print it to the console)
    std::cout << html;

    return 0;
}
