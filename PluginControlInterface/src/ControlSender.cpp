/**
 * Copyright (C) 2011, BMW AG
 *
 * GeniviAudioMananger DbusPlugin
 *
 * \file ControlSender.cpp
 *
 * \date 20-Oct-2011 3:42:04 PM
 * \author Christian Mueller (christian.ei.mueller@bmw.de)
 *
 * \section License
 * GNU Lesser General Public License, version 2.1, with special exception (GENIVI clause)
 * Copyright (C) 2011, BMW AG Christian Mueller  Christian.ei.mueller@bmw.de
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License, version 2.1, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License, version 2.1, for more details.
 * You should have received a copy of the GNU Lesser General Public License, version 2.1, along with this program; if not, see <http://www.gnu.org/licenses/lgpl-2.1.html>.
 * Note that the copyright holders assume that the GNU Lesser General Public License, version 2.1, may also be applicable to programs even in cases in which the program is not a library in the technical sense.
 * Linking AudioManager statically or dynamically with other modules is making a combined work based on AudioManager. You may license such other modules under the GNU Lesser General Public License, version 2.1. If you do not want to license your linked modules under the GNU Lesser General Public License, version 2.1, you may use the program under the following exception.
 * As a special exception, the copyright holders of AudioManager give you permission to combine AudioManager with software programs or libraries that are released under any license unless such a combination is not permitted by the license of such a software program or library. You may copy and distribute such a system following the terms of the GNU Lesser General Public License, version 2.1, including this special exception, for AudioManager and the licenses of the other code concerned.
 * Note that people who make modified versions of AudioManager are not obligated to grant this special exception for their modified versions; it is their choice whether to do so. The GNU Lesser General Public License, version 2.1, gives permission to release a modified version without this exception; this exception also makes it possible to release a modified version which carries forward this exception.
 *
 * THIS CODE HAS BEEN GENERATED BY ENTERPRISE ARCHITECT GENIVI MODEL. PLEASE CHANGE ONLY IN ENTERPRISE ARCHITECT AND GENERATE AGAIN
 */

#include "ControlSender.h"
#include "control/ControlReceiveInterface.h"
#include <cassert>
#include <algorithm>

using namespace am;

extern "C" ControlSendInterface* PluginControlInterfaceFactory()
{
    return (new ControlSenderPlugin());
}

extern "C" void destroyControlPluginInterface(ControlSendInterface* controlSendInterface)
{
    delete controlSendInterface;
}

ControlSenderPlugin::ControlSenderPlugin() :
        mControlReceiveInterface(NULL), //
        mListOpenConnections(), //
        mListOpenDisconnections(), //
        mListOpenVolumeChanges()
{
}

ControlSenderPlugin::~ControlSenderPlugin()
{
}

am_Error_e ControlSenderPlugin::startupController(ControlReceiveInterface *controlreceiveinterface)
{
    assert(controlreceiveinterface);
    mControlReceiveInterface = controlreceiveinterface;
    //here is a good place to insert SystemProperties into the database...
    return E_NOT_USED;
}

am_Error_e ControlSenderPlugin::stopController()
{
    return E_NOT_USED;
}

void ControlSenderPlugin::hookAllPluginsLoaded()
{
    //here is a good place to insert Source and SinkClasses into the database...
    mControlReceiveInterface->setRoutingReady();
    mControlReceiveInterface->setCommandReady();
}

am_Error_e ControlSenderPlugin::hookUserConnectionRequest(const am_sourceID_t sourceID, const am_sinkID_t sinkID, am_mainConnectionID_t & mainConnectionID)
{
    std::vector<am_Route_s> listRoutes;
    std::vector<am_connectionID_t> listConnectionIDs;
    am_Handle_s handle;
    mControlReceiveInterface->getRoute(true, sourceID, sinkID, listRoutes);
    if (listRoutes.empty())
        return E_NOT_POSSIBLE;

    std::vector<handleStatus> listHandleStaus;
    std::vector<am_RoutingElement_s>::iterator it(listRoutes[0].route.begin());
    for (; it != listRoutes[0].route.end(); ++it)
    {
        am_connectionID_t connectionID;
        mControlReceiveInterface->connect(handle, connectionID, it->connectionFormat, it->sourceID, it->sinkID);
        handleStatus status;
        status.handle = handle;
        status.status = false;
        listHandleStaus.push_back(status);
        listConnectionIDs.push_back(connectionID);
    }
    am_MainConnection_s mainConnectionData;
    mainConnectionData.mainConnectionID = 0;
    mainConnectionData.connectionState = CS_CONNECTING;
    mainConnectionData.delay = 0;
    mainConnectionData.listConnectionID = listConnectionIDs;
    mControlReceiveInterface->enterMainConnectionDB(mainConnectionData, mainConnectionID);
    mainConnectionSet set;
    set.connectionID = mainConnectionID;
    set.listHandleStaus = listHandleStaus;
    mListOpenConnections.push_back(set);
    return E_OK;
}

