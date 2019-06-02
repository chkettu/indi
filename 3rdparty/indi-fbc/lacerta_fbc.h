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

#include "indifocuser.h"

class FBC : public INDI::DefaultDevice
{
    public:
        FBC();
        bool initProperties() override;
        bool updateProperties() override;
        const char *getDefaultName() override;
        virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
        virtual bool initProperties();
        virtual bool updateProperties();

    protected:
        bool Connect();
        bool Disconnect();
        const char *getDefaultName();

    private:
        INumberVectorProperty SnoopedNumbersNP;
        INumber SnoopedNumbersN[1];
};
