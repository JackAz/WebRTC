#pragma once
#include "RTCinit.h"

using namespace webrtc;

class VoiceChannelTransport : public webrtc::test::UdpTransportData	/*,Transport */ {
public:
	VoiceChannelTransport(VoENetwork* vo, int _channel);
	virtual ~VoiceChannelTransport();
	// Specifies the ports to receive RTP packets on.
	int SetLocalReceiver(uint16_t rtp_port);
	// Specifies the destination port and IP address for a specified channel.
	int SetSendDestination(const char* ip_address, uint16_t rtp_port);

	//	interface for class Transport addwaz
	//	int SendPacket(int channel, const void *data, int len);     //发送端打包一个rtp包用于发送的api
	//	int SendRTCPPacket(int channel, const void *data, int len);

	virtual void IncomingRTPPacket(const int8_t* incoming_rtp_packet,
		const int32_t packet_length,
		const char* /*from_ip*/,
		const uint16_t /*from_port*/) OVERRIDE;

	virtual void IncomingRTCPPacket(const int8_t* incoming_rtcp_packet,
		const int32_t packet_length,
		const char* /*from_ip*/,
		const uint16_t /*from_port*/) OVERRIDE;
private:
	int channel;
	VoENetwork* voe_network_;
	webrtc::test::UdpTransport* socket_transport_;
};

class VideoChannelTransport : public webrtc::test::UdpTransportData {
public:
	VideoChannelTransport(ViENetwork* vie_network, int channel);
	virtual  ~VideoChannelTransport();

	virtual void IncomingRTPPacket(const int8_t* incoming_rtp_packet,
		const int32_t packet_length,
		const char* /*from_ip*/,
		const uint16_t /*from_port*/) OVERRIDE;

	virtual void IncomingRTCPPacket(const int8_t* incoming_rtcp_packet,
		const int32_t packet_length,
		const char* /*from_ip*/,
		const uint16_t /*from_port*/) OVERRIDE;
	// End implementation of UdpTransportData.
	// Specifies the ports to receive RTP packets on.
	int SetLocalReceiver(uint16_t rtp_port);
	// Specifies the destination port and IP address for a specified channel.
	int SetSendDestination(const char* ip_address, uint16_t rtp_port);

private:
	int channel_;
	ViENetwork* vie_network_;
	webrtc::test::UdpTransport* socket_transport_;
};


