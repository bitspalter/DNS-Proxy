//////////////////////////////////////////////////////////////////////////////////
// [ Net_Interface_Class_Header ]
//////////////////////////////////////////////////////////////////////////////////
//
// Last modi: 01.01.17 L.ey (µ~)
//
#ifndef _C_NET_INTERFACE_H_
 #define _C_NET_INTERFACE_H_

  #include <iostream>
  #include <stdio.h>
  
  using namespace std;

  #include <sys/ioctl.h> 
  #include <net/if.h>
  #include <sys/socket.h> 
  #include <arpa/inet.h> // in_addr Struktur
  #include <unistd.h>
  
  #include "C_DArray.hpp"

//////////////////////////////////////////////////////////////////////////////////

 const int C_NET_INTERFACE_READY = 0x01;
 const int C_NET_INTERFACE_ERROR = 0x00;

//////////////////////////////////////////////////////////////////////////////////

 struct S_Net_Interface {
    char          _ps_Name[16];
    unsigned int  _dw_IP;
    unsigned int  _dw_Netmask;
    unsigned int  _dw_Network;
    unsigned int  _dw_Broadcast;
    char          _ps_IP[16];
    char          _ps_Netmask[16];
    char          _ps_Network[16];
    char          _ps_Broadcast[16];
    char          _ps_MAC[20];
    unsigned char _uc_MAC[6];
    unsigned int  _dw_Flags;
    unsigned char _uc_Active;
    unsigned char _uc_LoopBack;
    unsigned int  _dw_index;
 };

//////////////////////////////////////////////////////////////////////////////////

 class C_Net_Interface {
   
   public:

     C_Net_Interface(){};
    ~C_Net_Interface(){};

     int create();
     int get_First_Active();

     // Inline
     //
     C_DArray* get_pDA_Interface(){return(&DA_Interface);}

   private:

     int fill(char* psDevice, S_Net_Interface* pS_Interface);

     C_DArray DA_Interface;
 };

#endif // _C_NET_INTERFACE_H_


 
