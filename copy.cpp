#include <iostream>
#include <sys/types.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int find_logical_size(int fd){


 int cur = lseek(fd, 0, SEEK_CUR);


 if(lseek(fd, 0, SEEK_SET) < 0) exit(errno);


 	int size = lseek(fd, 0, SEEK_END);

 	if(lseek(fd, cur, SEEK_SET) < 0) exit(errno);

  return size;

}

int main(int argc, char* argv[]){

	if(argc < 3){
		std::cerr << "Not enough arguments" << std::endl;
		return 1;
	}

	char* path = argv[1];
	char* path1 = argv[2];

	int source_fd = open(path, O_RDONLY);
	int destination_fd = open(path1, O_CREAT | O_WRONLY | O_TRUNC, 0644);

	char* buffer = new char[BUFFER_SIZE];

	int cur{}, start{}, end{}, blockSize{},miniBlock{};
	while(true){
		start = cur;
		cur = lseek(source_fd, cur, SEEK_DATA);
		end = cur;
		blockSize = end - start;
		if(blockSize > 0){
			lseek(destination_fd, blockSize, SEEK_END);
		}
		start = cur;
		cur = lseek(source_fd, cur, SEEK_HOLE);
		end = cur;
		blockSize = end - start;

		lseek(source_fd, start, SEEK_SET);
		if(blockSize < BUFFER_SIZE){
			int readBytes = read(source_fd, buffer, blockSize);
			int writeBytes = write(destination_fd, buffer, blockSize);
		}
		if(blockSize > BUFFER_SIZE){
			while(true){
				int readBytes = read(source_fd, buffer, BUFFER_SIZE);
				int writeBytes = write(destination_fd, buffer, BUFFER_SIZE);
				miniBlock += BUFFER_SIZE;
				if(miniBlock + BUFFER_SIZE > blockSize){
					int lastSize = blockSize - miniBlock;
					readBytes = read(source_fd, buffer, lastSize);
					writeBytes = write(destination_fd, buffer, lastSize);
					break;
				}
			}
		}
		if(cur == find_logical_size(source_fd)){
			break;
		}
	}


	delete [] buffer; 
	return 0;
}
