#ifndef MGCMD_AG_STOP_H
#define MGCMD_AG_STOP_H

/*
 * mgcmd_ag_stop.h
 *
 *  Created on: 16 november 2018
 *      Author: chkettu (Christian Liska)
 */

#include "mgc.h"

class MGCMD_AG_STOP : MGC
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
                _E("no ack (%d bytes read)", bytes_read);
            }

            return CR_FAILURE;
        }

    public:
        MGCMD_AG_STOP() : MGC(IOBuffer{ opCode(), 0x01 }, IOBuffer(1+1)){}
};
#endif // MGCMD_AG_STOP_H
