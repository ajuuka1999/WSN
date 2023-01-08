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

#include "real-app.h"
#include "simulator.h"

using namespace std;

map<nsaddr_t, RealApplication*> RealApplication::apps_;

RealApplication::RealApplication(Node* node): node_(node)
{
    // add to the address-application map
    apps_[node_->address()] = this;
}

RealApplication::~RealApplication()
{
    // delete all connections
    map<nsaddr_t, Wrapper*>::iterator it;
    for (it = connections_.begin(); it != connections_.end(); it++)
    {
        delete it->second;
    }
}

void RealApplication::send(ADU* packet, nsaddr_t dst)
{
    // if already connected, send the packet
    map<nsaddr_t, Wrapper*>::iterator it = connections_.find(dst);
    if (connections_.end() != it)
    {
        it->second->send(packet);
        return;
    }

    // if not connected

    Tcl& tcl = Tcl::instance();
    
    // create local TCP agent
    tcl.eval("new Agent/TCP/FullTcp/Newreno");
    Agent* localAgent = (Agent*)TclObject::lookup(tcl.result());

    // attach local agent to node
    tcl.evalf("%s attach %s", node_->name(), localAgent->name());

    // create remote TCP agent
    tcl.eval("new Agent/TCP/FullTcp/Newreno");
    Agent* remoteAgent = (Agent*)TclObject::lookup(tcl.result());

    // get remote RealApplication
    // attach remote agent to node
    map<nsaddr_t, RealApplication*>::iterator iter = apps_.find(dst);
    RealApplication* remoteApplication = iter->second;
    tcl.evalf("%s attach %s",
              remoteApplication->node_->name(),
              remoteAgent->name());

    // let the remote agent start listening
    tcl.evalf("%s listen", remoteAgent->name());

    // connect agents
    tcl.evalf("%s connect %s %s",
              Simulator::instance().name(),
              localAgent->name(),
              remoteAgent->name());

    // create local application wrapper
    Wrapper* localWrapper = new Wrapper(this, localAgent);
    localAgent->attachApp(localWrapper);

    // create remote application wrapper
    Wrapper* remoteWrapper = new Wrapper(remoteApplication, remoteAgent);
    remoteAgent->attachApp(remoteWrapper);

    // set peer of the application wrapper
    localWrapper->setPeer(remoteWrapper);
    remoteWrapper->setPeer(localWrapper);

    // step 3: put the application to the connection list
    connections_[dst] = localWrapper;
    remoteApplication->setConnection(remoteWrapper);

    // 2) send the packet
    localWrapper->send(packet);
}

void RealApplication::setConnection(Wrapper* peerWrapper)
{
    connections_[peerWrapper->getPeerAddress()] = peerWrapper;
}

Node* RealApplication::getNode() const
{
    return node_;
}