am_Error_e ControlSenderPlugin::hookUserDisconnectionRequest(const am_mainConnectionID_t connectionID)
{
    //first check if there is a connectionID like that
    am_MainConnection_s mainConnection;
    am_Error_e error;
    if ((error = mControlReceiveInterface->getMainConnectionInfoDB(connectionID, mainConnection)) != E_OK)
    {
        return error;
    }

    std::vector<am_connectionID_t>::iterator it(mainConnection.listConnectionID.begin());
    std::vector<handleStatus> listHandleStaus;
    for (; it != mainConnection.listConnectionID.end(); ++it)
    {
        handleStatus status;
        status.status = false;
        mControlReceiveInterface->disconnect(status.handle, *it);
        listHandleStaus.push_back(status);
    }
    mainConnectionSet set;
    set.connectionID = connectionID;
    set.listHandleStaus = listHandleStaus;
    mListOpenConnections.push_back(set);
    return E_OK;
}

am_Error_e ControlSenderPlugin::hookUserSetMainSinkSoundProperty(const am_sinkID_t sinkID, const am_MainSoundProperty_s & soundProperty)
{
    if (sinkID==0) return E_NON_EXISTENT;

    mainSinkSoundPropertySet set;
    set.sinkID = sinkID;
    set.mainSoundProperty = soundProperty;
    am_SoundProperty_s sp;
    //I know this is bad - just for the reference, ok?
    sp.type = static_cast<am_SoundPropertyType_e>(soundProperty.type);
    sp.value = soundProperty.value;
    am_Error_e error;
    if ((error = mControlReceiveInterface->setSinkSoundProperty(set.handle, sinkID, sp)) != E_OK)
    {
        return error;
    }
    mListMainSoundPropertyChanges.push_back(set);
    return E_OK;
}

am_Error_e ControlSenderPlugin::hookUserSetMainSourceSoundProperty(const am_sourceID_t sourceID, const am_MainSoundProperty_s & soundProperty)
{
    (void) sourceID;
    (void) soundProperty;
    return E_NOT_USED;
}

am_Error_e ControlSenderPlugin::hookUserSetSystemProperty(const am_SystemProperty_s & property)
{
    (void) property;
    return E_NOT_USED;
}

am_Error_e ControlSenderPlugin::hookUserVolumeChange(const am_sinkID_t SinkID, const am_mainVolume_t newVolume)
{
    assert(SinkID!=0);
    mainVolumeSet set;
    set.sinkID = SinkID;
    set.mainVolume = newVolume;
    am_Error_e error;
    if ((error = mControlReceiveInterface->setSinkVolume(set.handle, SinkID, newVolume, RAMP_DIRECT, 20)) != E_OK)
    {
        return error;
    }
    mListOpenVolumeChanges.push_back(set);
    return E_OK;
}

am_Error_e ControlSenderPlugin::hookUserVolumeStep(const am_sinkID_t SinkID, const int16_t increment)
{
    assert(SinkID!=0);
    mainVolumeSet set;
    set.sinkID = SinkID;
    am_Error_e error;
    am_Sink_s sink;
    mControlReceiveInterface->getSinkInfoDB(SinkID, sink);
    set.mainVolume = sink.volume + increment;
    if ((error = mControlReceiveInterface->setSinkVolume(set.handle, SinkID, set.mainVolume, RAMP_DIRECT, 20)) != E_OK)
    {
        return error;
    }
    mListOpenVolumeChanges.push_back(set);
    return E_OK;
}

am_Error_e ControlSenderPlugin::hookUserSetSinkMuteState(const am_sinkID_t sinkID, const am_MuteState_e muteState)
{
    (void) sinkID;
    (void) muteState;
    return E_NOT_USED;
}

