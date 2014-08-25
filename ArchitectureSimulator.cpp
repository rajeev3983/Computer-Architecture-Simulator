#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
class insfile{
      public:
             int type;
             short int rs;
             short int rt;
             short int rd;
             };
insfile Inst[10000];

class gs{
      public:
             short int B[4096];
           unsigned  int bhr;
           gs(){
                int i;
                for(i=0;i<4096;i++){
                                  B[i]=2;
                                  }
                                  bhr=0;
                                  }
      bool predict(unsigned int pc){
        unsigned   int k=bhr ^ pc;
           k=k<<20;
           k=k>>20;
           if(B[k]==0 || B[k]==1) return 0;
           else return 1;
           }
           
      bool train(unsigned int pc,int bresult){
           
      unsigned     int k=bhr ^ pc;
           k=k<<20;
           k=k>>20;
           bhr=bhr<<1;
           
           bhr=bhr | bresult;
          // cout<<bhr<<"  ";
           bhr=bhr<<26;
           bhr=bhr>>26;
           if(bresult==0){
           B[k]=max(B[k]-1,0);
           }
           else{
           B[k] = min(B[k]+1,3);
           }
           }           
};          
///////////////////////////////////////////
class bm{
      public:
             short int B[1024];
             bm(){
                int i;
                for(i=0;i<1024;i++){
                                  B[i]=2;
                                  }
                                  }
                                  
      bool       predict(unsigned int pc){
                              int k;
                              k=pc%1024;
                              if(B[k]==0 || B[k]==1) return 0;
                              else return 1;
                              }
      void train(unsigned int pc,int bresult){
                              int k;
                              k=pc%1024;
                              if(bresult==0){
                                             B[k]=max(B[k]-1,0);
                                             }
                              else{
                              B[k]=min(B[k]+1,3);
                              }
                              }
      };
 ////////////////////////////////////////////////////////////////////////////////            
class brah{
      public:
      gs gshare;
     int no_of_branch;
     int cpred;
    bool  lastprediction;
    bool lastpredictor;
      bm bimodal;
      short int C[4096];
      brah(){
             int i;
             for(i=0;i<4096;i++){
                                 C[i]=2;
                                 }
             no_of_branch=0;
             cpred=0;
             }
      
  bool    prediction(unsigned int pc){
                 int k=pc%4096;
                 no_of_branch++;
                          if(C[k]==0 || C[k]==1 ) {
                                     lastprediction=gshare.predict(pc); 
                          lastpredictor=0; 
                          }
                          else { lastprediction=bimodal.predict(pc); 
                          lastpredictor=1;
                           }
                          return lastprediction;
                          }                          
void  train(unsigned int pc,int bresult){
                int k=pc%4096;
                 if(lastpredictor==0){
                                      if((lastprediction==0 && bresult==0) ||(lastprediction==1 && bresult==1)){
                                                           C[k]=max(C[k]-1,0);
                                                           gshare.train(pc, bresult);
                                                           bimodal.train(pc, bresult);
                                                           }
                                     else{ 
                                           C[k]=min(C[k]+1,3); 
                                                     gshare.train(pc,bresult);
                                                     bimodal.train(pc, bresult);
                                                     }
                                     }
                 else{
                      if((lastprediction==0 && bresult==0)|| (lastprediction==1 && bresult==1)){
                                                           C[k]=min(C[k]+1,3);
                                                           bimodal.train(pc, bresult);
                                                           gshare.train(pc,bresult);
                                                           }
                                     else{ 
                                           C[k]=max(C[k]-1,0); 
                                                     bimodal.train(pc,bresult);
                                                     gshare.train(pc,bresult);
                                                     }
                                     }
                 }
};
brah branch;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

class c1node{
      public:
       int tag[2];
      };
class tagnode{
      public:
       int tg;
       int t;
      };
/////////////////////////////////////////////////////////////////////      
class c2node{
      public:
      tagnode tag[8];
      };      
////////////////////////////////////////////////////////////////////////////////////
class c1{
      public:
             float no_of_acess;
             int miss;
             float hit;
             c1node A[512];
             c1(){
                  no_of_acess=0;
             int miss=0;
             int hit=0;
             int i;
             for(i=0;i<512;i++){
                                A[i].tag[0]=-1;
                                A[i].tag[1]=-1;
                                }
             }
             
 bool   read(unsigned memadd){
     //   no_of_acess++;
 
            memadd=memadd>>5;
                    int i,k;
                     k=memadd%512;
                    if(A[k].tag[0]==memadd || A[k].tag[1]==memadd){hit++; return 1; }
                    else return 0;
                    }
 void write(unsigned memadd){
      memadd=memadd>>5;
             int i,k;
             k=memadd%512;
             A[k].tag[1]=A[k].tag[0];
             A[k].tag[0]=memadd;
             }
 };
 c1 cache1;
