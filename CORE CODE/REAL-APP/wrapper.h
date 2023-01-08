/*
   Copyright (C) 2009 Xizhi Zhu.  All Rights Reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __real_app_wrapper_h__
#define __real_app_wrapper_h__

#include <list>
#include "adu.h"
#include "app.h"
#include "real-app.h"

class RealApplication;

// the wrapper of the application
class Wrapper: public Application
{
public:
    // constructor & destructor
    Wrapper(RealApplication* app, Agent* agent);
    ~Wrapper();

    // called when new data to be sent
    void send(ADU* packet);

    // called when new data received
    void recv(int nbytes);

    // set the peer
    void setPeer(Wrapper* peer);

    // get address
    nsaddr_t getLocalAddress() const;
    nsaddr_t getPeerAddress() const;

private:
    // pointer to real application for callback
    RealApplication* app_;

    // peer
    Wrapper* peer_;

    // packets to be sent
    std::list<ADU*> packets_;

    // length of data received and in the buffer
    int currentLength_;
};

#endif // __real_app_wrapper_h__

