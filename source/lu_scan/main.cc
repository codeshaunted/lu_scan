// averysumner - lu_scan
// source/lu_scan/main.cc
// contains entry point

#include <iostream>
#include <string>
#include <cstring>

#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include "NetworkIDManager.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"

#define LU_SCAN_AUTH_PORT 1001
#define LU_SCAN_PASSWORD "3.25 ND1"
#define LU_SCAN_CLIENT_VERSION 171022

int main(int argc, char* argv[]) {
    RakPeerInterface* peer_interface = RakNetworkFactory::GetRakPeerInterface();
    SocketDescriptor socket_descriptor;

    if (peer_interface && peer_interface->Startup(1, 10, &socket_descriptor, 1)) {
        // build client handshake BitStream
        RakNet::BitStream client_handshake;
        client_handshake.Write<uint8_t>(0x53);
        client_handshake.Write<uint16_t>(0);
        client_handshake.Write<uint32_t>(0);
        client_handshake.Write<uint8_t>(0);
        client_handshake.Write<uint32_t>(LU_SCAN_CLIENT_VERSION);
        client_handshake.Write<uint32_t>(0);
        client_handshake.Write<uint32_t>(5);
        client_handshake.Write<uint32_t>(0);
        client_handshake.Write<uint16_t>(LU_SCAN_AUTH_PORT);

        Packet* packet;
        
        for (uint32_t ip = 0; ip < UINT32_MAX; ip++) {
            peer_interface->Connect(std::to_string(ip).c_str(), LU_SCAN_AUTH_PORT, LU_SCAN_PASSWORD, std::strlen(LU_SCAN_PASSWORD));

            while (true) {
                for (packet = peer_interface->Receive(); packet; peer_interface->DeallocatePacket(packet), packet = peer_interface->Receive()) {
                    switch (packet->data[0]) {
                        case ID_CONNECTION_REQUEST_ACCEPTED:
                            std::cout << "Sucessfully connected to: " << ip << ", sending handshake!" << std::endl;
                            peer_interface->Send(&client_handshake, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
                            break;
                        case ID_CONNECTION_ATTEMPT_FAILED:
                            std::cout << "Could not connect to: " << ip << "!" << std::endl;
                            goto break_handler;
                            break;
                        case 0x53:
                            RakNet::BitStream server_handshake(packet->data, packet->length, false);
                            uint64_t header;
                            uint32_t server_version;

                            server_handshake.Read(header);
                            server_handshake.Read(server_version);

                            std::cout << "Received handshake response with version: " << server_version << "!" << std::endl;
                            peer_interface->CloseConnection(packet->systemAddress, true);
                            break;
                    }
                }
            }
            break_handler: ;
        }

        peer_interface->Shutdown(1000);
    }
}

#undef LU_SCAN_AUTH_PORT
#undef LU_SCAN_PASSWORD
#undef LU_SCAN_CLIENT_VERSION