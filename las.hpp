#ifndef las_hpp
#define las_hpp
#include<cstdio>
typedef long long LL;

class las //lui,lb,lh,lw,lbu,lhu,sb,sh,sw
{
    private:
         int *id;
         int *t,*p;
         int imm,rs1,rs2,opcode,rd;
    public:
        las(int &cur_id, int *tt, int *pp) : id(&cur_id),t(tt),p(pp) {}
        
        bool judge(unsigned int x) 
        {
            int y=x&c7;
            return y==55||y==3||y==35;
        }

        void execute1()
        {
            if (opcode==0)  //lb
            {
                int x=p[rs1+imm];
                if ((x>>7)&1) 
                {
                    LL tmp=(1<<24)-1;
                    tmp=x|(tmp<<8);
                    x=tmp;
                }
                t[rd]=x;
            }
            else if (opcode==1) //lh
            {
                int x=(p[rs1+imm+1]<<8)+p[rs1+imm];
                if ((x>>15)&1) 
                {
                    LL tmp=(1<<16)-1;
                    tmp=x|(tmp<<16);
                    x=tmp;
                }
                t[rd]=x;
            }
            else if (opcode==2) //lw
                t[rd]=(p[rs1+imm+3]<<24)+(p[rs1+imm+2]<<16)+(p[rs1+imm+1]<<8)+p[rs1+imm];
            else if (opcode==4) //lbu
                t[rd]=p[rs1+imm];
            else if (opcode==5) 
                t[rd]=(p[rs1+imm+1]<<8)+p[rs1+imm];//lhu
        }

        void execute2()
        {
            if (opcode==0) //sb
                p[rs1+imm]=rs2&c8;
            else if (opcode==1) //sh
            {
                int x=rs2&c16;
                int y=x&c8;
                p[rs1+imm+1]=(x-y)>>8;
                p[rs1+imm]=y;
            }
            else //sw
            {
               // printf("sw %d %d %d--\n",rs2,rs1,imm);
                int x=rs2;
                int y=x&c24;
                int z=y&c16;
                int w=z&c8;
                p[rs1+imm+3]=(x-y)>>24;
                p[rs1+imm+2]=(y-z)>>16;
                p[rs1+imm+1]=(z-w)>>8;
                p[rs1+imm]=w;
            }
        }

        void parse(unsigned int x) 
        {
            if ((x&c7)==3) 
            {
                imm=x>>20;
                rd=(x>>7)&c5;
                rs1=(x>>15)&c5;
                opcode=(x>>12)&c3;
                rs1=t[rs1];
                if ((imm>>11)&1) imm+=(c20<<12);
                execute1();
            }
            else if ((x&c7)==55) //lui
            {
                rd=(x>>7)&c5;
                imm=x>>12;
                t[rd]=imm;
                t[rd]=t[rd]<<12;
            }
            else 
            {
                imm=x>>25;
                rd=(x>>7)&c5;
                rs1=(x>>15)&c5;
                rs2=(x>>20)&c5;
                opcode=(x>>12)&c3;
                imm=(imm<<5)+((x>>7)&c5);
                if ((imm>>11)&1) imm+=(c20<<12);
               // if (opcode!=0&&opcode!=1) printf("register: %d %d\n",rs1,rs2);
                rs1=t[rs1];
                rs2=t[rs2];
                execute2();
            }
        }
};
#endif