#include<iostream>
#include<cstdio>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "auth.h"
//#include "ecdsa.h"
#define CURRENT_TIME Scheduler::instance().clock()
using namespace std;
int ndid;
void ECDSAData::ecdsasign() {
   char str[]="MYAUTHENTICATIONCODE";
   FILE *fp;
   fp=fopen("wsndata/authmsg.txt","w");
   fprintf(fp,"%s",str);
   printf("\nAuthentication Message is : %s\n",str);
   
   double now2 = CURRENT_TIME; 
   printf("\n Athentication Strat Time : %f",now2); 
   
   keygen();
   h=ecdsahash(str);
   printf("\nHash Value for the Authentication Message is %lu\n",h);
   signgen();
   printf("\nThe Digital Signature for Authentcation is (%lu,%lu)\n\n",r,s); 
   buffer[0]=r;
   buffer[1]=s;
   fclose(fp);
}
void ECDSAData::ecdsaver() {
   char str[128];
   FILE *fp1;
   FILE *fp2;
   fp1=fopen("wsndata/authmsg.txt","r");
   fp2=fopen("wsndata/pk.txt","r");
   fscanf(fp1,"%s",str);
   h=ecdsahash(str);
   printf("\nAuthentication Message is : %s\n",str);
   printf("Hash value is %lu",h);
   fscanf(fp2,"%ld%lld%lld",&p,&g,&y);
   printf("\nPublic Key is (%ld,%lld,%lld)",p,g,y);
   if(r!=0 && s!=0)
   {  
    
     kn=y;
     kn2=0;
     for(kn2=0;kn>=g;kn2++)
     {
      
      kn=kn/g;
      
     } 

      tt1=((h-kn2*r)/k)%RAND_MAX;
      tt2=s-tt1;
      tt3=tt2%p;
         
      printf("\n\n tt3 = %lu t3= %lu",tt3,t3);
      
      

     if(tt3==t3)
      {
	  printf("\nAuthentication Successful\n");
          cf=1;
          FILE *fp5;
          fp5=fopen("wsndata/auval.txt","w");
          fprintf(fp5,"%d",cf);
          fclose(fp5);
          
          /*
          ///////////////////////////////////////////25-03-2022
          double now = CURRENT_TIME;
          FILE *fp7;
          fp7=fopen("wsndata/autime.txt","w");
          fprintf(fp7,"%f",now);
          fclose(fp7);

          printf("\n\n Authentication Time = %f \n",now);
          Scheduler::instance().clock()
          //////////////////////////////////////////
          */
          double now1 = CURRENT_TIME; 
     printf("\n Athentication successfull Time : %f",now1); 
         
      }
      else
      {
	printf("\nAuthentication Failed\n");
      }
   } 
   else
   {
      printf("\nAuthentication Failed\n");
   }
   fclose(fp1);
   fclose(fp2);
}


ECDSAData::ECDSAData()
{
    strcpy(buf,"MYAUTHENTICATIONCODE");
    cout<<"\n ECDSA Authentication Digital Signature Generation \n";
    //ecdsasign();
    cout<<"\n ECDSA Verification \n";
    //ecdsaver();
}

int ECDSAData::size() const
{
    return 8;
}

const char * ECDSAData::getData() const
{
    printf("\nDigital Signature received is %ld & %ld",r,s);
    return buf;
}

static class ECDSAAppClass : public TclClass
{
public:
    ECDSAAppClass() : TclClass("Application/ECDSA") {}
    TclObject* create(int argc, const char*const* argv)
    {
        return (new ECDSA((Node*)TclObject::lookup(argv[4])));
    }
} class_test_app;

ECDSA::ECDSA(Node* node): RealApplication(node)
{
 // MobileNode  *auNode;
 // aunodeid;
    
 }

