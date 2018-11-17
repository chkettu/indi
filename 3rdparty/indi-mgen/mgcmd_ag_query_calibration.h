#ifndef MGCMD_AG_QUERY_CALIBRATION_H
#define MGCMD_AG_QUERY_CALIBRATION_H

/*
 * mgcmd_ag_query_calibration.h
 *
 *  Created on: 16 november 2018
 *      Author: chkettu (Christian Liska)
 */

#include "mgc.h"

class MGCMD_AG_QUERY_CALIBRATION : MGC
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
                    if (answer[1] == 0xf0)
                    {
                        _E("can't do function, UI locked");
                        return CR_FAILURE;
                    }
                    else if (answer[1] == 0x00)
                    {
                        //calib_state
                        if (answer[2] == 0x00)
                        {
                            _E("calibration: not yet started");
                            return CR_SUCCESS;
                        }
                        else if (answer[2] == 0x01)
                        {
                            _E("calibration: measuring start position");
                            return CR_SUCCESS;
                        }
                        else if (answer[2] == 0x02)
                        {
                            _E("calibration: moving DEC, eliminating backlash");
                            return CR_SUCCESS;
                        }
                        else if (answer[2] == 0x03)
                        {
                            _E("calibration: measuring / moving DEC");
                            return CR_SUCCESS;
                        }
                        else if (answer[2] == 0x04)
                        {
                            _E("calibration: measuring / moving RA");
                            return CR_SUCCESS;
                        }
                        else if (answer[2] == 0x05)
                        {
                            _E("calibration: almost done, moving DEC back to original position");
                            return CR_SUCCESS;
                        }
                        else if (answer[2] == 0xff)
                        {
                            _E("calibration has ended");
                            if (answer[3] == 0x00)
                            {
                                _E("calibration result: no error, success");
                                return CR_SUCCESS;
                            }
                            else if (answer[3] == 0x01)
                            {
                                _E("calibration result: user canceled calibration");
                            }
                            else if (answer[3] == 0x02)
                            {
                                _E("calibration result: star has been lost (or wasn't present");
                            }
                            else if (answer[3] == 0x04)
                            {
                                _E("calibration result: fatal position error detected");
                            }
                            else if (answer[3] == 0x05)
                            {
                                _E("calibration result: orientation error detected");
                            }
                        }
                    }
                }
            }

            return CR_FAILURE;
        }

    public:
        MGCMD_AG_QUERY_CALIBRATION() : MGC(IOBuffer{ opCode(), 0x29 }, IOBuffer(1+1+2)){}
};
#endif // MGCMD_AG_QUERY_CALIBRATION_H
