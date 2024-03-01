// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.portmap.reg;

import cmajor.portmap.message;

namespace cmajor::portmap {

void Register()
{
    HelloPmsRequest::Register();
    HelloPmsReply::Register();
    GetFreePortNumberRequest::Register();
    GetFreePortNumberReply::Register();
    ExtendPortLeaseRequest::Register();
    ExtendPortLeaseReply::Register();
    StopPortMapServerRequest::Register();
    StopPortMapServerReply::Register();
    PortLease::Register();
    ViewPortLeaseRequest::Register();
    ViewPortLeaseReply::Register();
}

namespace {

    struct Reg
    {
        Reg()
        {
            Register();
        }
    };

    Reg reg;
}

} // namespace cmajor::portmap
