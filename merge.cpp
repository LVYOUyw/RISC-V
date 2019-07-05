#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;
int pool[200005];
unsigned int order[200005],IR;
char s[1005],w[1005];
int cur_id,reg[32],rs1,rs2,imm,rd,immt,opcode;
int funct3,funct7,address,ans,pd;
int ans_mem;
typedef long long LL;
const int c3=(1<<3)-1;
const int c4=(1<<4)-1;
const int c7=(1<<7)-1;
const int c5=(1<<5)-1;
const int c6=(1<<6)-1;
const int c8=(1<<8)-1;
const int c16=(1<<16)-1;
const int c19=(1<<19)-1;
const int c24=(1<<24)-1;
const int c20=(1<<20)-1;
const int c10=(1<<10)-1;
const int c12=(1<<12)-1;
const int c11=(1<<11)-1;
const int c32=-1;

int trans(int l) 
{
    int len=strlen(s+1);
    int x=0;
    for (int i=l;i<=l+8-1;i++)
        x=(x<<4)+(s[i]>'9'?s[i]-'A'+10:s[i]-'0');
    return x;
}

int getnum(int x,int y) 
{
    x=x>'9'?x-'A'+10:x-'0';
    y=y>'9'?y-'A'+10:y-'0';
    return (x<<4)+y;
}

int extend(int x,int len) 
{ 
    LL ci=(1<<(32-len))-1;
    len--;
    return (x>>len)&1?(ci<<(len+1))|x:x;
}

void IF() 
{
    IR=order[cur_id];
    //printf("%x\n",IR);
    if (IR==0x00c68223) 
    {
        printf("%d\n",(unsigned int)reg[10]&255u);
        exit(0);
    }
    cur_id+=4;
}

void ID() 
{
    opcode=IR&c7;
    rs1=(IR>>15)&c5;
    rs2=(IR>>20)&c5;
    rd=(IR>>7)&c5;
    funct3=(IR>>12)&c3;
    funct7=(IR>>25)&c7;
    if (opcode==3||opcode==19||opcode==103) imm=(IR>>20)&c12,immt=extend(imm,12); //I-type
    if (opcode==35) imm=((IR>>7)&c5)+((IR>>25)<<5),immt=extend(imm,12); //S-type
    if (opcode==99) //B-type
    {
        imm=(IR>>8)&c4;
        imm=(((IR>>25)&c6)<<4)|imm;
        int t1=(IR>>7)&1;
        int t2=(IR>>31)&1;
        t1=t1<<10;
        t2=t2<<11;
        imm=(imm|t1|t2);
        imm<<=1;
        immt=extend(imm,13);
    }
    if (opcode==55||opcode==23) imm=(IR>>12)&c20,immt=extend(imm,20); //U-type
    if (opcode==111) 
    {
        imm=(IR>>21)&c10;
        imm+=(((IR>>12)&c8)<<11);
        imm+=(((IR>>20)&1)<<10);
        imm+=(((IR>>31)&1)<<19);
        imm<<=1;
        immt=extend(imm,21);
    }
}