am_Error_e ControlSenderPlugin::hookSystemRegisterDomain(const am_Domain_s & domainData, am_domainID_t & domainID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->enterDomainDB(domainData, domainID);
}

am_Error_e ControlSenderPlugin::hookSystemDeregisterDomain(const am_domainID_t domainID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->removeDomainDB(domainID);
}

void ControlSenderPlugin::hookSystemDomainRegistrationComplete(const am_domainID_t domainID)
{
    (void) domainID;
}

am_Error_e ControlSenderPlugin::hookSystemRegisterSink(const am_Sink_s & sinkData, am_sinkID_t & sinkID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->enterSinkDB(sinkData, sinkID);
}

am_Error_e ControlSenderPlugin::hookSystemDeregisterSink(const am_sinkID_t sinkID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->removeSinkDB(sinkID);
}

am_Error_e ControlSenderPlugin::hookSystemRegisterSource(const am_Source_s & sourceData, am_sourceID_t & sourceID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->enterSourceDB(sourceData, sourceID);
}

am_Error_e ControlSenderPlugin::hookSystemDeregisterSource(const am_sourceID_t sourceID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->removeSourceDB(sourceID);
}

am_Error_e ControlSenderPlugin::hookSystemRegisterGateway(const am_Gateway_s & gatewayData, am_gatewayID_t & gatewayID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->enterGatewayDB(gatewayData, gatewayID);
}

am_Error_e ControlSenderPlugin::hookSystemDeregisterGateway(const am_gatewayID_t gatewayID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->removeGatewayDB(gatewayID);
}

am_Error_e ControlSenderPlugin::hookSystemRegisterCrossfader(const am_Crossfader_s & crossfaderData, am_crossfaderID_t & crossfaderID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->enterCrossfaderDB(crossfaderData, crossfaderID);
}

am_Error_e ControlSenderPlugin::hookSystemDeregisterCrossfader(const am_crossfaderID_t crossfaderID)
{
    //this application does not do anything with it -> but some product might want to take influence here
    return mControlReceiveInterface->removeCrossfaderDB(crossfaderID);
}

void ControlSenderPlugin::hookSystemSinkVolumeTick(const am_Handle_s handle, const am_sinkID_t sinkID, const am_volume_t volume)
{
    (void) handle;
    (void) sinkID;
    (void) volume;
}

void ControlSenderPlugin::hookSystemSourceVolumeTick(const am_Handle_s handle, const am_sourceID_t sourceID, const am_volume_t volume)
{
    (void) handle;
    (void) sourceID;
    (void) volume;
}

void ControlSenderPlugin::hookSystemInterruptStateChange(const am_sourceID_t sourceID, const am_InterruptState_e interruptState)
{
    (void) sourceID;
    (void) interruptState;
}

void ControlSenderPlugin::hookSystemSinkAvailablityStateChange(const am_sinkID_t sinkID, const am_Availability_s & availability)
{
    (void) sinkID;
    (void) availability;
}

void ControlSenderPlugin::hookSystemSourceAvailablityStateChange(const am_sourceID_t sourceID, const am_Availability_s & availability)
{
    (void) sourceID;
    (void) availability;
}

void ControlSenderPlugin::hookSystemDomainStateChange(const am_domainID_t domainID, const am_DomainState_e state)
{
    (void) domainID;
    (void) state;
}

void ControlSenderPlugin::hookSystemReceiveEarlyData(const std::vector<am_EarlyData_s> & data)
{
    (void) data;
}

void ControlSenderPlugin::hookSystemSpeedChange(const am_speed_t speed)
{
    (void) speed;
}

void ControlSenderPlugin::hookSystemTimingInformationChanged(const am_mainConnectionID_t mainConnectionID, const am_timeSync_t time)
{
    (void) mainConnectionID;
    (void) time;
}

