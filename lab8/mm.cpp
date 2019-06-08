#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32    //进程名最大长度
#define MIN_SLICE 10           //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0    //内存开始分配时的起始地址

#define FIRST_FIT 0
#define BEST_FIT 1
#define WORST_FIT 2

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = true;  //当内存以及被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
My_algo algo[123];
int alg_size;
int alg;

struct free_block {  //空闲数据块
    int size;
    int start_addr;
    struct free_block *next;
};

bool (*alg_func[123])(free_block *, free_block *);

struct allocated_block {  //已分配的数据块
    int pid;
    int size;
    int start_addr;
    char process_name[PROCESS_NAME_LEN];
    int *data;
    struct allocated_block *next;
};

free_block *free_block_head;                   //空闲数据块首指针
allocated_block *allocated_block_head = NULL;  //分配块首指针

allocated_block *find_process(int id);      //寻找pid为id的分配块
free_block *init_free_block(int mem_size);  //空闲块初始化
void display_menu();                        //显示选项菜单
void set_mem_size();                        //设置内存大小
int allocate_mem(allocated_block *ab);      //为制定块分配内存
void rearrange();                           // 对块进行重新分配
int create_new_process();                   //创建新的进程
int free_mem(allocated_block *ab);          //释放分配块
void swap(int *p, int *q);                  //交换地址
int dispose(allocated_block *ab);           //释放分配块结构体
void display_mem_usage();                   //显示内存情况
void kill_process();      //杀死对应进程并释放其空间与结构体
void Usemy_algo(int id);  //使用对应的分配算法
void set_alg();
bool first_fit(free_block *, free_block *);
bool best_fit(free_block *, free_block *);
bool worst_fit(free_block *, free_block *);

//主函数
int main() {
    int op;
    pid = 0;
    free_block_head = init_free_block(
        mem_size);  //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
    algo[FIRST_FIT] = make_pair<int, string>(FIRST_FIT, "First Fit");
    algo[BEST_FIT] = make_pair<int, string>(BEST_FIT, "Best Fit");
    algo[WORST_FIT] = make_pair<int, string>(WORST_FIT, "Worst Fit");
    alg_func[FIRST_FIT] = first_fit;
    alg_func[BEST_FIT] = best_fit;
    alg_func[WORST_FIT] = worst_fit;
    alg_size = 3;
    alg = FIRST_FIT;
    for (;;) {
        sleep(1);
        display_menu();
        fflush(stdin);
        scanf("%d", &op);
        switch (op) {
            case 1: {
                set_mem_size();
                break;
            }
            case 2: {
                set_alg();
                break;
            }
            case 3: {
                create_new_process();
                break;
            }
            case 4: {
                kill_process();
                break;
            }
            case 5: {
                display_mem_usage();
                break;
            }
            case 233: {
                puts("bye....");
                sleep(1);
                return 0;
            }
            default:
                break;
        }
    }
}

allocated_block *find_process(
    int id) {  //循环遍历分配块链表，寻找pid=id的进程所对应的块
    allocated_block *p = allocated_block_head;
    while (p != NULL) {
        if (p->pid == id) {
            return p;
        }
        p = p->next;
    }
    return p;
}

free_block *init_free_block(
    int mem_size) {  //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
    free_block *p;
    p = (free_block *)malloc(sizeof(free_block));
    if (p == NULL) {
        puts("No memory left");
        return NULL;
    }
    p->size = mem_size;
    p->start_addr = DEFAULT_MEM_START;
    p->next = NULL;
    return p;
}

void display_menu() {
    puts("\n\n******************menu*******************");
    printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
    printf("2) Set memory allocation algorithm\n");
    printf("3) Create a new process\n");
    printf("4) Kill a process\n");
    printf("5) Display memory usage\n");
    printf("233) Exit\n");
}

void set_alg() {
    printf("Please choose a No.\n");
    for (int i = 0; i < alg_size; i++) {
        printf("%d: %s\n", algo[i].first, algo[i].second.c_str());
    }
    scanf("%d", &alg);
    printf("Current algorithm %s\n", algo[alg].second.c_str());
}

void set_mem_size() {  //更改最大内存大小
    if (flag) {
        printf("Please input the mem size\n");
        scanf("%d", &mem_size);
        free_block_head->size = mem_size;
        printf("The new max memory size is %d\n", mem_size);
    } else {
        printf(
            "You can not modify memory size now cause the memory has already "
            "been allocated\n");
    }
}

int allocate_mem(
    allocated_block *ab) {  //为块分配内存，真正的操作系统会在这里进行置换等操作
    if (free_block_head == NULL) {
        printf("No memory\n");
        return 0;
    }
    rearrange();
    free_block *p = NULL;
    free_block *q = NULL;
    free_block *tmp = free_block_head;
    if (alg == FIRST_FIT || alg == BEST_FIT) {
        while (tmp != NULL) {
            if (tmp->size >= ab->size) {
                p = tmp;
                break;
            }
            q = tmp;
            tmp = tmp->next;
        }
        if (p == NULL) return 0;
        if (p->size - ab->size <= MIN_SLICE) {
            ab->start_addr = p->start_addr;
            ab->size = p->size;
            if (p == free_block_head) {
                free_block_head = free_block_head->next;
            } else {
                q->next = p->next;
            }
            free(p);
        } else {
            ab->start_addr = p->start_addr;
            p->start_addr += ab->size;
            p->size -= ab->size;
        }
    } else if (alg == WORST_FIT) {
        p = free_block_head;
        if (p->size < ab->size) return 0;
        if (p->size - ab->size <= MIN_SLICE) {
            ab->start_addr = p->start_addr;
            ab->size = p->size;
            free_block_head = free_block_head->next;
            free(p);
        } else {
            ab->start_addr = p->start_addr;
            p->start_addr += ab->size;
            p->size -= ab->size;
        }
    }
    return 1;
}

