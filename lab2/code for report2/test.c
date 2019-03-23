#include <unistd.h>

int main()
{
    char *prog_argv[4];

    /* 建立参数表 */
    prog_argv[0] = "/usr/bin/vi";
    prog_argv[1] = "a.cpp";
    prog_argv[2] = NULL;
    prog_argv[3] = NULL;
    //  args[2] =
    //  args[2] = NULL;
    int ret;
    execvp(prog_argv[0], prog_argv);
    if (ret == -1)
        perror("execvp");
}
