#ifndef calc_hpp
#define calc_hpp
#include<cstdio>
typedef long long LL;
const int c3=(1<<3)-1;
const int c7=(1<<7)-1;
const int c4=(1<<4)-1;
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

class calc //addi,slti,sltiu,xori,ori,andi,slli,srli,srai,add,sub,sll,slt,sltu,xor,srl,sra,or,and
{
    private:
         int *id;
         int *t,*p;
         int imm,rs1,rs2,opcode,rd;
    public:
        calc(int &cur_id, int *tt, int *pp) : id(&cur_id),t(tt),p(pp) {}
       
        bool judge(unsigned int x) 
        {
            int y=x&c7;
            return y==19||y==51;
        }

        int extend12(int x) 
        {
            if ((x>>11)&1) 
                x=x+(c20<<12);
            return x;
        }

        void execute1(unsigned int b)
        {
            opcode=(b>>12)&c3;
            int immt=extend12(imm);
            if (opcode==0) //addi
                t[rd]=rs1+immt;
            else if (opcode==2) //slti
                t[rd]=rs1<immt?1:t[rd];
            else if (opcode==3) //sltiu
                t[rd]=(unsigned)rs1<(unsigned)immt?1:t[rd];
            else if (opcode==4) //xori
                t[rd]=rs1^immt; 
            else if (opcode==6) //ori
                t[rd]=rs1|immt;
            else if (opcode==7) //andi
                t[rd]=rs1&immt;
            else  
            {
                imm=(b>>20)&c5;
                if (opcode==1) //slli
                    t[rd]=rs1<<imm;
                else if ((b>>30)&1) //srai
                    t[rd]=rs1>>imm;
                else //srli
                    t[rd]=(rs1&(1<<31))?(rs1>>imm)&((1<<(32-imm))-1):rs1>>imm;
            }
        }

        void execute2(unsigned int b) 
        {
            opcode=(b>>12)&c3;
            rs2=(b>>20)&c5;
            rs2=t[rs2];
            if (opcode==1) //sll
                t[rd]=rs1<<(rs2&c5);
            else if (opcode==2) //slt
                t[rd]=rs1<rs2?1:t[rd];
            else if (opcode==3) //sltu
                t[rd]=(unsigned int)rs1<(unsigned int)rs2?1:t[rd];
            else if (opcode==4) //xor
                t[rd]=rs1^rs2;
            else if (opcode==6) //or
                t[rd]=rs1|rs2;
            else if (opcode==7) //and 
                t[rd]=rs1&rs2;
            else if (opcode==0) 
            {
                if ((b>>30)&1) //sub
                    t[rd]=rs1-rs2;
                else //add
                    t[rd]=rs1+rs2;
            }
            else 
            {
                rs2&=c5;
                if ((b>>30)&1) //sra
                    t[rd]=rs1>>rs2;
                else //srl
                    t[rd]=(rs1&(1<<31))?(rs1>>rs2)&((1<<(32-rs2))-1):rs1>>rs2;;
            }
        }

        void parse(unsigned int x) 
        {
            opcode=x&c7;
            rd=(x>>7)&c5;
            rs1=(x>>15)&c5;
            imm=(x>>20)&c12;
            rs1=t[rs1];
            if (opcode==19) execute1(x);
            else execute2(x);     
        }
};
#endif