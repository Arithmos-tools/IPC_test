#define IPC_CREAT 0x200
#define IPC_EXCL 0x200
struct msgbuf {
    long mtype;      
    const char *mtext;
};
unsigned long long strlen(const char *str)
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
template <typename T>
void print(T *str)
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
char* receive()
{

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
                 : "r"((unsigned long long)1234567890), "r"((unsigned long long)0644)
                 : "%rdi", "%rax", "%rsi");
    if (returnValue < 0)
    {
        print("IPC queue bot found\n");
        exit(returnValue);
    }
    print("looking for a message\n");
    char* message = receive();
    print(message);
    //相手のプロセスが情報を読み取るために信号を送ったほうがいいですが、この状況では、ターゲットプロセスのIDがわからないので、タイムアウトを使用します。
    return 0;
}