void ControlSenderPlugin::cbAckConnect(const am_Handle_s handle, const am_Error_e errorID)
{
    (void) errorID;
    //\todo:error checking
    std::vector<mainConnectionSet>::iterator it(mListOpenConnections.begin());
    for (; it != mListOpenConnections.end(); ++it)
    {
        std::vector<handleStatus>::iterator hit;
        handleStatus status;
        status.status = true;
        status.handle = handle;
        hit = std::find_if(it->listHandleStaus.begin(), it->listHandleStaus.end(), findHandle(status));
        if (hit == it->listHandleStaus.end())
            continue;
        hit->status = true;
        if (it->listHandleStaus.end() == std::find_if(it->listHandleStaus.begin(), it->listHandleStaus.end(), checkHandle(status)))
        {
            mControlReceiveInterface->changeMainConnectionStateDB(it->connectionID, CS_CONNECTED);
            mListOpenConnections.erase(it);
            break;
        }
    }
}

void ControlSenderPlugin::cbAckDisconnect(const am_Handle_s handle, const am_Error_e errorID)
{
    (void) errorID;
    //\todo:error checking
    std::vector<mainConnectionSet>::iterator it(mListOpenDisconnections.begin());
    for (; it != mListOpenDisconnections.end(); ++it)
    {
        std::vector<handleStatus>::iterator hit;
        handleStatus status;
        status.status = true;
        status.handle = handle;
        hit = std::find_if(it->listHandleStaus.begin(), it->listHandleStaus.end(), findHandle(status));
        if (hit == it->listHandleStaus.end())
            continue;
        hit->status = true;
        if (it->listHandleStaus.end() == std::find_if(it->listHandleStaus.begin(), it->listHandleStaus.end(), checkHandle(status)))
        {
            mControlReceiveInterface->removeMainConnectionDB(it->connectionID);
            mListOpenDisconnections.erase(it);
            break;
        }
    }
}

void ControlSenderPlugin::cbAckCrossFade(const am_Handle_s handle, const am_HotSink_e hostsink, const am_Error_e error)
{
    (void) handle;
    (void) hostsink;
    (void) error;
}

void ControlSenderPlugin::cbAckSetSinkVolumeChange(const am_Handle_s handle, const am_volume_t volume, const am_Error_e error)
{
    (void) error;
    (void) volume;
    //\todo:error checking
    std::vector<mainVolumeSet>::iterator it(mListOpenVolumeChanges.begin());
    for (; it != mListOpenVolumeChanges.end(); ++it)
    {
        if (handle.handle == it->handle.handle)
        {
            mControlReceiveInterface->changeSinkMainVolumeDB(it->mainVolume, it->sinkID);
        }
    }
}

void ControlSenderPlugin::cbAckSetSourceVolumeChange(const am_Handle_s handle, const am_volume_t voulme, const am_Error_e error)
{
    (void) error;
    (void) voulme;
    (void) handle;
}

void ControlSenderPlugin::cbAckSetSourceState(const am_Handle_s handle, const am_Error_e error)
{
    (void) error;
    (void) handle;
}

void ControlSenderPlugin::cbAckSetSourceSoundProperty(const am_Handle_s handle, const am_Error_e error)
{
    (void) error;
    (void) handle;
}

void ControlSenderPlugin::cbAckSetSinkSoundProperty(const am_Handle_s handle, const am_Error_e error)
{
    (void) error;
    //\todo:error checking
    std::vector<mainSinkSoundPropertySet>::iterator it(mListMainSoundPropertyChanges.begin());
    for (; it != mListMainSoundPropertyChanges.end(); ++it)
    {
        if (handle.handle == it->handle.handle)
        {
            mControlReceiveInterface->changeMainSinkSoundPropertyDB(it->mainSoundProperty, it->sinkID);
        }
    }
}

void ControlSenderPlugin::cbAckSetSourceSoundProperties(const am_Handle_s handle, const am_Error_e error)
{
    (void) error;
    (void) handle;
}

void ControlSenderPlugin::cbAckSetSinkSoundProperties(const am_Handle_s handle, const am_Error_e error)
{
    (void) error;
    (void) handle;
}

am_Error_e ControlSenderPlugin::getConnectionFormatChoice(const am_sourceID_t sourceID, const am_sinkID_t sinkID, const std::vector<am_ConnectionFormat_e> listPossibleConnectionFormats, std::vector<am_ConnectionFormat_e>& listPrioConnectionFormats)
{
    (void) sourceID;
    (void) sinkID;
    //ok, this is cheap. In a real product you have your preferences, right?
    listPrioConnectionFormats = listPossibleConnectionFormats;
    return (E_OK);
}

uint16_t ControlSenderPlugin::getInterfaceVersion() const
{
    return (ControlSendVersion);
}

