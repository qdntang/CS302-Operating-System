#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>

int main()
{
<<<<<<< HEAD
	int cpid;/* ±£´æ×Ó½ø³ÌµÄidºÅ */
	int ppid;/* ±£´æ¸¸½ø³ÌµÄidºÅ */
	char buf[256];
  
	ppid=getpid();//µÃµ½½ü³ÌºÅ
=======
	int cpid;/* ä¿å­˜å­è¿›ç¨‹çš„idå· */
	int ppid;/* ä¿å­˜çˆ¶è¿›ç¨‹çš„idå· */
	char buf[256];
  
	char *prog_argv[2];
	prog_argv[0] = "/usr/bin/vi";
	prog_argv[1] = NULL;
	
	pid_t spid = tcgetsid(0);

	ppid=getpid();//å¾—åˆ°è¿‘ç¨‹å·
>>>>>>> 608322e088af48bad43f7e3bd06e95d1dbd917d1
	cpid=fork();
  
	if (cpid<0)
		exit(-1);
<<<<<<< HEAD
    
=======
  	  
>>>>>>> 608322e088af48bad43f7e3bd06e95d1dbd917d1
	if (!cpid)
	{
		fprintf(stdout,"ID(child)=%d\n",getpid());

<<<<<<< HEAD
		/* Ê¹×Ó½ø³ÌËùÔÚµÄ½ø³Ì×é³ÉÎªÇ°Ì¨½ø³Ì×é£¬È»ºóÖ´ÐÐvi */
		setpgid(0,0);
		tcsetpgrp(0,getpid());
		execvp("/bin/vi","vi",NULL);
		exit(-1);
	}
   
	fprintf(stdout,"ID(parent)=%d\n",ppid);
	setpgid(cpid,cpid);/* ÉèÖÃ½ø³Ì×é */
	tcsetpgrp(0,cpid);/* ÉèÖÃ¿ØÖÆÖÕ¶ËÎª×Ó½ø³ÌÓµÓÐ */
	waitpid(cpid,NULL,0);/* ¸¸½ø³ÌµÈ´ý×Ó½ø³ÌÖ´ÐÐÍê±Ï£¬ËùÔÚ½ø³Ì×é³ÉÎªÇ°Ì¨½ø³Ì×é */
	tcsetpgrp(0,ppid);

	//¸¸½ø³ÌµÈ´ýÖÕ¶ËÊäÈë£¬È»ºó»ØÏÔ
=======
		/* ä½¿å­è¿›ç¨‹æ‰€åœ¨çš„è¿›ç¨‹ç»„æˆä¸ºå‰å°è¿›ç¨‹ç»„ï¼Œç„¶åŽæ‰§è¡Œvi */
		//setpgid(0,0);
		tcsetpgrp(0,getpid());
		int ret = execv(prog_argv[0], prog_argv);
		fprintf(stdout, "fuck");
		exit(-1);
	}
  	signal(SIGTTOU, SIG_IGN); 
	fprintf(stdout,"ID(parent)=%d\n",ppid);
	setpgid(cpid,cpid);/* è®¾ç½®è¿›ç¨‹ç»„ */
	tcsetpgrp(0,cpid);/* è®¾ç½®æŽ§åˆ¶ç»ˆç«¯ä¸ºå­è¿›ç¨‹æ‹¥æœ‰ */
	waitpid(cpid,NULL,0);/* çˆ¶è¿›ç¨‹ç­‰å¾…å­è¿›ç¨‹æ‰§è¡Œå®Œæ¯•ï¼Œæ‰€åœ¨è¿›ç¨‹ç»„æˆä¸ºå‰å°è¿›ç¨‹ç»„ */
	setpgid(0,ppid);

	int ret = tcsetpgrp(0,ppid);

	//çˆ¶è¿›ç¨‹ç­‰å¾…ç»ˆç«¯è¾“å…¥ï¼Œç„¶åŽå›žæ˜¾
>>>>>>> 608322e088af48bad43f7e3bd06e95d1dbd917d1
	while(1)
	{
		memset(buf, 0, 256);
		fgets(buf, 256, stdin);
		puts("ECHO: ");
		puts(buf);
		puts("\n");
	}

	return 0;
}
