/*******************************************************************************
 Copyright(c) 2019 Christian Liska. All rights reserved.

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

#pragma once

#include "defaultdevice.h"

class FBC : public INDI::DefaultDevice
{
    public:
        FBC();
        virtual bool initProperties();
        virtual bool updateProperties();
        virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
        virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n);
        virtual bool ISSnoopDevice(XMLEle *root);
        bool Connect() override;
        bool Disconnect() override;

    protected:
        const char *getDefaultName();

        ITextVectorProperty ActiveDeviceTP;
        IText ActiveDeviceT[1];
        enum
        {
            SNOOP_SQM
        };

    private:
        INumberVectorProperty SnoopedNumbersNP;
        INumber SnoopedNumbersN[1];

        // Sky Quality
        double MPSAS;

        //Connection::Serial *serialConnection { nullptr };
};
