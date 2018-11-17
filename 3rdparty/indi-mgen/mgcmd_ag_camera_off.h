#ifndef MGCMD_AG_CAMERA_OFF_H
#define MGCMD_AG_CAMERA_OFF_H

class MGCMD_AG_CAMERA_OFF : MGC
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
                        _E("can't stop camera, an other command is already under execution");
                    }
                    else if (answer[1] == 0xf0)
                    {
                        _E("can't stop camera, UI is locked");
                    }
                }
            }

            return CR_FAILURE;
        }

    public:
        MGCMD_AG_CAMERA_OFF() : MGC(IOBuffer{ opCode(), 0xc0 }, IOBuffer(1+1)){}
};


#endif // MGCMD_AG_CAMERA_OFF_H
