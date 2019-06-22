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
#include "indilightboxinterface.h"
#include "defaultdevice.h"

namespace Connection
{
    class Serial;
}

class FBC : public INDI::DefaultDevice, public INDI::LightBoxInterface
{
    public:
        FBC();
        virtual ~FBC() = default;

        virtual bool initProperties();
        //virtual void ISGetProperties(const char *dev);
        virtual bool updateProperties();

        virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
        virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n);
        virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n);
        virtual bool ISSnoopDevice(XMLEle *root);

        bool Connect() override;
        bool Disconnect() override;

    protected:
        const char *getDefaultName();
        virtual bool saveConfigItems(FILE *fp);
        //void TimerHit();

        //from light box
        virtual bool SetLightBoxBrightness(uint16_t value);
        virtual bool EnableLightBox(bool enable);


        ITextVectorProperty ActiveDeviceTP;
        IText ActiveDeviceT[2];
        enum
        {
            SNOOP_FILTER,
//            SNOOP_CAPT,    
            SNOOP_SQM
        };

    private:
        bool sendCommand(const char *command, char *response);

        bool getBrightness();
        bool hasLight{ true };

        uint8_t prevLightStatus{ 0xFF };
        uint8_t prevBrightness{ 0xFF };

        //int PortFD{ -1 };
        //Connection::Serial *serialConnection { nullptr };
};
