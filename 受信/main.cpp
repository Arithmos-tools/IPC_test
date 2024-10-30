#define MSG_WAITALL 0x1000
struct msgbuf
{
    long mtype;
    char mtext[200];
};
template <typename T>
unsigned long long strlen(T *str)
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
signed long long receive(unsigned long long IPCid, msgbuf *buffer)
{
    signed long long returnValue;
    asm volatile("movq %1, %%rdi\n\t"
                 "movq $0x46, %%rax\n\t"
                 "movq $200, %%rdx\n\t"
                 "movq %2, %%rsi\n\t"
                 "movq $1, %%r10\n\t"
                 "movq %3, %%r8\n\t"
                 "syscall\n\t"
                 "movq %%rax, %0\n\t"
                 : "=r"(returnValue)
                 : "r"(IPCid), "r"(buffer), "r"((unsigned long long)0)
                 : "%rdi", "%rax", "%rsi", "%r8", "%r10", "%rdx");
    return returnValue;
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
                 : "r"((unsigned long long)1234567890), "r"((unsigned long long)0644)
                 : "%rdi", "%rax", "%rsi");
    if (IPCid < 0)
    {
        print("IPC queue bot found\n");
        exit(IPCid);
    }
    print("looking for a message\n");
    while(true)
    {
        msgbuf message;
        message.mtype = 1;
        signed long long returnValue = receive(IPCid, &message);
        if(returnValue >= 0)
        {
            print(message.mtext);
            print("\n");
        }
            
        else
            print("The message cannot be read\n");
    }
    
    // 相手のプロセスが情報を読み取るために信号を送ったほうがいいですが、この状況では、ターゲットプロセスのIDがわからないので、タイムアウトを使用します。
    return 0;
}
