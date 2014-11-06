/** @file
    @brief Implementation

    @date 2014

    @author
    Ryan Pavlik
    <ryan@sensics.com>
    <http://sensics.com>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
#include <ogvr/Connection/DeviceToken.h>
#include "AsyncDeviceToken.h"
#include "SyncDeviceToken.h"
#include <ogvr/Connection/Connection.h>
#include "ConnectionDevice.h"

// Library/third-party includes
// - none

// Standard includes
#include <stdexcept>

namespace ogvr {
DeviceTokenPtr DeviceToken::createAsyncDevice(std::string const &name,
                                              ConnectionPtr const &conn) {
    DeviceTokenPtr ret(new AsyncDeviceToken(name));
    ret->m_sharedInit(conn);
    return ret;
}

DeviceTokenPtr DeviceToken::createSyncDevice(std::string const &name,
                                             ConnectionPtr const &conn) {
    DeviceTokenPtr ret(new SyncDeviceToken(name));
    ret->m_sharedInit(conn);
    return ret;
}

DeviceToken::DeviceToken(std::string const &name) : m_name(name) {}

DeviceToken::~DeviceToken() {}

std::string const &DeviceToken::getName() const { return m_name; }

void DeviceToken::sendData(MessageType *type, const char *bytestream,
                           size_t len) {
    time::TimeValue tv;
    time::getNow(tv);
    m_sendData(tv, type, bytestream, len);
}
void DeviceToken::sendData(time::TimeValue const &timestamp, MessageType *type,
                           const char *bytestream, size_t len) {
    m_sendData(timestamp, type, bytestream, len);
}

void DeviceToken::setAsyncWaitCallback(AsyncDeviceWaitCallback const &cb) {
    AsyncDeviceToken *dev = this->asAsync();
    if (!dev) {
        throw std::logic_error(
            "Called setAsyncWaitCallback on a non-async device token!");
    }
    dev->setWaitCallback(cb);
}

void DeviceToken::setSyncUpdateCallback(SyncDeviceUpdateCallback const &cb) {
    SyncDeviceToken *dev = this->asSync();
    if (!dev) {
        throw std::logic_error(
            "Called setSyncUpdateCallback on a non-sync device token!");
    }
    dev->setUpdateCallback(cb);
}

void DeviceToken::connectionInteract() { m_connectionInteract(); }

ConnectionPtr DeviceToken::m_getConnection() { return m_conn; }

ConnectionDevicePtr DeviceToken::m_getConnectionDevice() { return m_dev; }

AsyncDeviceToken *DeviceToken::asAsync() { return NULL; }
SyncDeviceToken *DeviceToken::asSync() { return NULL; }

void DeviceToken::m_sharedInit(ConnectionPtr const &conn) {
    m_conn = conn;
    m_dev = conn->registerDevice(m_name);
    m_dev->setDeviceToken(*this);
}

} // end of namespace ogvr
