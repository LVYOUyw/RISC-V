#include<cstdio>
#include<cstring>
#include<iostream>
#include"calc.hpp"
#include"las.hpp"
#include"jump.hpp"
using namespace std;
int pool[200005];
unsigned int order[200005];
char s[1005],w[1005];
int cur_id,reg[32];

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

int main()
{
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
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
            //printf("@%x: %x\n",cur_id-4,y);
        }
    }
    calc A(cur_id,reg,pool);
    las B(cur_id,reg,pool);
    jump C(cur_id,reg,pool);
    cur_id=0;
    int cnt=0;
    while (1) 
    {
        int ord=order[cur_id];//if (cnt==31855) printf("%d\n",ord);
        if (ord==0x00c68223) 
        {
            printf("%d\n",(unsigned int)reg[10]&255u);
            break;
        }
        if (A.judge(ord)) {A.parse(ord);}
        else if (B.judge(ord)) {B.parse(ord);}
        else if (C.judge(ord)) {C.parse(ord);}
        else 
        {
            printf("%x\n",ord);
            printf("%x\n",cur_id);
            printf("%d\n",cnt);
            break;
        }
       printf("%x\n",ord);
        reg[0]=0;
        cur_id+=4;
        cnt++;
       // if (cnt>=50000000) break;
    }
    return 0;
}