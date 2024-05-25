#include "Config.hpp"

int	main(int argc, char **argv) {
	if (argc != 2)
		std::cout << "\033[22;31m" << "Oops!!, you might need to follow:\n" << \
		 "\033[22;31m" << "-->USAGE: ./executable configFile" << std::endl;
	else {
		Config	obj;
		obj.readConfigurations(argv[1]);
	}
}
