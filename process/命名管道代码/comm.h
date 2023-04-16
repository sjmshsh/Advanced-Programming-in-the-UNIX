#pragma once 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

// 客户端和服务端使用同一个命名管道
#define FILE_NAME "myfifo"
