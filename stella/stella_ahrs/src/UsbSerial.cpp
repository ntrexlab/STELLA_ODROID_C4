#include <stdio.h>
#include <pthread.h>

#include "UsbSerial.hpp"
#include "SerialTypedf.hpp"
#include "UsbPacket.hpp"

#include <unistd.h>

UsbSerial::UsbSerial()
{
    _bus = new UsbPacket();

    _stopReqThreadTransmit = true;
    _stopReqThreadReceive = false;

    ThreadStart();
}

UsbSerial::~UsbSerial()
{
    _stopReqThreadTransmit = true;
    _stopReqThreadReceive = true;

    ThreadStop();

    delete _bus;
}

void UsbSerial::ThreadStart()
{
    pthread_create(&ThreadTransmit_, NULL, &UsbSerial::ThreadTransmitStatic,this);
    pthread_create(&ThreadReceive_ ,NULL,  &UsbSerial::ThreadReceiveStatic,this);
}

void UsbSerial::ThreadStop()
{
    pthread_join(ThreadTransmit_, NULL);
    pthread_join(ThreadReceive_, NULL);
}

void *UsbSerial::ThreadTransmitStatic(LPVOID pParam)
{
    return ((UsbSerial *)pParam) -> ThreadTransmit();
}

void *UsbSerial::ThreadReceiveStatic(LPVOID pParam)
{
    return ((UsbSerial *)pParam)->ThreadReceive();
}

void *UsbSerial::ThreadTransmit() 
{
    while (!_stopReqThreadTransmit)
    {

    }

    return 0;
}

void *UsbSerial::ThreadReceive()
{
    while (!_stopReqThreadReceive)
    {
        Usb2Packet pkt;

        if(_bus->RecvPacket(pkt))
        {
            _lockReceive.Lock();
            _queueReceive.push_back(pkt);         
            _lockReceive.Unlock(); 

    /*        
        if(pkt.msg.data[1] == 0x35)
        {
            int16_t z = ((int)(unsigned char)pkt.msg.data[6]) | ((int)(unsigned char)pkt.msg.data[7] << 8);
            printf("%f\n",z/100.0);
        }
    */ 
        }
    }

    return 0;
}

bool UsbSerial::ReceiveSerialMessage(SerialMessage &msg)
{
    if(_queueReceive.size() > 0)
    {
        sleep(0.00001);
       //_lockReceive.Lock();
       Usb2Packet &pkt = _queueReceive.front();
       msg = pkt.msg;
        _queueReceive.pop_front();
       //_lockReceive.Unlock();

        /*
        if(msg.data[1] == 0x35)
        {
            int16_t z = ((int)(unsigned char)msg.data[6]) | ((int)(unsigned char)msg.data[7] << 8);
            printf("%f\n",z/100.0);
        }
        */
        
     
        return true;
    }
   return false;
}


