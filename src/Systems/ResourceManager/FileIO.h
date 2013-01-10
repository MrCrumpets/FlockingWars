#ifndef FILEIO_H
#define FILEIO_H
#include <istream>

template <typename DataType>
DataType readData(std::istream& stream){
	DataType in;
	if(!(stream >> in))
		std::cout << "Failed to read data" << std::endl;
	return in;
}
#endif
