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

#include "wrapper.h"

using namespace std;

Wrapper::Wrapper(RealApplication* app, Agent* agent)
    : app_(app), peer_(NULL), currentLength_(0)
{
    agent_ = agent;
    enableRecv_ = 1;
}

Wrapper::~Wrapper()
{
    // delete all the packet in the list
    list<ADU*>::iterator it = packets_.begin();
    for(; it != packets_.end(); it++)
    {
        delete (*it);
    }

    // delete agent and its peer
    delete agent_;
    delete peer_;
}

void Wrapper::send(ADU* packet)
{
    packets_.push_back(packet);

    Application::send(packet->size());
}

void Wrapper::recv(int nbytes)
{
    currentLength_ += nbytes;

    ADU* p = NULL;
    int size = 0;

    // handle the received bytes
    while (currentLength_ > 0)
    {
        p = peer_->packets_.front();
        size = p->size();

        // if less than one message, do nothing
        if (currentLength_ < size)
        {
            return;
        }

        // if equal or more than one message, receive the message
        app_->recv(p, peer_->agent_->addr());
        peer_->packets_.pop_front();
        currentLength_ -= size;
    }
}

void Wrapper::setPeer(Wrapper* peer)
{
    peer_ = peer;
}

nsaddr_t Wrapper::getLocalAddress() const
{
    return agent_->addr();
}

nsaddr_t Wrapper::getPeerAddress() const
{
    return agent_->daddr();
}

