#ifndef _C_NET_RAW_PROTO_H_
  #define _C_NET_RAW_PROTO_H_ 

//////////////////////////////////////////////////////////////////////////////////
//  | Netzwerk Protokolle |    |            |    |            |
//////////////////////////////////////////////////////////////////////////////////
 //
 //  last Modi: 03.01.17 [µ~]
 //
 //
//////////////////////////////////////////////////////////////////////////////////
 // [ INTERN_HEADER ] ( for Packet Engine )
//////////////////////////////////////////////////////////////////////////////////
 //

 typedef unsigned char  UCHAR;
 typedef unsigned short WORD;
 typedef unsigned int   DWORD;
 
 const UCHAR SIZE_MAC  = 0x06;
 const UCHAR SIZE_IP   = 0x04;
 const UCHAR SIZE_PORT = 0x02;

 const UCHAR IP_ZERO[SIZE_IP]        = { 0x00, 0x00, 0x00, 0x00 };
 const UCHAR IP_BROADCAST[SIZE_IP]   = { 0xFF, 0xFF, 0xFF, 0xFF };
 const UCHAR IP_SUBMASK[SIZE_IP]     = { 0xFF, 0xFF, 0xFF, 0x00 };

 const UCHAR MAC_ZERO[SIZE_MAC]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
 const UCHAR MAC_BROADCAST[SIZE_MAC] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

 struct S_ArpPacket{
    WORD   Operation;
    UCHAR* pETH_MAC_D;
    UCHAR* pETH_MAC_S;
    UCHAR* pARP_MAC_D;
    UCHAR* pARP_MAC_S;
    UCHAR* pARP_IP_D;
    UCHAR* pARP_IP_S;
 };

 struct S_IpPacket{
    UCHAR* pETH_MAC_D;
    UCHAR* pETH_MAC_S;
    UCHAR* pIP_D;
    UCHAR* pIP_S;
    UCHAR  pIP_Type;
    WORD   pIP_ID; 
    WORD   pIP_Frag;
    UCHAR  pIP_TTL;
    UCHAR  pIP_Protocol;      
 };

 struct S_IcmpPacket{
    UCHAR ICMP_Type;
    UCHAR ICMP_Code;
    WORD  ICMP_ID;          
    WORD  ICMP_Sequence;    
    DWORD ICMP_TimeStamp;    
 };

 struct S_UdpPacket{
    WORD  UDP_Port_D;
    WORD  UDP_Port_S;
 };

 struct S_TcpPacket{
    WORD  TCP_Port_D;
    WORD  TCP_Port_S;
    UCHAR TCP_Flag;
    DWORD TCP_nSeq;
    DWORD TCP_nAck;
 };
 //
//////////////////////////////////////////////////////////////////////////////////
 // [ REAL_HEADER ]
//////////////////////////////////////////////////////////////////////////////////
 //
 //
 //////////////////////////////////////////////////////////////////////////////////
 //--------------------------[ PHYSICAL LAYER ]------------------------------0x00//
 //////////////////////////////////////////////////////////////////////////////////
 //
 //////////////////////////////////////////////////////////////////////////////////
 //--------------------------[ DATA LINK LAYER ]-----------------------------0x01//
 //////////////////////////////////////////////////////////////////////////////////
 //
 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ ETHERNET_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 struct ETHERNET_HEADER{
    UCHAR   MAC_D[SIZE_MAC];  
    UCHAR   MAC_S[SIZE_MAC];      
    WORD    Type;               
 };

 const DWORD cETHERNET_HEADER = sizeof(ETHERNET_HEADER);

 ///////////////////////////////////////////////////
 // ETHERNET_HEADER CONSTANT
 //
 const WORD ETH_TYP_IPv4 = 0x0008;
 const WORD ETH_TYP_IPv6 = 0xDD86; 
 const WORD ETH_TYP_ARP  = 0x0608; 
 const WORD ETH_TYP_RARP = 0x3580; 

