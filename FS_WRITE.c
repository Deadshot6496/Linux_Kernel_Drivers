

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    
	// Create a new file or open it
	//if it exists (write-only access)
	// File permissions: rw-r--r--
	
	char *value = "100";
	int fd = open("/sys/module/myModule/parameters/cb_valueETX", O_RDWR);
	int fd_1 = open("/dev/MY_DEVICE", O_RDONLY);
	int sizeRead = 0;
	char buffer_read[256];
	if(fd_1 == -1)
	{
		perror("Error opening DEVICE FILE");
		close(fd);
		close(fd_1);

		return -1;
	}
	if (fd == -1) {
		perror("Error opening file");
		close(fd);
		close(fd_1);

		return 1;
	}

	write(fd, value, strlen(value));
	sizeRead = read(fd_1,buffer_read,sizeof(buffer_read)-1);
	printf("%s\n",buffer_read);
	if(sizeRead == -1)
	{
		perror("error reading DEVICE file");
		close(fd_1);
		close(fd);

		return -1;
	}
	// Close the file descriptor
	close(fd);

	return 0;
} 

/*#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main ()
{
	printf("Started");
	printf "Value: %s\n" "77" >> /sys/module/myModule/parameters/cb_valueETX.txt;
	int f = open("/sys/module/myModule/parameters/cb_valueETX", O_RDWR);
	if(f == 1)
	{
		printf("ERROR");
	}

	if(write(f, "77", 2) == 1)
	{
		printf("ERROR");
	}
	close(f);
}
*/
