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

#include <iostream>
#include "test.h"

using namespace std;

TestData::TestData()
{
    memcpy(this->buffer, "hello, world!", 13);
}

int TestData::size() const
{
    return 100;
}

const char* TestData::getData() const
{
    return buffer;
}

// hook class that the Test can be created in OTcl
static class TestAppClass : public TclClass
{
public:
    TestAppClass() : TclClass("Application/Test") {}
    TclObject* create(int argc, const char*const* argv)
    {
        return (new Test((Node*)TclObject::lookup(argv[4])));
    }
} class_test_app;

Test::Test(Node* node): RealApplication(node)
{
}

int Test::command(int argc, const char*const* argv)
{
    if (argc == 3 && strcmp(argv[1], "send") == 0)
    {
        // the packet is created and delete by the application itself
        TestData* p = new TestData();
        RealApplication* app = (RealApplication*)TclObject::lookup(argv[2]);
        send(p, app->getNode()->address());
        return TCL_OK;
    }
    return RealApplication::command(argc, argv);
}

void Test::recv(ADU* packet, nsaddr_t src)
{
    TestData* p = (TestData*)packet;
    cout << node_->address()
         << " received " << p->size()
         << " from " << src
         << " at " << Scheduler::instance().clock()
         << "\n message: " << p->getData()
         << "\n" << endl;
    delete packet;
}

