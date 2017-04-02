#include "lock.h"

#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>

char* lock_filename = "/tmp/screensht.lock";

int lock_init()
{
	int fd = open("/tmp/screensht.lock", O_CREAT | O_RDWR);

	if (fd == -1)
	{
		printf("couldn't create lock file\n");
		close(fd);
		return 1;
	}

	struct flock file_lock;
	file_lock.l_type = F_WRLCK;
	file_lock.l_whence = SEEK_SET;
	file_lock.l_start = 0;
	file_lock.l_len = 0;
	file_lock.l_pid = getpid();

	if (fcntl(fd, F_SETLK, &file_lock) == -1)
	{
		close(fd);
		return 1;
	}

	close(fd);

	return 0;
}

void lock_kill()
{
	remove(lock_filename);
}
