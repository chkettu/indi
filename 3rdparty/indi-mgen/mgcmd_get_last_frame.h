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
        virtual IOByte opMode() const { return OPM_APPLICATION; }

    public:
        unsigned short frame_num() const { return (unsigned short)answer[1]; }
        bool star_present() const { return true; } //TODO

    public:
        virtual IOResult ask(MGenDevice &root)
        {
            if (CR_SUCCESS != MGC::ask(root))
                return CR_FAILURE;

            if (root.lock())
            {
                query.resize(2);
                query[1] = 0x01; //read drift value
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
        MGCMD_GET_LAST_FRAME() : MGC(IOBuffer{ opCode() }, IOBuffer(1+1+4)){}
}
#endif // MGCMD_GET_LAST_FRAME_H
