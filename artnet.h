#ifndef _ARTNET_H_
#define _ARTNET_H_

#include "./fixture.h"

namespace ledstickler {

    constexpr uint16_t artnet_port = 6454;
    constexpr size_t artnet_sync_packet_size = 14;

    std::vector<std::vector<uint8_t>> create_artnet_output_packets(const fixture &f);

    constexpr std::array<uint8_t, artnet_sync_packet_size> make_arnet_sync_packet() {
        std::array<uint8_t, artnet_sync_packet_size> packet = { 0 };

        // https://art-net.org.uk/structure/streaming-packets/artsync-packet-definition/
    
        constexpr uint16_t artnet_output_packet_id = 0x5200;
        constexpr uint16_t artnet_output_packet_version = 14;
        
        // packet.insert(packet.begin(), std::to_array("Art-Net"));

        packet.at( 0) = 'A';
        packet.at( 1) = 'r';
        packet.at( 2) = 't';
        packet.at( 3) = '-';
        packet.at( 4) = 'N';
        packet.at( 5) = 'e';
        packet.at( 6) = 't';
        packet.at( 7) = 0;

        packet.at( 8) = (artnet_output_packet_id >> 0) & 0xFF;
        packet.at( 9) = (artnet_output_packet_id >> 8) & 0xFF;
        packet.at(10) = ( artnet_output_packet_version >> 8 ) & 0xFF;
        packet.at(11) = ( artnet_output_packet_version >> 0 ) & 0xFF;

        return packet;
    }
}

#endif  // #ifndef _ARTNET_H_
