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

/************************************************************************************
 *
************************************************************************************/
FBC::FBC() : LightBoxInterface(this, true)
{
   // constructor
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::initProperties()
{
    INDI::DefaultDevice::initProperties();


    //serialConnection->setDefaultBaudRate(Connection::Serial::B_38400);

    initLightBoxProperties(getDeviceName(), MAIN_CONTROL_TAB);
    //setDriverInterface(AUX_INTERFACE | LIGHTBOX_INTERFACE);

    hasLight = true;

    addDebugControl();
    addPollPeriodControl();
    addAuxControls();

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
        defineText(&ActiveDeviceTP);
        defineSwitch(&LightSP);
        defineNumber(&LightIntensityNP);
        updateLightBoxProperties();
    //    defineNumber(&SnoopedNumbersNP);
    }
    else
    {
        deleteProperty(ActiveDeviceTP.name);
        deleteProperty(LightSP.name);
        deleteProperty(LightIntensityNP.name);
        updateLightBoxProperties();
    //    deleteProperty(SnoopedNumbersNP.name);
    }

    return true;
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    LOG_INFO("ISNewNumber");
    if (processLightBoxNumber(dev, name, values, names, n)) {
        return true;
    }

    return INDI::DefaultDevice::ISNewNumber(dev, name, values, names, n);
}

bool FBC::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    LOG_INFO("ISNewText");

    if (dev != nullptr && strcmp(dev, getDeviceName()) ==0)
    {
        if (processLightBoxText(dev, name, texts, names, n))
        {
            return true;
        }
    }

    return INDI::DefaultDevice::ISNewText(dev, name, texts, names, n);
}


bool FBC::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        if (processLightBoxSwitch(dev, name, states, names, n))
        {
            return true;
        }
    }

    return INDI::DefaultDevice::ISNewSwitch(dev, name, states, names, n);
}

bool FBC::ISSnoopDevice(XMLEle *root)
{
    snoopLightBox(root);
    return DefaultDevice::ISSnoopDevice(root);
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


bool FBC::saveConfigItems(FILE *fp)
{
    INDI::DefaultDevice::saveConfigItems(fp);

    return saveLightBoxConfigItems(fp);
}

/************************************************************************************
 *
************************************************************************************/
bool FBC::EnableLightBox(bool enable)
{
    if (isSimulation())
    {
        return true;
    }

    if (enable)
    {
        //send on
    } else {
        //send off
    }
    return false;
}

bool FBC::SetLightBoxBrightness(uint16_t value)
{
    if (isSimulation())
    {
        LightIntensityN[0].value = value;
        IDSetNumber(&LightIntensityNP, nullptr);
        return true;
    }

    //send brightness

    int brightnessVal = 0;
    //scan value from respone

    if (brightnessVal != prevBrightness)
    {
        prevBrightness = brightnessVal;
        LightIntensityN[0].value = brightnessVal;
        IDSetNumber(&LightIntensityNP, nullptr);
    }

    return true;
}

bool FBC::getBrightness()
{
    if (isSimulation())
    {
        return true;
    }

    //get brightness

    int brightnessVal = 0;
    //scan value from response

    if (brightnessVal != prevBrightness)
    {
        prevBrightness = brightnessVal;
        LightIntensityN[0].value = brightnessVal;
        IDSetNumber(&LightIntensityNP, nullptr);
    }

    return true;
}
