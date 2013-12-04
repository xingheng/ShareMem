//
//  main.c
//  Child
//
//  Created by Will Han on 11/22/13.
//  Copyright (c) 2013 Will Han. All rights reserved.
//
//  Child Process

#include "Public.h"

int main(int argc, const char * argv[])
{
    sleep(IntervalsForDebug);
    
    printf("Hello, I'm child.\n");
    
    int fd = -1;
    if (argc == 2)
    {
        fd = atoi(argv[1]);
    }
    else if ( argc == 3)
    {
        const char *memName = argv[2];
        fd = shm_open(memName, O_RDWR/*|O_CREAT*/|O_EXCL/*, 0666*/);
    }
    
    if(fd == -1) {
        perror("shm_open");
        return 1;
    }
    
#if SAMPLE2
    size_t *numSize = NULL;
    numSize = mmap(NULL, sizeof(size_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (numSize != MAP_FAILED)
        printf("size: %zu\n", *numSize);
    else
        perror("mmap");
    
    size_t realSize = *numSize;
    munmap(numSize, sizeof(size_t));
    
    char *memory = mmap(NULL, realSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory != MAP_FAILED)
    {
        memory += sizeof(size_t);
        printf("memory: %s\n", memory);
        
        strncpy(memory, "I_am_a_child_process.", realSize);
    }
    else
        perror("mmap");
    
#elif SAMPLE1
    container *memory;
    memory = mmap(NULL, sizeof(container), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    printf("counter value:\n");
    // prints nothing
    printf("%d\n", memory->counter);
#endif
    
    return 0;
}
