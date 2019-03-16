#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
	int pid;
	char *prog_argv[4];

	/* 建立参数表 */
	prog_argv[0]="/bin/ls";
	prog_argv[1]="-l";
	prog_argv[2]="/";
	prog_argv[3]=NULL;
	int pipe_fd[2];
	pipe(pipe_fd);
	/* 为命令ls创建进程 */
	if ((pid=fork())<0)
	{
		perror("Fork failed");
		exit(errno);
	}
	printf("%d\n", pipe_fd[1]);
	if (!pid)/* 这是子进程，执行命令ls */
	{
		printf("%d\n", stdin->_fileno);;
		printf("%d\n", stdout->_fileno);
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		//close(pipe_fd[1]);  
		printf("fuck\n");
		//write(pipe_fd[1], "fuck", 4);
		//printf("%d\n", stdout);
		//fprintf(pipe_fd[1], "fuck\n");
		execvp(prog_argv[0],prog_argv);
	}

	if (pid)/* 这是父进程，等待子进程执行结束*/
	{
		waitpid(pid,NULL,0);
		char buff[1024];
		read(pipe_fd[0], buff, 1024);
		//printf("%s\n", buff);
	}

	return 0;
}