/*
 Typfeld	Protokoll
0x0800	IP Internet Protocol (IPv4)
0x86DD	Internet Protocol, Version 6 (IPv6)
0x0806	Address Resolution Protocol (ARP)
0x8035	Reverse Address Resolution Protocol (RARP)

0x809B	Appletalk (Ethertalk)
0x80F3	Appletalk Address Resolution Protocol (AARP)
0x8100	VLAN Tag (VLAN)
0x8137	Novell IPX (alt)
0x8138	Novell
*/

 //
 //////////////////////////////////////////////////////////////////////////////////
 //---------------------------[ NETWORK LAYER ]------------------------------0x02//
 //////////////////////////////////////////////////////////////////////////////////
 //
 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ ARP_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 typedef struct ARP_HEADER{
    WORD    ARP_HType;           // Hardware Format
    WORD    ARP_PType;           // Protocol Format
    UCHAR   ARP_cHAddr;          // Hardware Address Length
    UCHAR   ARP_cPAddr;          // Protocol Address Length
    WORD    ARP_OpCode;          // Opcode
    UCHAR   ARP_MAC_S[SIZE_MAC]; // Sender Ethernet Address
    UCHAR   ARP_IP_S[SIZE_IP];   // Sender IP Address
    UCHAR   ARP_MAC_D[SIZE_MAC]; // Target Ethernet Address
    UCHAR   ARP_IP_D[SIZE_IP];   // Target IP Address
 }  ARP_HEADER;

 const DWORD cARP_HEADER = sizeof(ARP_HEADER);

 ///////////////////////////////////////////////////
 // ARP_HEADER CONSTANT
 //
 const WORD  ARP_HTYP_ETH  = 0x0100;   // Hardware Format [ ethernet ]
 const WORD  ARP_PTYP_IP   = 0x0008;   // Protocol Format [ ip ]

 const WORD  ARP_REQUEST   = 0x0100;   // Opcode
 const WORD  ARP_RESPONSE  = 0x0200;   // Opcode

 const UCHAR ARP_MAC_LEN   = SIZE_MAC; // Hardware Address Length [ ethernet ]
 const UCHAR ARP_PRO_LEN   = SIZE_IP;  // Protocol Address Length [ ip ]

 
 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ IP_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 struct IP_HEADER{
    UCHAR  IP_VerLen;    // version(4Bit) & length(4Bit)
    UCHAR  IP_ToS;       // Type of service(8Bit)
    WORD   IP_cPacket;   // Total length of the packet(16Bit)
    WORD   IP_ID;        // Unique identifier(16Bit)
    WORD   IP_Flags;     // Flags(3Bit) & Frag offset(13Bit)
    UCHAR  IP_Ttl;       // Time to live(8Bit)
    UCHAR  IP_Protocol;  // Protocol(8Bit) (TCP, UDP etc)
    WORD   IP_Checksum;  // IP checksum(16Bit)
    UCHAR  IP_IP_S[SIZE_IP];   // Source IP(32Bit)
    UCHAR  IP_IP_D[SIZE_IP];   // Destination IP(32Bit)
    //unsigned char  options[40];  // Optional bytes(40 byte max)
 };

 const DWORD cIP_HEADER = sizeof(IP_HEADER);

 ///////////////////////////////////////////////////
 // IP_HEADER CONSTANT
 //
 const UCHAR IP_ICMP = 0x01;  // proto
 const UCHAR IP_IP   = 0x04;  // proto
 const UCHAR IP_TCP  = 0x06;  // proto
 const UCHAR IP_UDP  = 0x11;  // proto

 const UCHAR IP_VERSION = 64;
 const UCHAR IP_DEFAULT = IP_VERSION | (cIP_HEADER / 4);

 
 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ ICMP_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 struct ICMP_HEADER{
    UCHAR ICMP_Type;        // ICMP Message Type
    UCHAR ICMP_Code;        // Message Interner (Fehler-)Code 
    WORD  ICMP_CheckSum;    // Checksum für das ICMP Paket 
    WORD  ICMP_ID;          // ID des Pakets, oft wie Port bei TCP / UDP genutzt 
    WORD  ICMP_Sequence;    // Sequenznummer, bei mehreren typgleichen, (sinn-)zusammenhängenden Paketen 
    //DWORD   ICMP_TimeStamp;   // Zeitstempel beim Abesenden 
 };   

 const DWORD cICMP_HEADER = sizeof(ICMP_HEADER);

 ///////////////////////////////////////////////////
 // ICMP_HEADER CONSTANT
 //
 const UCHAR ICMP_TYPE_RESPONSE  = 0x00;
 const UCHAR ICMP_TYPE_ERROR     = 0x03;
 const UCHAR ICMP_TYPE_REQUEST   = 0x08;
 const UCHAR ICMP_TYPE_TIME      = 0x0B;

 const UCHAR ICMP_ERROR_NET      = 0x00;
 const UCHAR ICMP_ERROR_HOST     = 0x01;
 const UCHAR ICMP_ERROR_PROTOCOL = 0x02;
 const UCHAR ICMP_ERROR_PORT     = 0x03; 

 //
 //////////////////////////////////////////////////////////////////////////////////
 //-------------------------[ TRANSPORT LAYER ]------------------------------0x03//
 //////////////////////////////////////////////////////////////////////////////////
 //
 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ UDP_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 struct UDP_HEADER{
    WORD UDP_Port_S;         // [00] - source port            
    WORD UDP_Port_D;         // [02] - destination port       
    WORD UDP_cPacket;        // [04] - Packet length + message length         
    WORD UDP_Checksum;       // [06] - checksum           
 };

 const DWORD cUDP_HEADER = sizeof(UDP_HEADER);

 ///////////////////////////////////////////////////
 // UDP_PSEUDO_HEADER
 //
 struct UDP_PSEUDO_HEADER{
    UCHAR      UDP_PS_IP_S[SIZE_IP];    
    UCHAR      UDP_PS_IP_D[SIZE_IP];     
    UCHAR      UDP_PS_Empty;  
    UCHAR      UDP_PS_Protocol;
    WORD       UDP_PS_Size;  
    UDP_HEADER UDP_PS_UDP_HEADER;
 };

 const DWORD cUDP_PS_HEADER = sizeof(UDP_PSEUDO_HEADER);

 ///////////////////////////////////////////////////
 // UDP_HEADER DEFAULT PORTS
 //
 const WORD UDP_PORT_NS_DNS    = 0x3500; //  DNS  net Short 
 const WORD UDP_PORT_NS_DHCP_S = 0x4300; //  DHCP Server net Short
 const WORD UDP_PORT_NS_DHCP_C = 0x4400; //  DHCP Client net Short
 const WORD UDP_PORT_NS_NBS137 = 0x8900; //  137
 const WORD UDP_PORT_NS_NBS138 = 0x8A00; //  138
 const WORD UDP_PORT_NS_NBS2   = 0xBD01; //  445
 const WORD UDP_PORT_NS_IPSEC1 = 0xF401; //  500
 const WORD UDP_PORT_NS_IPSEC2 = 0x9411; // 4500

 const WORD UDP_PORT_HS_DNS    = 0x0035; //  DNS win host Short
 const WORD UDP_PORT_HS_DHCP_S = 0x0043; //  DHCP Server host Short
 const WORD UDP_PORT_HS_DHCP_C = 0x0044; //  DHCP Client host Short
 const WORD UDP_PORT_HS_NBS137 = 0x0089; //  137
 const WORD UDP_PORT_HS_NBS138 = 0x008A; //  138
 const WORD UDP_PORT_HS_NBS2   = 0x01BD; //  445
 const WORD UDP_PORT_HS_IPSEC1 = 0x01F4; //  500
 const WORD UDP_PORT_HS_IPSEC2 = 0x1194; // 4500


 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ TCP_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 struct TCP_HEADER{
    WORD   TCP_Port_S;     // Source Port
    WORD   TCP_Port_D;     // Destination Port
    DWORD  TCP_nSeq;       // Sequence Nummer
    DWORD  TCP_nAck;       // Acknowledged Nummer
    UCHAR  TCP_cHead;      // Data offset(4bit) & Reserved(4bit)(bNULL)
    UCHAR  TCP_FLAG;       // Flag
    WORD   TCP_cWin;       // Windows Size
    WORD   TCP_Checksum;   // Checksum
    WORD   TCP_pUrgent;    // Urgent pointer
    //
    //UCHAR TCP_Options[n * 32]; //optional Data [ (TCP_cHead & 0xF0) - cTCP_HEADER ]
 };                     

 const DWORD cTCP_HEADER = sizeof(TCP_HEADER);

 ///////////////////////////////////////////////////
 // TCP_HEADER CONSTANT
 //
 const UCHAR TCP_FLAG_FIN = 0x01; //flags
 const UCHAR TCP_FLAG_SYN = 0x02; 
 const UCHAR TCP_FLAG_RST = 0x04; 
 const UCHAR TCP_FLAG_PSH = 0x08;
 const UCHAR TCP_FLAG_ACK = 0x10;
 const UCHAR TCP_FLAG_URG = 0x20;
 const UCHAR TCP_FLAG_ECE = 0x40;
 const UCHAR TCP_FLAG_CWR = 0x80; //flags

 //const UCHAR TCP_FLAG_FIN_ACK = 0x11;
 //const UCHAR TCP_FLAG_SYN_ACK = 0x12;
 //const UCHAR TCP_FLAG_RST_ACK = 0x14;
 //const UCHAR TCP_FLAG_PSH_ACK = 0x18;

 //const UCHAR TCP_FLAG_FIN_PSH_ACK = 0x19;

 ///////////////////////////////////////////////////
 // TCP_PSEUDO_HEADER
 //
 struct TCP_PSEUDO_HEADER{
    UCHAR      TCP_PS_IP_S[SIZE_IP];    
    UCHAR      TCP_PS_IP_D[SIZE_IP];     
    UCHAR      TCP_PS_Empty;  
    UCHAR      TCP_PS_Protocol;
    WORD       TCP_PS_Size;  
    TCP_HEADER TCP_PS_TCP_HEADER;
 };

 const DWORD cTCP_PSEUDO_HEADER = sizeof(TCP_PSEUDO_HEADER);

 ///////////////////////////////////////////////////
 // TCP_HEADER DEFAULT PORTS
 //
 const WORD TCP_PORT_NS_FTP   = 0x1400; //net Short 
 const WORD TCP_PORT_NS_CTP   = 0x1500; 
 const WORD TCP_PORT_NS_HTTP  = 0x5000; 
 const WORD TCP_PORT_NS_HTTPS = 0xBB01; 
 const WORD TCP_PORT_NS_POP3  = 0x6E00;
 const WORD TCP_PORT_NS_NBS1  = 0x8B00; // 139
 const WORD TCP_PORT_NS_NBS2  = 0xBD01; // 445
 const WORD TCP_PORT_NS_DCE   = 0x8700; // 135

 const WORD TCP_PORT_HS_FTP   = 0x0014; //win host Short 
 const WORD TCP_PORT_HS_CTP   = 0x0015;
 const WORD TCP_PORT_HS_HTTP  = 0x0050; 
 const WORD TCP_PORT_HS_HTTPS = 0x01BB; 
 const WORD TCP_PORT_HS_POP3  = 0x006E;
 const WORD TCP_PORT_HS_NBS1  = 0x008B; // 139
 const WORD TCP_PORT_HS_NBS2  = 0x01BD; // 445
 const WORD TCP_PORT_HS_DCE   = 0x0087; // 135

 //
 //////////////////////////////////////////////////////////////////////////////////
 //-------------------------[ APPLICATION LAYER ]----------------------------0x04//
 //////////////////////////////////////////////////////////////////////////////////
 //
 //////////////////////////////////////////////////////////////////////////////////
 // ++ [ DNS_HEADER ]
 //////////////////////////////////////////////////////////////////////////////////
 //
 struct DNS_HEADER{
    WORD DNS_ID;
    WORD DNS_Flags;
    WORD DNS_cQuestion;
    WORD DNS_cAnswer;
    WORD DNS_cAuthority;
    WORD DNS_cAdditional;
 };

 struct DNS_QUEST{
    // UCHAR DNS_cName;  // example: ([WWW].BMW.COM) = 0x03; ([geocities].com) = 0x09;
    // char  DNS_Name[DNS_cName];
    WORD DNS_Type;
    WORD DNS_Class;
 };

 struct DNS_ANSWER{
    WORD  DNS_pNAME;  // ( Offset(DNS_pName) | 0xC000 );
    WORD  DNS_Type;
    WORD  DNS_Class;
    DWORD DNS_TTL;
    WORD  DNS_cData;
    //UCHAR DNS_DATA[DNS_cData]; // IP OR Name String
 } __attribute__ ((packed));

 ///////////////////////////////////////////////////
 // DNS_HEADER CONSTANT
 //
 // DNS_Flags 
 // WORD Type:1, OpCode:4, AAnswer:1, TrunCation:1, RecD:1, RecA:1, NotUse:3, RCode:4;
 //
 const WORD DNS_FLAG_TYP_REQUEST     = 0x0000;   // Request
 const WORD DNS_FLAG_TYP_RESPONSE    = 0x8000;   // Response

 const WORD DNS_FLAG_OPCODE_SQUERY   = 0x0000;   // Standart Query
 const WORD DNS_FLAG_OPCODE_IQUERY   = 0x0800;   // Inverse Query
 const WORD DNS_FLAG_OPCODE_SERVER   = 0x2000;   // Server Status Request

 const WORD DNS_FLAG_BIT_AA          = 0x0040;   // Authoritative Answer
 const WORD DNS_FLAG_BIT_TC          = 0x0080;   // TrunCation
 const WORD DNS_FLAG_BIT_RD          = 0x0100;   // Recursion Desired
 const WORD DNS_FLAG_BIT_RA          = 0x0200;   // Recursion Available

 const WORD DNS_FLAG_RCODE_SUCCESS   = 0x0000;   // No Error
 const WORD DNS_FLAG_RCODE_FORMAT    = 0x0001;   // Format Error
 const WORD DNS_FLAG_RCODE_SERVER    = 0x0002;   // Server Error
 const WORD DNS_FLAG_RCODE_NAME      = 0x0003;   // Not Exist Error
 const WORD DNS_FLAG_RCODE_IMPLEMENT = 0x0004;   // Not Implmemted Error
 const WORD DNS_FLAG_RCODE_REFUSED   = 0x0005;   // Refused Error

 // DNS_QUEST

 const WORD DNS_QUEST_TYP_ADDRESS    = 0x0100;   // Address
 const WORD DNS_QUEST_TYP_CNAME      = 0x0500;   // String

 const WORD DNS_QUEST_ClASS_INTERNET = 0x0100;   // Class Internet
 const WORD DNS_QUEST_CLASS_PTR      = 0x0C00;   // Class PTR ?


 /*
 //// Sample
 ////
 static UCHAR DnsQuestion[] = {0xFF, 0x00,              // DNS_ID
                               0x01, 0x00,              // DNS_Flags
                               0x00, 0x01,              // DNS_cQuestion
                               0x00, 0x00,              // DNS_cAnswer
                               0x00, 0x00,              // DNS_cAuthority
                               0x00, 0x00,              // DNS_cAdditional

                               0x06, 0x67, 0x6F, 0x6F,  // [0x06]Google.com[0x00]
                               0x67, 0x6C, 0x65, 0x03,  //  0x06 == strlen("Google") 
                               0x63, 0x6F, 0x6D, 0x00,  //

                               0x00, 0x01,              // DNS_QTYP_ADDRESS
                               0x00, 0x01 };            // DNS_QClASS_INTERNET

 ////////////////////////////////////////////////////////////////////////////////////////////

 static UCHAR DnsAnswer[] = {  0xFF, 0x00,              // DNS_ID
                               0x81, 0x80,              // DNS_Flags         (!!)
                               0x00, 0x01,              // DNS_cQuestion
                               0x00, 0x03,              // DNS_cAnswer       (!!)
                               0x00, 0x00,              // DNS_cAuthority
                               0x00, 0x00,              // DNS_cAdditional

                               0x06, 0x67, 0x6F, 0x6F,  // [0x06]Google.com[0x00]
                               0x67, 0x6C, 0x65, 0x03,  //  0x06 == strlen("Google") 
                               0x63, 0x6F, 0x6D, 0x00,  //

                               0x00, 0x01,              // DNS_QTYP_ADDRESS
                               0x00, 0x01,              // DNS_QClASS_INTERNET

                                                        // DNS_ANSWER

                               0xC0, 0x0C,              // DNS_pNAME
                               0x00, 0x01,              // DNS_QTYP_ADDRESS
                               0x00, 0x01,              // DNS_QClASS_INTERNET
                               0x00, 0x00, 0x06, 0x1D,  // DNS_TTL 26'05''
                               0x00, 0x04,
                               0x42, 0xF9, 0x5D, 0x68,  // DNS_IP 66.249.93.104

                               0xC0, 0x0C,              // DNS_pNAME
                               0x00, 0x01,              // DNS_QTYP_ADDRESS
                               0x00, 0x01,              // DNS_QClASS_INTERNET
                               0x00, 0x00, 0x06, 0x1D,  // DNS_TTL 26'05''
                               0x00, 0x04,
                               0xD8, 0xEF, 0x3B, 0x68,  // DNS_IP 216.239.59.104

                               0xC0, 0x0C,              // DNS_pNAME
                               0x00, 0x01,              // DNS_QTYP_ADDRESS
                               0x00, 0x01,              // DNS_QClASS_INTERNET
                               0x00, 0x00, 0x06, 0x1D,  // DNS_TTL 26'05''
                               0x00, 0x04,
                               0x48, 0x0E, 0xDD, 0x68   // DNS_IP 72.14.221.104
                             };
*/
//////////////////////////////////////////////////////////////////////////////////
 // [ END ]
//////////////////////////////////////////////////////////////////////////////////

#endif // _C_NET_RAW_PROTO_H_
