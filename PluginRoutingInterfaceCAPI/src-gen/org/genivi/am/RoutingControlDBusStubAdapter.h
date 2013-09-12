/*
* This file was generated by the CommonAPI Generators.
*
* This file was generated by the CommonAPI Generators.
*
* This file was generated by the CommonAPI Generators.
*
* Copyright (C) 2012, BMW AG
*
* This file is part of GENIVI Project AudioManager.
*
* Contributions are licensed to the GENIVI Alliance under one or more
* Contribution License Agreements.
*
* \copyright
* This Source Code Form is subject to the terms of the
* Mozilla Public License, v. 2.0. If a  copy of the MPL was not distributed with
* this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*
*
* \author Aleksandar Donchev, aleksander.donchev@partner.bmw.de BMW 2013
* \author Christian Linke christian.linke@bmw.de BMW 2013
*
* For further information see http://www.genivi.org/.
*/
#ifndef ORG_GENIVI_AM_Routing_Control_DBUS_STUB_ADAPTER_H_
#define ORG_GENIVI_AM_Routing_Control_DBUS_STUB_ADAPTER_H_

#include <org/genivi/am/RoutingControlStub.h>

#define COMMONAPI_INTERNAL_COMPILATION

#include <CommonAPI/DBus/DBusStubAdapterHelper.h>
#include <CommonAPI/DBus/DBusFactory.h>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace org {
namespace genivi {
namespace am {

typedef CommonAPI::DBus::DBusStubAdapterHelper<RoutingControlStub> RoutingControlDBusStubAdapterHelper;

class RoutingControlDBusStubAdapter: public RoutingControlStubAdapter, public RoutingControlDBusStubAdapterHelper {
 public:
    RoutingControlDBusStubAdapter(
            const std::string& commonApiAddress,
            const std::string& dbusInterfaceName,
            const std::string& dbusBusName,
            const std::string& dbusObjectPath,
            const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusConnection,
            const std::shared_ptr<CommonAPI::StubBase>& stub);
    


 protected:
    virtual const char* getMethodsDBusIntrospectionXmlData() const;
};

} // namespace am
} // namespace genivi
} // namespace org

#endif // ORG_GENIVI_AM_Routing_Control_DBUS_STUB_ADAPTER_H_