//
//  main.c
//  ShareMem
//
//  Created by Will Han on 11/22/13.
//  Copyright (c) 2013 Will Han. All rights reserved.
//
//  Parent Process

#include "Public.h"

#if SAMPLE2

int main()
{
    const char *uuid = GenerateUUID();
//    printf("uuid: %s\n", uuid);
    
    char *memory;
    int fd = shm_open(uuid, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR/*, 0666*/);
    
    if(fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    int fdflags = fcntl(fd, F_GETFD);
    if (fdflags & FD_CLOEXEC)
    {
        fdflags &= ~FD_CLOEXEC;
        fcntl(fd, F_SETFD, fdflags);
    }
    
    size_t shmSize = sizeof(container);
    shmSize += sizeof(size_t);  // we need to save main memory size at the head.
    
    int ret = ftruncate(fd, shmSize);
    if (ret != 0)
    {
        printf("ftruncate ret: %d, errno: %d.\n", ret, errno);
        return 1;
    }
    
    /*
    // Check the permission mode
    struct stat st;
    ret = fstat(fd, &st);
    int mode = st.st_mode;
    printf("mode: 0%o \n", mode);
     */
    
    char *memResult = NULL;
    memory = mmap(NULL, shmSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory != MAP_FAILED)
    {
        *((size_t*)memory) = shmSize;
        
        memResult = memory + sizeof(size_t);
        strncpy(memResult, "hanwei_is_just_a_name_right?_<end>", sizeof(container));
        printf("Before child process, size: %zu, memResult: %s\n", shmSize, memResult);
    }
    else
        perror("mmap");
    
    // child process ...
    
    char buffer[256];
    sprintf(buffer, "./Child %d", fd);
//    sprintf(buffer, "./Child %d %s", fd, memName);
    int status = system(buffer);
    sleep(IntervalsForWaiting);
    
    printf("child status: %d\n", status);
    printf("After child process, memResult: %s\n", memResult);
    
    return 0;
}

#elif SAMPLE1

int main()
{
    container *memory;
    const char *memName = "MYSHM68434161222222";
    int fd = shm_open(memName, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR/*, 0666*/);
    //    fd = shm_open(memName, O_RDWR|O_CREAT|O_EXCL, 0666);
    //    fd = shm_open(memName, O_RDWR, 0666);
    
    if(fd == -1) {
        perror("shm_open");
        return 1;
    }
    
    /*
     Link: http://stackoverflow.com/questions/2862027/mmap-and-structure
     it's failing because you are trying to access the file descriptor in a child process after exec(), and shm_open sets the FD_CLOEXEC flag by default so the file descriptor is no longer open in the child. So you just need to unset that flag in the main process
     */
    
    int fdflags = fcntl(fd, F_GETFD);
    fdflags &= ~FD_CLOEXEC;
    fcntl(fd, F_SETFD, fdflags);
    
    
    ftruncate(fd, sizeof(container));
    memory = mmap(NULL, sizeof(container), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    memory->counter = 5;
    printf("%d\n", memory->counter);
    
    
    // child process ...
    
    char buffer[256];
    sprintf(buffer, "./Child %d %s", fd, memName);
    int status = system(buffer);
    printf("child status: %d", status);
    
    sleep(9999 /* seconds */); // sleep the main process all the time.
    
    return 0;
}
#endif
