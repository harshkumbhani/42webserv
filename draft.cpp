#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

std::string generateDirectoryListingHTML(const std::string& directoryPath) {
    std::ostringstream html;

    // Start the table with headers for Icon, Name, and Action
    html << "<tr>\n"
         << "    <th>Icon</th>\n"
         << "    <th>Name</th>\n"
         << "    <th>Action</th>\n"
         << "</tr>\n";

    // Iterate over the directory contents
		for (const auto& entry : fs::directory_iterator(directoryPath)) {
						const auto& path = entry.path();
						std::string filename = path.filename().string();
						std::string icon = entry.is_directory() ? "📁" : "📄"; // Folder or file icon
						std::string deleteLink = "/delete?file=" + filename; // URL for delete

						// Generate the HTML row for each file or directory
						html << "<tr>\n"
								<< "    <td>" << icon << "</td>\n"
								<< "    <td><a href=\"" << directoryPath + "/" + filename << "\">" << filename << "</a></td>\n"
								<< "    <td><button onclick=\""
								<< "fetch('" << deleteLink << "', {method: 'DELETE'})"
								<< ".then(function(response) { "
								<< "if (response.ok) { "
								<< "window.location.reload();"
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