int ECDSA::command(int argc, const char*const* argv)
{
    if (argc == 3 && strcmp(argv[1], "send") == 0)
    {   
        
        ECDSAData* p = new ECDSAData();
        ECDSA* app = (ECDSA*)TclObject::lookup(argv[2]);
        send(p, app->node_->address());

        printf("\nn%d is Sending Node\n",app->node_->address());
        
        FILE *fp6;
        fp6=fopen("wsndata/ausendlist.txt","w");
        fprintf(fp6,"%d",app->node_->address());
        fclose(fp6);

        ////////////////////
        p->ecdsasign();
        ////////////////////
        

        /////////////////////
	  std::string path1="wsndata/";
	  std::string fname1 = "auval.txt";
	  std::string fname2;
	  std::stringstream sstm1;
	  sstm1 << path1 << fname1;
	  fname2 = sstm1.str();

	  std::fstream myfile1(fname2.c_str());
 	  int mnd , c;

          while (myfile1 >> c)
          {  
            mnd = c ;
           }
          myfile1.close();
         
          printf("\nAuthentication value : %d\n",mnd);
        /////////////////////////

        /////////////////////

       if(mnd == 1)
       {
        std::string path2="wsndata/crl/";
	std::string fname4 = ".txt";
	std::string fname3;
	std::stringstream sstm2;
	sstm2 << path2 << app->node_->address() << fname4;
	fname3 = sstm2.str();
	FILE *fp7;
        
        printf("\nfilename : %s\n",fname3.c_str());

        fp7=fopen(fname3.c_str(),"w");
        fprintf(fp7,"%d",1);
        fclose(fp7);
       }
        /////////////////////////


   


        return TCL_OK;
    }
    return RealApplication::command(argc, argv);
}


void ECDSA::recv(ADU* packet, nsaddr_t src)
{
    ECDSAData* p = (ECDSAData*)packet;
    
    //////////////////////////////////////
    p->ecdsaver();
    //////////////////////////////////////
    cout <<"\n\n"
         << node_->address()
         << " received " << p->size()
         << " bytes from " << src
         << " at " << Scheduler::instance().clock()
         << "\n Authentication Code : " << p->getData()
         << "\n" << endl;

        double now = CURRENT_TIME;  
        std::string path12="wsndata/autime/";
	std::string fname14 = ".txt";
	std::string fname13;
	std::stringstream sstm12;
	sstm12 << path12 << node_->address() << fname14;
	fname13 = sstm12.str();
	FILE *fp17;
        
        printf("\nfilename : %s\n",fname13.c_str());

        fp17=fopen(fname13.c_str(),"w");
        fprintf(fp17,"%f",now);
        fclose(fp17);
          
          //////////////////////////////////////////
         
    delete packet;
}

long int ECDSAData::ecdsahash(char *msg)
{
    long int hk = 7907;
    int ct;
    while(*msg++)
    {    
        ct=*msg++;
        hk=((hk<<5)+hk)+ct;
    }
    return hk;
}

int ECDSAData::check(long int rn)
{
    int n=2;
    while(n<=rn/2)
    {
        if((rn%n)==0)
        {
            return 1;
        }
        n++;
    }
    return 0;
}

long int ECDSAData::primegen()
{
    int rc,ct;
    long int rn=0;
    rn=0;
    while(rn<100)
    {
        rc=rand();
        rn=rn+rc;
    }
    ct=check(rn);
    if(ct==0)
    {
        return rn;
    }
    else
    {
        return primegen();
    }
}

int ECDSAData::gcd(long int a,long int b)
{
  int c;
  while(a!=0)
  {primegen();
     c=a;
     a=b%a;
     b=c;
  }
  return b;
}

void ECDSAData::randomk()
{
   int t;
   k=rand()%4;
   t=gcd(k,p-1);
   if(t!=1)
   {
       randomk();
   }
}

void ECDSAData::randomr()
{
   long long int t,temp;
   r=rand();
   temp=pow(g,k);
   temp=temp%RAND_MAX;
   t=r-temp;
   t=t%p;
}

void ECDSAData::keygen()
{
    FILE *fp;
    fp=fopen("wsndata/pk.txt","w");
    srand(time(NULL));
    p=primegen();
    printf("\nPrime Number used as seed is %lu",p);
    g=561;
    x=rand()%5+1;
        
    y=pow(g,x);
    
    
    //ss=pow(g,y/1.0);
    //printf("\nPrivate Key is:%ld \tPublic Key is:%lld\tPsame value:%lld )",x,y,kn2);
 
    //x = pow (g, 1.0 / y);
    //y=y%p;
    printf("\nPublic Key is (%ld,%lld,%lld)",p,g,y);
    fprintf(fp,"%ld %lld %lld",p,g,y);
    printf("\nPrivate Key is %ld",x);
    fclose(fp);
}

long int ECDSAData::computes()
{
    long int t1,t2;
    t1=((h-x*r)/k)%RAND_MAX;
    s=rand();
    t2=s-t1;
    t3=t2%(p);
    return s;
}

void ECDSAData::signgen()
{
   randomk();
   randomr();
   s=computes();
   if(s==0)
   {
      s=computes();
   }
}


