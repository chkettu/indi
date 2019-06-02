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

bool FBC::Disconnect() {
    return true;
}

void ISSnoopDevice(XMLEle *root)
{
    fbc->ISSnoopDevice(root);
}

/************************************************************************************
 *
************************************************************************************/
FBC::FBC()
{
   setVersion(0,1);
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

    serialConnection->setDefaultBaudRate(Connection::Serial::B_38400);

    addDebugControl();
    return true;
}

/************************************************************************************
 *
************************************************************************************/
bool astromechanics_foc::updateProperties()
{
    // Get Initial Position before we define it in the INDI::Focuser class
    FocusAbsPosN[0].value = GetAbsFocuserPosition();

    INDI::DefaultDevice::updateProperties();

    if (isConnected())
    {
        //defineNumber(&AppertureNP);
    }
    else
    {
        //deleteProperty(AppertureNP.name);
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
