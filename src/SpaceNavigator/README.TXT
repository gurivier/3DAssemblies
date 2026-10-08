
INSTALLATION
""""""""""""

Prepare a Guest Virtual Machine running Debian with Virtual Box that will handle the first SpaceNavigator.


COMPILATION OF THE CLIENT
"""""""""""""""""""""""""

gcc client_spnav_af_unix.c -DBUILD_AF_UNIX -lpthread -lspnav -o client_spnav_af_unix


EXECUTION
"""""""""

1) From host: Start the server

   ./server 10222

2) Start the guest OS from VirtualBox

   a) Plug the wire of the first SpaceNavigator device

   b) Connect the SpaceNavigator device to the guest in the Virtual Machine's USB manager

   c) Start the client from the guest:

      ./client_af_unix 10.0.2.2 10222 1

3) From the host OS

   a) Plug the wire of the second SpaceNavigator device

   b) Start the client from the host:

      ./client_af_unix 127.0.0.1 10222 2


