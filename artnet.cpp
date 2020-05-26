#include "./artnet.h"
#include "./color.h"

namespace ledstickler {

static constexpr color_convert<uint8_t> convert;

std::vector<std::vector<uint8_t>> create_artnet_output_packets(const fixture &f) {
    std::vector<std::vector<uint8_t>> packets;

    // https://art-net.org.uk/structure/streaming-packets/artdmx-packet-definition/
	
    constexpr uint16_t artnet_output_packet_id = 0x5000;
    constexpr uint16_t artnet_output_packet_version = 14;
    constexpr size_t artnet_dmx_len = 512;

    uint16_t uni_index = 0;
    auto iter = f.points.begin();
    for (size_t len = f.points.size(); len > 0; ) {
        size_t chunk_len = std::min(artnet_dmx_len / (3 * 2), len);
        std::vector<std::pair<vec4, vec4>> chunk(iter, iter + chunk_len);
        
        std::vector<uint8_t> packet;

        packet.push_back('A');
        packet.push_back('r');
        packet.push_back('t');
        packet.push_back('-');
        packet.push_back('N');
        packet.push_back('e');
        packet.push_back('t');
        packet.push_back(0);

        packet.push_back(uint8_t( (artnet_output_packet_id >> 0) & 0xFF ));
        packet.push_back(uint8_t( (artnet_output_packet_id >> 8) & 0xFF ));
        packet.push_back(uint8_t( (artnet_output_packet_version >> 8 ) & 0xFF ));
        packet.push_back(uint8_t( (artnet_output_packet_version >> 0 ) & 0xFF ));
        packet.push_back(0); // seq 
        packet.push_back(0); // phy 
        packet.push_back(uint8_t( (f.universes[uni_index] >> 0 ) & 0xFF ));
        packet.push_back(uint8_t( (f.universes[uni_index] >> 8 ) & 0xFF ));
        packet.push_back(uint8_t( ( (chunk_len * 6 ) >> 8) & 0xFF ));
        packet.push_back(uint8_t( ( (chunk_len * 6 ) >> 0) & 0xFF ));

        size_t offset = 18;
        for (auto item : chunk) {
            const rgba<uint16_t> col(convert.CIELUV2LED(item.first));
            packet.push_back(uint8_t( ( col.r >> 8 ) & 0xFF )); 
            packet.push_back(uint8_t( ( col.r >> 0 ) & 0xFF )); 
            packet.push_back(uint8_t( ( col.g >> 8 ) & 0xFF )); 
            packet.push_back(uint8_t( ( col.g >> 0 ) & 0xFF )); 
            packet.push_back(uint8_t( ( col.b >> 8 ) & 0xFF )); 
            packet.push_back(uint8_t( ( col.b >> 0 ) & 0xFF )); 
            offset += 6;
        }
        
        packets.push_back(packet);

        len -= chunk_len;
        iter += chunk_len;
        uni_index++;
    }
    
    return packets;
}

}
