#include "../inc/Server.hpp"


int main(int ac, char **av) {
	Server serv;
	if (ac != 3)
		std::cout << RED << "Invalid arguments USAGE: " << av[0] << " [PORT] [PASSWORD]" << END << std::endl;
	try {
		serv.createServer(std::atoi(av[1]), av[2]);//static_cast<std::string>(av[2]));		

	} catch (std::exception &e) {
		std::cout << RED << e.what() << END << std::endl;
	}
}