///////////////////////////////////////////////////////////////////////////////////// 
 class c2{
       public:
              float no_of_acess;
              int miss;
              float hit;
              c2node A[2048];              
              c2(){
                  no_of_acess=0;
             int miss=0;
             int hit=0;
             int i,j;
             for(i=0;i<2048;i++){
                                 for(j=0;j<8;j++){
                                                  A[i].tag[j].tg=-1;
                                                  A[i].tag[j].t=0;
                                                  }
                                 
                                                  }
             }             
  bool read(unsigned memadd,long unsigned int tme){
              no_of_acess++;
                memadd=memadd>>7;
                     int i,k;
                     k=memadd%2048;
                    for(i=0;i<8;i++){
                                     if(A[k].tag[i].tg == memadd){ A[k].tag[i].t=tme; 
                                     hit++;
                                     return 1; }
                                     }
                     return 0;
                    }                   
  void write(unsigned memadd, long unsigned tme){
                     int i,k,min,s;
                     memadd=memadd >>7;
                     s=0;
                     min=1000000000;
                     k=memadd%2048;            
                     for(i=0;i<8;i++){
                                      if(A[k].tag[i].t<min){ s=i; min=A[k].tag[i].t; }
                                      }
                     A[k].tag[s].tg=memadd;
                      A[k].tag[s].t=tme;
                     }
  };
 c2 cache2;                                                           
//////////////////////////////////////////////////////////////////////////////////////
void fetch(unsigned int pc,int bresult,long unsigned int &tme){
bool z;
if(Inst[pc].type==3){
                     z=branch.prediction(pc); 
                    if((z==false && bresult==1)|| (z==true && bresult==0)){
                             branch.cpred++;
                             tme=tme+2;
                             }
                    }
                                               
}
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void execute(unsigned int pc,int bresult,long unsigned int &tme,short int pr){
     tme=tme+1;
     if((pr==Inst[pc].rs || pr==Inst[pc].rt) && (Inst[pc].type==3 || Inst[pc].type==2 ) ){ tme=tme+1; }
     if(pr==Inst[pc].rs && (Inst[pc].type==0 || Inst[pc].type==1 )){ tme=tme+1; }
     if(Inst[pc].type!=3){  return; }
    branch.train(pc,bresult);   
}
//////////////////////////////////////////////////////////////////////////////////// 
void memacess(unsigned int pc, unsigned int memadd,long unsigned int &tme){
     if(Inst[pc].type!=0){  return; }
   //   if(pr==Inst[pc].rs){ tme=tme+5;}
     bool z;
     z=cache1.read(memadd);
     cache1.no_of_acess++;
     if(z==1) {  return; }
     else { cache1.write(memadd); }
     z=cache2.read(memadd,tme);
     if(z==1) { tme=tme+8; return; }
      else { cache2.write(memadd,tme); }
     tme=tme + 208;
     }
     
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
int main(const int argc,const char *argv[]){
      int m;
  /*  if(argc < 3 ){
            cout<<"please enter the files on command line"<<endl;
            cin>>m;
            return 0;
            } */
  short int pr;
     int k,a,bresult;
     unsigned int memadd,pc,num_ins=0;
     long unsigned int tme;
     ifstream f1,f2;
     f1.open("2.txt",ios::in);                                           // pc file read
     while(!f1.eof()){
      f1>>hex>>k;
     f1>>dec>>Inst[k].type;
    f1>>dec>>Inst[k].rs;
     f1>>dec>>Inst[k].rt;
     f1>>dec>>Inst[k].rd;
     }
     f1.close();  
       tme=0;
       pc=0;
     f2.open("1.txt",ios::in);                       //instruction file read
      while(!f2.eof()){
                       if(Inst[pc].type==0){
                        pr=Inst[pc].rd;
                        }
                        else pr=-99;
      num_ins++;
      f2>>hex>>pc;
    f2>>hex>>memadd;
     f2>>dec>>bresult;  
/////////////////////////////////////////////////////////////////////////////////////     input done!!     everything here!!
     fetch(pc,bresult,tme);
  //   decode(pc,tme);
     execute(pc,bresult,tme,pr);
     memacess(pc,memadd,tme);
 //    writeback(pc,memadd,tme);
///////////////////////////////////////////////////////////////////////////////////////////////     
     } 
     float a1=tme;
   //  cout<<tme<<endl;
     float a2=num_ins;
    // cout<<a2<<endl;
     cout<<"ipc= "<<(a2)/a1<<endl;
     float l=(cache1.no_of_acess-cache1.hit)/cache1.no_of_acess;
     cout<<"l1 miss rate= "<<(l*100)<<" %"<<endl;
     l=(cache2.no_of_acess-cache2.hit)/cache2.no_of_acess;
     cout<<"l2 miss rate= "<<l*100<<" %"<<endl;
    a1=branch.cpred;
    a2=branch.no_of_branch;
    cout<<"accuracy of branch predictor= "<<((a2-a1)/a2)*100<<" %"<<endl;
     }
