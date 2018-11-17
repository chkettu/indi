#ifndef MGCMD_AG_CANCEL_CALIBRATION_H
#define MGCMD_AG_CANCEL_CALIBRATION_H

/*
 * mgcmd_ag_cancel_calibration.h
 *
 *  Created on: 16 november 2018
 *      Author: chkettu (Christian Liska)
 */

#include "mgc.h"

class MGCMD_AG_CANCEL_CALIBRATION : MGC
{
    public:
        virtual IOByte opCode() const { return 0xca; }
        virtual IOMode opMode() const { return OPM_APPLICATION; }

    public:
        virtual IOResult ask(MGenDevice &root)
        {
            if (CR_SUCCESS != MGC::ask(root))
                return CR_FAILURE;

            if (root.lock())
            {
                root.write(query);
                int const bytes_read = root.read(answer);
                if (answer[0] == query[0] && answer[1] == 0x00)
                {
                    return CR_SUCCESS;
                }
                else
                {
                    _E("no ack (%d bytes read)", bytes_read);

                    if (answer[1] == 0xf1)
                    {
                        _E("can't cancel calibration, an other command is already under execution");
                    }
                    else if (answer[1] == 0xf0)
                    {
                        _E("can't cancel calibration, UI is locked");
                    }
                }
            }

            return CR_FAILURE;
        }

    public:
        MGCMD_AG_CANCEL_CALIBRATION() : MGC(IOBuffer{ opCode(), 0x2c }, IOBuffer(1+1)){}
};


#endif // MGCMD_AG_CANCEL_CALIBRATION_H
