#define IPC_CREAT 0x200
#define IPC_EXCL 0x200
struct msgbuf
{
    long mtype = 1;
    char message[22];
};
struct __kernel_timespec
{
    unsigned long long tv_sec;
    unsigned long long tv_nsec;
};
constexpr unsigned long long strlen(const char *str)
{
    unsigned long long count = 0;
    while (*str != 0x00)
    {
        ++count;
        ++str;
    }
    return count;
}
void exit(signed long long errno)
{
    asm volatile("movq %0, %%rdi\n\t"
                 "movq $0x3C, %%rax\n\t"
                 "syscall\n\t"
                 :
                 : "r"(errno)
                 : "%rdi", "%rax");
}
void print(const char *str)
{
    asm volatile("movq $0x01, %%rdi\n\t"
                 "movq $0x01, %%rax\n\t"
                 "movq %0, %%rsi\n\t"
                 "movq %1, %%rdx\n\t"
                 "syscall\n\t"
                 :
                 : "r"(str), "r"(strlen(str))
                 : "%rdi", "%rax", "%rsi", "%rdx");
}
unsigned long long send(unsigned long long IPCid, msgbuf *buffer)
{
    signed long long returnValue;
    asm volatile("movq %1, %%rdi\n\t"
                 "movq $0x45, %%rax\n\t"
                 "movq %2, %%rdx\n\t"
                 "movq %3, %%rsi\n\t"
                 "movq $0, %%r10\n\t"
                 "syscall\n\t"
                 "movq %%rax, %0\n\t"
                 : "=r"(returnValue)
                 : "r"(IPCid), "r"(sizeof(msgbuf) - 8), "r"(buffer)
                 : "%rdi", "%rax", "%rsi");
    return returnValue;
}
void sleep()
{
    __kernel_timespec time;
    time.tv_sec = 1;
    time.tv_nsec = 0;
    asm volatile("movq %0, %%rdi\n\t"
                 "movq $0x23, %%rax\n\t"
                 "movq $0, %%rsi\n\t"
                 "syscall\n\t"
                 :
                 : "r"(&time)
                 : "%rdi", "%rax", "%rsi");
}
int main()
{
    signed long long IPCid = 0;
    asm volatile("movq %1, %%rdi\n\t"
                 "movq $0x44, %%rax\n\t"
                 "movq %2, %%rsi\n\t"
                 "syscall\n\t"
                 "movq %%rax, %0\n\t"
                 : "=r"(IPCid)
                 : "r"((unsigned long long)1234567890), "r"((unsigned long long)IPC_CREAT | IPC_EXCL | 0644)
                 : "%rdi", "%rax", "%rsi");
    if (IPCid < 0)
    {
        print("IPC queue already exists\n");
        exit(IPCid);
    }
    while (true)
    {
        msgbuf buffer;
        const char *message = "Did you receive that ?";
        for (int i = 0; i < strlen(message); ++i)
            buffer.message[i] = message[i];
        signed long long returnValue = send(IPCid, &buffer);
        if (returnValue >= 0)
            print("Message sent successfully\n");
        sleep();
    }
    // 相手のプロセスが情報を読み取るために信号を送ったほうがいいですが、この状況では、ターゲットプロセスのIDがわからないので、タイムアウトを使用します。
    return 0;
}