void EX()
{
    if (opcode==3||opcode==35) address=reg[rs1]+immt; //load and store
    if (opcode==51)  //reg - reg alu
    {
        if (funct3==0) 
            if (funct7) ans=reg[rs1]-reg[rs2]; //sub
            else ans=reg[rs1]+reg[rs2];  //add
        else if (funct3==4) //xor
            ans=reg[rs1]^reg[rs2]; 
        else if (funct3==6) //or
            ans=reg[rs1]|reg[rs2];
        else if (funct3==7) //and
            ans=reg[rs1]&reg[rs2];
        else if (funct3==1) //sll
            ans=reg[rs1]<<(reg[rs2]&c5);
        else if (funct3==5) 
            if (funct7) ans=reg[rs1]>>(reg[rs2]&c5); //sra
            else //srl
            {
                int t1=reg[rs1];
                int t2=reg[rs2]&c5;
                ans=(t1&(1<<31))?(t1>>t2)&((1<<(32-t2))-1):t1>>t2;
            }
        else if (funct3==2) //slt
            ans=reg[rs1]<reg[rs2]?1:reg[rd];
        else if (funct3==3) //sltu
            ans=(unsigned int)reg[rs1]<(unsigned int)reg[rs2]?1:reg[rd];
    }
    if (opcode==19) //reg-imm alu
    {
        if (funct3==0) ans=reg[rs1]+immt; //addi
        else if (funct3==4) ans=reg[rs1]^immt; //xori
        else if (funct3==6) ans=reg[rs1]|immt; //ori
        else if (funct3==7) ans=reg[rs1]&immt; //andi
        else if (funct3==1) ans=reg[rs1]<<(imm&c5); //slli
        else if (funct3==5)
            if (funct7) ans=reg[rs1]>>(imm&c5); //srai
            else //srli 
            {
                int t1=reg[rs1];
                imm=imm&c5;
                ans=(t1&(1<<31))?(t1>>imm)&((1<<(32-imm))-1):t1>>imm;
            }
        else if (funct3==2) ans=reg[rs1]<immt?1:reg[rd]; //slti
        else if (funct3==3) ans=(unsigned int)reg[rs1]<(unsigned int)immt?1:reg[rd]; //sltiu
    }
    if (opcode==99) //control
    {
        address=cur_id-4+immt;
        if (funct3==0) pd=reg[rs1]==reg[rs2]; //beq
        else if (funct3==1) pd=reg[rs1]!=reg[rs2]; //bne
        else if (funct3==4) pd=reg[rs1]<reg[rs2]; //blt
        else if (funct3==5) pd=reg[rs1]>=reg[rs2]; //bge
        else if (funct3==6) pd=(unsigned int)reg[rs1]<(unsigned int)reg[rs2]; //bltu
        else if (funct3==7) pd=(unsigned int)reg[rs1]>=(unsigned int)reg[rs2]; //bgeu
    }
    if (opcode==111) ans=cur_id,address=cur_id-4+immt,pd=1; //jal
    if (opcode==103) ans=cur_id,address=reg[rs1]+immt,pd=1; //jalr
    if (opcode==55) ans=imm<<12; //lui
    if (opcode==23) address=cur_id-4+(imm<<12),ans=address; //auipc
} 

void MEM() 
{
    if (opcode==3) //load 
    {
        if (funct3==0) ans_mem=extend(pool[address],8); //lb
        else if (funct3==1) ans_mem=extend((pool[address+1]<<8)|pool[address],16); //lh
        else if (funct3==2) ans_mem=(pool[address+3]<<24)+(pool[address+2]<<16)+(pool[address+1]<<8)+pool[address]; //lw
        else if (funct3==4) ans_mem=pool[address]; //lbu
        else if (funct3==5) ans_mem=(pool[address+1]<<8)|pool[address]; //lhu
    }
    if (opcode==35) //store
    {
        if (funct3==0) pool[address]=reg[rs2]&c8; //sb
        else if (funct3==1)  //sh
        {
            int x=reg[rs2]&c16;
            int y=x&c8;
            pool[address+1]=(x-y)>>8;
            pool[address]=y;
        }
        else //sw
        {
            int x=reg[rs2];
            int y=x&c24;
            int z=y&c16;
            int w=z&c8;
            pool[address+3]=(x-y)>>24;
            pool[address+2]=(y-z)>>16;
            pool[address+1]=(z-w)>>8;
            pool[address]=w;
        }
    }
    if (opcode==99||opcode==111||opcode==103||opcode==23) 
        cur_id=pd?address:cur_id;
} 

void WB() 
{
    if (opcode==51||opcode==19||opcode==111||opcode==55||opcode==23||opcode==103) reg[rd]=ans;
    if (opcode==3) reg[rd]=ans_mem;
    reg[0]=0;
}

int main()
{
    freopen("1.in","r",stdin);
    //freopen("1.out","w",stdout);
    while (~scanf("%s",s+1))  
    {
        if (s[1]=='@') cur_id=trans(2);
        else
        {
           // printf("@ %x: ",cur_id);
            s[7]=s[1];s[8]=s[2]; 
            pool[cur_id++]=getnum(s[1],s[2]);
            scanf("%s",w+1);
            s[5]=w[1];s[6]=w[2];
            pool[cur_id++]=getnum(s[5],s[6]);
            scanf("%s",w+1);
            s[3]=w[1];s[4]=w[2];
            pool[cur_id++]=getnum(s[3],s[4]);
            scanf("%s",w+1);
            s[1]=w[1];s[2]=w[2];
            pool[cur_id++]=getnum(s[1],s[2]); 
            s[9]='\0';
            int y=trans(1);
            order[cur_id-4]=y;
        }
    }
    cur_id=0;
    int cnt=0;
    while (1) 
    {
        cnt++;
        IF();
        ID();
        EX();
        MEM();
        WB();
    }
    return 0;
}