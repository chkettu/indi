#ifndef MGCMD_GET_LAST_FRAME_H
#define MGCMD_GET_LAST_FRAME_H

/*
 * mgcmd_get_last_frame.h
 *
 *  Created on: 29 october 2018
 *      Author: chkettu (Christian Liska)
 */

#include "mgc.h"

class MGCMD_GET_LAST_FRAME : MGC
{
    public:
        virtual IOByte opCode() const { return 0x9d; }
        virtual IOMode opMode() const { return OPM_APPLICATION; }

    public:
        unsigned short frame_num() const { return (unsigned short)answer[1]; }
        bool star_present() const { return ((answer[0] >> 6) & 1) == 1; }

    public:
        virtual IOResult ask(MGenDevice &root)
        {
            if (CR_SUCCESS != MGC::ask(root))
                return CR_FAILURE;

            if (root.lock())
            {
                root.write(query);
                int const bytes_read = root.read(answer);
                if (answer[0] == query[0] && (1 == bytes_read || 5 == bytes_read))
                {
                    return CR_SUCCESS;
                }

                _E("no ack (%d bytes read)", bytes_read);
            }

            return CR_FAILURE;
        }

    public:
        MGCMD_GET_LAST_FRAME() : MGC(IOBuffer{ opCode(), 1 }, IOBuffer(1+1+4)){}
};

#endif // MGCMD_GET_LAST_FRAME_H
