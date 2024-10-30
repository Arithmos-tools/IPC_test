#define IPC_CREAT 0x200
#define IPC_EXCL 0x200
struct msgbuf {
    long mtype;      
    const char *mtext;
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
unsigned long long send(unsigned long long IPCid, const char* message)
{
    signed long long returnValue;
    msgbuf buffer;
    buffer.mtext = message;
    buffer.mtype = 1;
    asm volatile("movq %1, %%rdi\n\t"
                 "movq $0x45, %%rax\n\t"
                 "movq %2, %%rdx\n\t"
                 "movq %3, %%rsi\n\t"
                 "movq $0, %%r10\n\t"
                 "syscall\n\t"
                 "movq %%rax, %0\n\t"
                 : "=r"(returnValue)
                 : "r"(IPCid), "r"(strlen(message)), "r"(&buffer)
                 : "%rdi", "%rax", "%rsi");
    return returnValue;
}
int main()
{
    signed long long returnValue = 0;
    asm volatile("movq %1, %%rdi\n\t"
                 "movq $0x44, %%rax\n\t"
                 "movq %2, %%rsi\n\t"
                 "syscall\n\t"
                 "movq %%rax, %0\n\t"
                 : "=r"(returnValue)
                 : "r"((unsigned long long)1234567890), "r"((unsigned long long)IPC_CREAT | IPC_EXCL | 0644)
                 : "%rdi", "%rax", "%rsi");
    if (returnValue < 0)
    {
        print("IPC queue already exists\n");
        exit(returnValue);
    }
    returnValue = send(returnValue, "Did you receive that ?");
    if(returnValue >= 0)
        print("Message sent successfully\n");
    //相手のプロセスが情報を読み取るために信号を送ったほうがいいですが、この状況では、ターゲットプロセスのIDがわからないので、タイムアウトを使用します。
    return 0;
}
