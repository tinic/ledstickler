#include "./artnet.h"
#include "./color.h"

namespace ledstickler {

static constexpr color_convert<uint8_t> convert;

std::vector<std::array<uint8_t, artnet_output_packet_size>> create_artnet_output_packets(const fixture &f) {
    std::vector<std::array<uint8_t, artnet_output_packet_size>> packets;

    // https://art-net.org.uk/structure/streaming-packets/artdmx-packet-definition/
	
    constexpr uint16_t artnet_output_packet_id = 0x5000;
    constexpr uint16_t artnet_output_packet_version = 14;
    constexpr size_t artnet_dmx_len = 512;

    uint16_t uni_index = 0;
    auto iter = f.points.begin();
    for (size_t len = f.points.size(); len > 0; ) {
        size_t chunk_len = std::min(artnet_dmx_len / (3 * 2), len);
        std::vector<std::pair<vec4, vec4>> chunk(iter, iter + chunk_len);
        
        std::array<uint8_t, artnet_output_packet_size> packet = { 0 };
        
        packet.at( 8) = (artnet_output_packet_id >> 0) & 0xFF;
        packet.at( 9) = (artnet_output_packet_id >> 8) & 0xFF;
        packet.at(10) = ( artnet_output_packet_version >> 8 ) & 0xFF;
        packet.at(11) = ( artnet_output_packet_version >> 0 ) & 0xFF;
        packet.at(12) = 0; // seq 
        packet.at(13) = 0; // phy 
        packet.at(14) = ( f.universes[uni_index] >> 0 ) & 0xFF;
        packet.at(15) = ( f.universes[uni_index] >> 8 ) & 0xFF;
        packet.at(16) = ( artnet_output_packet_size >> 8 ) & 0xFF;
        packet.at(17) = ( artnet_output_packet_size >> 0 ) & 0xFF;

        memcpy(packet.data(), "Art-Net", 8);

        size_t offset = 18;
        for (auto item : chunk) {
            const rgba<uint16_t> col(convert.CIELUV2sRGB(item.first));
            printf("%04x %04x %04x\n", col.r, col.g, col.b);
            packet.at(offset + 0) = ( col.r >> 8 ) & 0xFF; 
            packet.at(offset + 1) = ( col.r >> 0 ) & 0xFF; 
            packet.at(offset + 2) = ( col.g >> 8 ) & 0xFF; 
            packet.at(offset + 3) = ( col.g >> 0 ) & 0xFF; 
            packet.at(offset + 4) = ( col.b >> 8 ) & 0xFF; 
            packet.at(offset + 5) = ( col.b >> 0 ) & 0xFF; 
            offset += 6;
        }
        
        packets.push_back(packet);

        len -= chunk_len;
        iter += chunk_len;
        uni_index++;
    }
    
    return packets;
}

};
