#include "Utils.hpp"

std::map<std::string, std::string> g_mimeTypes;

bool parseMimeTypes(const std::string &filename) {

  std::ifstream file(filename);
  if (!file.is_open()) {
    ERROR("Unable to open " + filename);
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);

    if (line.empty() == true)
      continue;
    std::string extension, mimeType;

    if (!(ss >> extension >> mimeType)) {
      WARNING("Error in " + filename + " line: " + line);
      continue;
    }
    g_mimeTypes[extension] = mimeType;
  }

  file.close();
  return true;
}
