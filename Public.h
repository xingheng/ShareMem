//
//  Public.h
//  ShareMem
//
//  Created by Will Han on 11/22/13.
//  Copyright (c) 2013 Will Han. All rights reserved.
//

#ifndef ShareMem_Public_h
#define ShareMem_Public_h

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <uuid/uuid.h>


// Switchers, unlock one of them at one time.
#define SAMPLE1     0
#define SAMPLE2     1

#if SAMPLE1
typedef struct container {
    sem_t resource, mutex;
    int counter;
} container;
#elif SAMPLE2
typedef char container[20];
#endif

/*
// size_t is 32 bits in 32bit archtecture and 64 bits in 64bit.
// size_t isn't the best type to save the length of container, if the parent process is 32bit and the child process is 64bit ...
typedef u_int64_t  Size;   // Size is 64 bits in both 32bit and 64bit environment.
*/
 
void PrintErr(int err)
{
    char * str = strerror(err);
    printf("err:%d, str:%s\n", err, str);
}

char *GenerateUUID(void)
{
    uuid_t idt = {0};
    uuid_generate_time(idt);
    
    uuid_string_t uuid_s;
    uuid_unparse(idt, uuid_s);
    
    char *res = uuid_s;
    res[8] = '\0';
    
    return res;
}


#define IntervalsForDebug           20 /* Remain the enough time to attach the child process when debugging, ONLY for debugging. */
#define IntervalsForWaiting         (5 + IntervalsForDebug)

#endif
