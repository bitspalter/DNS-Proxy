# DNS-Proxy

g++ *.cpp -std=c++11 -o DNS-Proxy `pkg-config gtkmm-3.0 --cflags --libs`

setcap 'cap_net_bind_service=+ep' ./DNS-Proxy

Browser ---> DNS-Proxy(whitelist | Blacklist) ---> DNS-Server