int create_new_process() {  //创建新进程
    int mem_sz = 0;
    flag = false;
    allocated_block *ab = (allocated_block *)malloc(sizeof(allocated_block));
    if (ab == NULL) {
        puts("No memory left");
        return 0;
    }
    ab->next = NULL;
    pid++;
    ab->pid = pid;
    sprintf(ab->process_name, "process%d", pid);

    printf("Please input memory size\n");
    scanf("%d", &mem_sz);
    if (mem_sz < 0) {
        puts("input memory size invalid\n");
        return 0;
    }
    ab->size = mem_sz;
    int ret = allocate_mem(ab);
    if (ret) {
        printf("Successfully create %s!\n", ab->process_name);
        if (allocated_block_head == NULL)
            allocated_block_head = ab;
        else {
            allocated_block *tmp = allocated_block_head;
            while (tmp->next) tmp = tmp->next;
            tmp->next = ab;
        }
        return 1;
    } else {
        printf("Allocate memory failed\n");
        return 0;
    }
}

void swap(int *p, int *q) {
    int tmp = *p;
    *p = *q;
    *q = tmp;
}

void rearrange() {  //将块按照地址大小进行排序
    free_block *tmp, *tmpx;
    puts("Rearrange begins...");
    puts("Rearrange by address...");
    tmp = free_block_head;
    while (tmp != NULL) {
        tmpx = tmp->next;
        while (tmpx != NULL) {
            if (tmpx->start_addr < tmp->start_addr) {
                swap(&tmp->start_addr, &tmpx->start_addr);
                swap(&tmp->size, &tmpx->size);
            }
            tmpx = tmpx->next;
        }
        tmp = tmp->next;
    }

    tmp = free_block_head;
    while (tmp != NULL) {
        tmpx = tmp->next;
        while (tmpx != NULL) {
            if (tmp->start_addr + tmp->size == tmpx->start_addr) {
                tmp->size += tmpx->size;
                tmp->next = tmpx->next;
                free(tmpx);
            }
            tmpx = tmpx->next;
        }
        tmp = tmp->next;
    }

    tmp = free_block_head;
    while (tmp != NULL) {
        tmpx = tmp->next;
        while (tmpx != NULL) {
            if (!alg_func[alg](tmp, tmpx)) {
                swap(&tmp->start_addr, &tmpx->start_addr);
                swap(&tmp->size, &tmpx->size);
            }
            tmpx = tmpx->next;
        }
        tmp = tmp->next;
    }
    usleep(500);
    puts("Rearrange Done.");
}

int free_mem(allocated_block *ab) {  //释放某一块的内存
    free_block *tmp = (free_block *)malloc(sizeof(free_block));
    if (tmp == NULL) {
        puts("No memory left");
        return 0;
    }
    tmp->start_addr = ab->start_addr;
    tmp->size = ab->size;
    if (free_block_head) {
        tmp->next = free_block_head->next;
        free_block_head->next = tmp;
    } else {
        free_block_head = tmp;
    }
    return 1;
}

int dispose(allocated_block *fab) {  //释放结构体所占的内存
    allocated_block *pre, *ab;
    if (fab == allocated_block_head) {
        allocated_block_head = allocated_block_head->next;
        free(fab);
        return 1;
    }
    pre = allocated_block_head;
    ab = allocated_block_head->next;
    while (ab != fab) {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    free(ab);
    return 2;
}

void display_mem_usage() {
    free_block *fb = free_block_head;
    allocated_block *ab = allocated_block_head;
    puts("*********************Free Memory*********************");
    printf("%20s %20s\n", "start_addr", "size");
    int cnt = 0;
    while (fb != NULL) {
        cnt++;
        printf("%20d %20d\n", fb->start_addr, fb->size);
        fb = fb->next;
    }
    if (!cnt)
        puts("No Free Memory");
    else
        printf("Totaly %d free blocks\n", cnt);
    puts("");
    puts("*******************Used Memory*********************");
    printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
    cnt = 0;
    while (ab != NULL) {
        cnt++;
        printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name,
               ab->start_addr, ab->size);
        ab = ab->next;
    }
    if (!cnt)
        puts("No allocated block");
    else
        printf("Totaly %d allocated blocks\n", cnt);
    return;
}

void kill_process() {  //杀死某个进程
    allocated_block *ab;
    int pid;
    puts("Please input the pid of Killed process");
    scanf("%d", &pid);
    if (pid < 0) {
        printf("pid invalid\n");
    }
    ab = find_process(pid);
    if (ab != NULL) {
        free_mem(ab);
        dispose(ab);
    } else {
        printf("Can not find process with pid = %d\n", pid);
    }
}

bool first_fit(free_block *lhs, free_block *rhs) {
    return lhs->start_addr < rhs->start_addr;
}

bool best_fit(free_block *lhs, free_block *rhs) {
    return lhs->size < rhs->size;
}

bool worst_fit(free_block *lhs, free_block *rhs) {
    return lhs->size > rhs->size;
}
