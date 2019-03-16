
  #include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc,char* argv[])
{
	int pid;
	char *prog_argv[3];

	/* 建立参数表 */
	prog_argv[0]="/usr/bin/vi";
	prog_argv[1]="vi";
	prog_argv[2]=NULL;

	/* 为命令ls创建进程 */
	if ((pid=fork())<0)
	{
		perror("Fork failed");
		exit(errno);
	}

	if (!pid)/* 这是子进程，执行命令ls */
	{
		printf("argc = %d, argv[0] = %s",argc,argv[0]);
		int ret = execv(prog_argv[0],prog_argv);
	}

	if (pid)/* 这是父进程，等待子进程执行结束*/
	{
		waitpid(pid,NULL,0);
	}

	return 0;
}
