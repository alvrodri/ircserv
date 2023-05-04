#include "./server/Server.hpp"
#include "./utils/Utils.hpp"
#include <stdexcept>

int	main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	if (!isNumber(argv[1]) || std::atoi(argv[1]) == 0) {
		std::cout << "You must specify a valid port." << std::endl;
		return 1;
	}

	if (!isPrintable(argv[2])) {
		std::cout << "You must specify a valid password." << std::endl;
        return 1;
    }

	try {
		Server	server(std::atoi(argv[1]), argv[2]);
	} catch (std::exception const &e) {
		std::cout << e.what() << std::endl;
	}
}
