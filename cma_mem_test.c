#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <assert.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#include <pthread.h>
#include <poll.h>
#include <semaphore.h>

#define CMEM_IOCTL_MAGIC	'm'
#define CMEM_GET_PHYS		_IOW(CMEM_IOCTL_MAGIC, 1, unsigned int)
#define CMEM_MAP		_IOW(CMEM_IOCTL_MAGIC, 2, unsigned int)
#define CMEM_GET_SIZE		_IOW(CMEM_IOCTL_MAGIC, 3, unsigned int)
#define CMEM_UNMAP		_IOW(CMEM_IOCTL_MAGIC, 4, unsigned int)

#define CMEM_ALLOCATE		_IOW(CMEM_IOCTL_MAGIC, 5, unsigned int)

#define CMEM_CONNECT		_IOW(CMEM_IOCTL_MAGIC, 6, unsigned int)

#define CMEM_GET_TOTAL_SIZE	_IOW(CMEM_IOCTL_MAGIC, 7, unsigned int)
#define CMEM_CACHE_FLUSH	_IOW(CMEM_IOCTL_MAGIC, 8, unsigned int)

struct cmamem_info {
	char *name;
	char is_cache;
	unsigned long id;
	unsigned long offset;
	unsigned long len;
	unsigned long phy_base;
	unsigned long mem_base;
	//struct list_head list;
};

int main()
{
	int cmem_fd;
	void *cmem_base;
	unsigned int size;
	struct cmamem_info region;
	int i;
	cmem_fd = open("/dev/cma_mem", O_RDWR, 0);//打开设备，为了操作硬件引擎，要noncache的
	printf("cmem_fd:%d\n", cmem_fd);
	if (cmem_fd >= 0)
		{ 
		memset(&region, 0x00, sizeof(struct cmamem_info));
		region.len = 800 * 480 * 4;
		if (ioctl(cmem_fd, CMEM_ALLOCATE, &region) < 0) //获取全部空间
			{
			perror("PMEM_GET_TOTAL_SIZE failed\n");
			return -1;
			}

		size = region.len;

		cmem_base = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, cmem_fd, 0);//mmap操作
		printf("cmem_base:0x%08x,region.len:0x%08x offset:0x%08x\n",(unsigned int)cmem_base, region.len, region.offset);
		printf("phy_base:0x%08x,  mem_base:0x%08x  \n",(unsigned int)region.phy_base, (unsigned int)region.mem_base);
		if (cmem_base == MAP_FAILED)
			{
			cmem_base = 0;
			close(cmem_fd);
			cmem_fd = -1;
			perror("mmap pmem error!\n");
			}

		for(i = 0; i < 10; i++)
			((unsigned int *)cmem_base)[i] = i;
		printf("pmem_base:0x%08x\n", cmem_base);
		for(i = 0; i < 10; i++)
			printf("%d\n", ((unsigned int *)cmem_base)[i]);
		}

	close(cmem_fd);
	return 0;
}
