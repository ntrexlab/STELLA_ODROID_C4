#include "UsbPacket.hpp"

#define Meaning 5

UsbPacket::UsbPacket()
{
    _bus = new SerialCOM();
}

UsbPacket::~UsbPacket()
{
    _bus->Close();
    delete _bus;
}

int UsbPacket::FindMove(char *packet, int length, char stx)
{
    for (int i = 0; i < length; ++i)
    {
        if (packet[i] == stx)
        {
            memmove(&packet[0], &packet[i],length-i);
            return length - i;
        }
    }
    return 0;
}

int UsbPacket::_RecvPacket(char *packet, int length)
{
    int recv_count = 0;

    for (int i = 0; i < 5 && 0 < length; i++)
    {
        int no_read = _bus->__Read(packet);

        if(no_read == 0) return 0;

        if (recv_count == 0 && packet[0] != STX)
        {
            no_read = FindMove(packet, no_read, STX);
        }
        length -= no_read;
        recv_count += no_read;
    }
    
       for(int i=0; i<13; i++)
        {
            printf("%02x ",packet[i]);
        }
        printf("\n");

    return recv_count;
}
bool UsbPacket::RecvPacket(Usb2Packet &pkt)
{
    int recv_count = 0;

    char packet[BIN_PACKET_LEN] = {
        0,
    };

    int ret = _RecvPacket(packet, BIN_PACKET_LEN);
  
    if (ret < 0)
    {
        //printf("[ERROR] Can't read packet\n");
        return false;
    }
    else if (ret == 0)
    {
        return false;
    }
    else if (ret != BIN_PACKET_LEN)
    {
        //printf("[ERROR] Packet length error: %d != %d\n", ret, BIN_PACKET_LEN);
        return false;
    }

    else if (packet[0] != STX || packet[13 - 1] != ETX)
    {
        //printf("[ERROR] STX, ETX error\n");
        return false;
    }

    else if (packet[11] != checkSum(&packet[0], 2, 10))
    {
        //printf("[ERROR] packet checksum error\n");
        return false;
    }

    SerialMessage &msg = pkt.msg;

    msg.ID =packet[2];
    msg.length = packet[1];

    memcpy(msg.data, &packet[3],10);
    
    /*
    if(packet[4] == 0x35)
    {
        int16_t z = ((int)(unsigned char)packet[9]) | ((int)(unsigned char)packet[10] << 8);
        printf("%f\n",z/100.0);
    }
*/
    for(int i=3; i<=10; i++)
    {
        printf("%02X ",packet[i]);
    }
    printf("\n");
   

    return true;
}

bool UsbPacket::SendPacket(void)
{
    return false;
}

char UsbPacket::checkSum(char *packet, int start_length, int end_length)
{
    char cs = 0;

    for (int i = start_length; i < end_length + 1; i++)
    {
        cs += packet[i];
    }

    return cs;
}
