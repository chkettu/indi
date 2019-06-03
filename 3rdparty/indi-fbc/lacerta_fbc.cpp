/*******************************************************************************
 Copyright(c) 2019 Christian Liska. All rights reserved.

 Implementation based on Lacerta MFOC driver
 (written 2018 by Franck Le Rhun and Christian Liska).

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License version 2 as published by the Free Software Foundation.
 .
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.
 .
 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
*******************************************************************************/

#include "lacerta_fbc.h"
#include "indicom.h"
#include "connectionplugins/connectionserial.h"

#include <cmath>
#include <memory>
#include <cstring>
#include <unistd.h>

// We declare an auto pointer to FBC.
static std::unique_ptr<FBC> fbc(new FBC());

void ISGetProperties(const char *dev)
{
    fbc->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    fbc->ISNewSwitch(dev, name, states, names, n);
}

void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    fbc->ISNewText(dev, name, texts, names, n);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    fbc->ISNewNumber(dev, name, values, names, n);
}

void ISSnoopDevice(XMLEle *root)
{
    fbc->ISSnoopDevice(root);
}

void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[],
               char *names[], int n)
{
    INDI_UNUSED(dev);
    INDI_UNUSED(name);
    INDI_UNUSED(sizes);
    INDI_UNUSED(blobsizes);
    INDI_UNUSED(blobs);
    INDI_UNUSED(formats);
    INDI_UNUSED(names);
    INDI_UNUSED(n);
}

bool FBC::ISSnoopDevice(XMLEle *root)
{
    XMLEle * ep           = nullptr;
    const char * propName = findXMLAttValu(root, "name");

    if (!strcmp(propName, "SKY_QUALITY"))
    {
        for (ep = nextXMLEle(root, 1); ep != nullptr; ep = nextXMLEle(root, 0))
        {
            const char * name = findXMLAttValu(ep, "name");

            if (!strcmp(name, "SKY_BRIGHTNESS"))
            {
                MPSAS = atof(pcdataXMLEle(ep));
                LOGF_DEBUG("recvd SQM: %f", MPSAS);
                break;
            }
        }
    } else if (!strcmp(propName, "FILTER_SLOT_VALUE"))
    {
        for (ep = nextXMLEle(root, 1); ep != nullptr; ep = nextXMLEle(root, 0))
        {
            const char * name = findXMLAttValu(ep, "name");

            if (!strcmp(name, "FILTER_SLOT"))
            {
                SLOT = atoi(pcdataXMLEle(ep));
                LOGF_DEBUG("recvd SLOT: %d", SLOT);
                break;
            }
        }        
    } else if (!strcmp(propName, "CCD_EXPOSURE_VALUE"))
    {
        for (ep = nextXMLEle(root, 1); eq != nullptr; ep = nextXMLEle(root, 0))
        {
            const char * name = findXMLAttValue(ep, "name");

            if (!strcmp(name, "CCD_EXPOSURE"))
            {
                EXPOSURE_TIME = atoi(pcdataXMLEle(ep));
                LOGF_DEBUG("recvd EXPOSURE_TIME: %d", EXPOSURE_TIME);
                break;
            }
        }
    }

    return DefaultDevice::ISSnoopDevice(root);
}

/************************************************************************************
 *
************************************************************************************/
FBC::FBC()
{
   // constructor
}

bool FBC::Connect()
{
    return true;
}

bool FBC::Disconnect()
{
    return true;
}
/************************************************************************************
 *
************************************************************************************/
const char *FBC::getDefaultName()
{
    return "Lacerta FBC";
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::initProperties()
{
    INDI::DefaultDevice::initProperties();

    // Default driver name SQM - could be Astromechanics LPM too
    IUFillText(&ActiveDeviceT[SNOOP_SQM], "ACTIVE_SKYQUALITY", "Sky Quality", "Astromech LPM");
    // TODO
    IUFillText(&ActiveDeviceT[SNOOP_FILTER], "ACTIVE_FILTERWHEEL", "Slot Position", "TODO");
    IUFillTextVector(&ActiveDeviceTP, ActiveDeviceT, 4, getDeviceName(), "ACTIVE_DEVICES", "Snoop devices", OPTIONS_TAB,
                     IP_RW, 60, IPS_IDLE);

    IDSnoopDevice(ActiveDeviceT[SNOOP_SQM].text, "SKY_QUALITY");
    IDSnoopDevice(ActiveDeviceT[SNOOP_FILTER].text, "FILTER_SLOT_VALUE");

    //serialConnection->setDefaultBaudRate(Connection::Serial::B_38400);

    addDebugControl();
    addPollPeriodControl();

    return true;
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::updateProperties()
{
    INDI::DefaultDevice::updateProperties();

    if (isConnected())
    {
    //    defineNumber(&SnoopedNumbersNP);
    }
    else
    {
    //    deleteProperty(SnoopedNumbersNP.name);
    }

    return true;
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {/*
        if (strcmp(name, "LENS_APP_SETTING") == 0) {
            AppertureNP.s = IPS_OK;
            IUUpdateNumber(&AppertureNP, values, names, n);

            IDSetNumber(&AppertureNP, nullptr);
            SetApperture(AppertureN[0].value);

            return true;
        }
        */
    }

    return INDI::DefaultDevice::ISNewNumber(dev, name, values, names, n);
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    LOG_INFO("ISNewText");
    if (dev != nullptr && strcmp(dev, getDeviceName()) ==0)
    {
        if (!strcmp(name, ActiveDeviceTP.name)) {
            ActiveDeviceTP.s = IPS_OK;
            IUUpdateText(&ActiveDeviceTP, texts, names, n);
            IDSetText(&ActiveDeviceTP, nullptr);
            // TODO
        }
    }

    return INDI::DefaultDevice::ISNewText(dev, name, texts, names, n);
}
