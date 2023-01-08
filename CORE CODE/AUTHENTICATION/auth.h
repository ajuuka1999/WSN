
#include "adu.h"
#include "real-app.h"
#include<string.h>
#include<time.h>
#include<stdlib.h>


class ECDSAData: public ADU
{
public:
    unsigned long int h,x,k,r,s;
    unsigned long long int g,y,kn,kn2;;
    unsigned long int p;
    long int t1,t2,t3,tt1,tt2,tt3;
    int cf;
    //int aunodeid;// Added for authenticated node
    //MobileNode  *auNode;
    
    void ecdsasign();
    void ecdsaver();
    long int ecdsahash(char *);
    int check(long int);
    long int primegen();
    int gcd(long int, long int);
    void randomk();
    void randomr();
    void keygen();
    long int computes();
    void signgen();
    
    
    ECDSAData();

    virtual int size() const;

    const char * getData() const;

private:
    long int buffer[2];
    char buf[21];
};

class ECDSA: public RealApplication
{
public:
    ECDSA(Node* node);

    virtual void recv(ADU* packet, nsaddr_t src);

protected:
    virtual int command(int argc, const char*const* argv);
};

