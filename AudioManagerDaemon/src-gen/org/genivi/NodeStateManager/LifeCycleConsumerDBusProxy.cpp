/*
* This file was generated by the CommonAPI Generators.
*
 * Copyright (C) 2013, BMW AG
 *
 * \author Christian Linke, christian.linke@bmw.de BMW 2012,2013
*/
#include "LifeCycleConsumerDBusProxy.h"

namespace org {
namespace genivi {
namespace NodeStateManager {

std::shared_ptr<CommonAPI::DBus::DBusProxy> createLifeCycleConsumerDBusProxy(
                    const std::string& commonApiAddress,
                    const std::string& interfaceName,
                    const std::string& busName,
                    const std::string& objectPath,
                    const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusProxyConnection) {
    return std::make_shared<LifeCycleConsumerDBusProxy>(commonApiAddress, interfaceName, busName, objectPath, dbusProxyConnection);
}

__attribute__((constructor)) void registerLifeCycleConsumerDBusProxy(void) {
    CommonAPI::DBus::DBusFactory::registerProxyFactoryMethod(LifeCycleConsumer::getInterfaceId(),
       &createLifeCycleConsumerDBusProxy);
}

LifeCycleConsumerDBusProxy::LifeCycleConsumerDBusProxy(
                    const std::string& commonApiAddress,
                    const std::string& interfaceName,
                    const std::string& busName,
                    const std::string& objectPath,
                    const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection>& dbusProxyconnection):
        CommonAPI::DBus::DBusProxy(commonApiAddress, interfaceName, busName, objectPath, dbusProxyconnection)
 {
}



void LifeCycleConsumerDBusProxy::LifecycleRequest(const uint32_t& Request, const uint32_t& RequestId, CommonAPI::CallStatus& callStatus, int32_t& ErrorCode) {
    CommonAPI::DBus::DBusProxyHelper<CommonAPI::DBus::DBusSerializableArguments<uint32_t, uint32_t>,
                                     CommonAPI::DBus::DBusSerializableArguments<int32_t> >::callMethodWithReply(
        *this,
        "LifecycleRequest",
        "uu",
        Request, RequestId, 
        callStatus
        , ErrorCode);
}
std::future<CommonAPI::CallStatus> LifeCycleConsumerDBusProxy::LifecycleRequestAsync(const uint32_t& Request, const uint32_t& RequestId, LifecycleRequestAsyncCallback callback) {
    return CommonAPI::DBus::DBusProxyHelper<CommonAPI::DBus::DBusSerializableArguments<uint32_t, uint32_t>,
                                     CommonAPI::DBus::DBusSerializableArguments<int32_t> >::callMethodAsync(
        *this,
        "LifecycleRequest",
        "uu",
        Request, RequestId, 
        std::move(callback));
}

void LifeCycleConsumerDBusProxy::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {
    ownVersionMajor = 1;
    ownVersionMinor = 0;
}

} // namespace NodeStateManager
} // namespace genivi
} // namespace org