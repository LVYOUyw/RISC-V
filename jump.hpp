#ifndef jump_hpp
#define jump_hpp
#include<cstdio>
typedef long long LL;


class jump //auipc,jal,jalr,beq,bne,blt,bge,bltu,bgeu
{
    private:
         int *id;
         int *t,*p;
         int imm,rs1,rs2,opcode,rd,imm2;
    public:
        jump(int &cur_id, int *tt, int *pp) : id(&cur_id),t(tt),p(pp) {}
        
        bool judge(unsigned int x) 
        {
            opcode=x&c7;
            return opcode==99||opcode==103||opcode==111||opcode==23;
        }

        void execute1() 
        {
          //  puts("W");
            if (opcode==0)  //beq
                (*id)=t[rs1]==t[rs2]?(*id)+imm2-4:(*id);
            else if (opcode==1) //bne
                (*id)=t[rs1]==t[rs2]?(*id):(*id)+imm2-4;
            else if (opcode==4) //blt
                (*id)=t[rs1]<t[rs2]?(*id)+imm2-4:(*id);
            else if (opcode==5) //bge
                (*id)=t[rs1]>=t[rs2]?(*id)+imm2-4:(*id);
            else if (opcode==6) //bltu
                (*id)=(unsigned int)t[rs1]<(unsigned int)t[rs2]?(*id)+imm2-4:(*id);
            else if (opcode==7) //bgeu
                (*id)=(unsigned int)t[rs1]>=(unsigned int)t[rs2]?(*id)+imm2-4:(*id);
        }

        void parse(unsigned int x) 
        {
            int y=x&c7;
            if (y==99) 
            {
                //printf("---%x---\n",x);
                rs1=(x>>15)&c5;
                rs2=(x>>20)&c5;
                imm=(x>>8)&c4;
                imm=(((x>>25)&c6)<<4)+imm;
                int t1=(x>>7)&1;
                int t2=(x>>31)&1;
                t1=t1<<10;
                t2=t2<<11;
                imm=imm+t1+t2;
                imm=imm<<1;
                imm2=imm;
                if ((imm>>12)&1) 
                    imm2+=(c19<<13);
                opcode=(x>>12)&c3;
                execute1(); 
            }
            else if (y==23) //auipc
            {
                rd=(x>>7)&c5;
                imm=(x>>12)&c20;
                imm<<=12;
                (*id)=(*id)+imm;
                t[rd]=(*id);
                (*id)-=4;
            }
            else if (y==111) //jal
            {
                imm=(x>>21)&c10;
                imm+=(((x>>12)&c8)<<11);
                imm+=(((x>>20)&1)<<10);
                imm+=(((x>>31)&1)<<19);
                imm<<=1;
                if ((imm>>20)&1) 
                    imm+=(c11<<21); 
               // printf("*jal* %x\n",imm);
                rd=(x>>7)&c5;
                t[rd]=(*id)+4;
                (*id)=(*id)+imm;
                (*id)-=4;
            }
            else //jalr
            {
                rs1=(x>>15)&c5;
                rd=(x>>7)&c5;
                imm=(x>>20)&c12; 
                if ((imm>>11)&1) 
                    imm+=(c20<<12);
                t[rd]=(*id)+4;
                (*id)=t[rs1]+imm;
                (*id)-=4;
            }
        } 
};
#endif