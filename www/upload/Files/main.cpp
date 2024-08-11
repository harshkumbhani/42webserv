#include "BitcoinExchange.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: Ivalid argument"<< std::endl;
        return 1;
    }

    std::string dbFile = "data.csv";
    std::string inputFile = argv[1];

    BitcoinExchange btcExchange(dbFile);
    btcExchange.processInputFile(inputFile);

    return (0);
}

