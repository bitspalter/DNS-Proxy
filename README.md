# DNS-Proxy (alpha)

1. compile:
g++ *.cpp -std=c++11 -o DNS-Proxy `pkg-config gtkmm-3.0 --cflags --libs`

2. set right to open udp port 53 as non root:
setcap 'cap_net_bind_service=+ep' ./DNS-Proxy

Options:
  -h help
  -d dns server
  -a autostart
  -l list (white|black)
  -i interface
  -b blacklist (path)
  -w whitelist (path)
  
Example: ./dns-proxy -i eth0 -d 192.168.0.1 -l black -b /home/foo/blacklist.txt -a
    

Browser <---> DNS-Proxy(whitelist | Blacklist) <---> DNS-Server

![alt tag](https://github.com/bitspalter/DNS-Proxy/blob/master/DNS-Proxy.png "DNS-Proxy")
