#include "network.h"
using namespace webrtc;

VoiceChannelTransport::VoiceChannelTransport(VoENetwork* vo, int _channel) :channel(_channel), voe_network_(vo) {
	uint8_t socket_threads = 1;
	socket_transport_ = webrtc::test::UdpTransport::Create(channel, socket_threads);
	int registered = voe_network_->RegisterExternalTransport(channel, *socket_transport_);
}

VoiceChannelTransport::~VoiceChannelTransport() {
	//	netw->Release();
	webrtc::test::UdpTransport::Destroy(socket_transport_);
}

void VoiceChannelTransport::IncomingRTPPacket(const int8_t* incoming_rtp_packet, const int32_t packet_length, const char* /*from_ip*/, const uint16_t /*from_port*/) {
	voe_network_->ReceivedRTPPacket(channel, incoming_rtp_packet, packet_length);
}

void VoiceChannelTransport::IncomingRTCPPacket(const int8_t* incoming_rtcp_packet, const int32_t packet_length, const char* /*from_ip*/, const uint16_t /*from_port*/) {
	voe_network_->ReceivedRTCPPacket(channel, incoming_rtcp_packet, packet_length);
}

int VoiceChannelTransport::SetLocalReceiver(uint16_t rtp_port) {
	int return_value = socket_transport_->InitializeReceiveSockets(this, rtp_port, NULL, NULL, localrtcp + Addport);
	if (return_value == 0) {
		return socket_transport_->StartReceiving(500);
	}
	return return_value;
}

int VoiceChannelTransport::SetSendDestination(const char* ip_address, uint16_t rtp_port) {
#ifdef  FEC
	return socket_transport_->InitializeSendSockets(ip_address, rtp_port, destrtcp + Addport, true);
#else
	return socket_transport_->InitializeSendSockets(ip_address, rtp_port, destrtcp + Addport, false);
#endif //  FEC	
}


VideoChannelTransport::VideoChannelTransport(ViENetwork* vie_network, int channel) : channel_(channel), vie_network_(vie_network) {
	uint8_t socket_threads = 1;
	socket_transport_ = webrtc::test::UdpTransport::Create(channel, socket_threads);
	int registered = vie_network_->RegisterSendTransport(channel, *socket_transport_);
}

VideoChannelTransport::~VideoChannelTransport() {
	vie_network_->DeregisterSendTransport(channel_);
	webrtc::test::UdpTransport::Destroy(socket_transport_);
}

void VideoChannelTransport::IncomingRTPPacket(const int8_t* incoming_rtp_packet, const int32_t packet_length, const char* /*from_ip*/, const uint16_t /*from_port*/) {
	vie_network_->ReceivedRTPPacket(channel_, incoming_rtp_packet, packet_length, PacketTime());
}

void VideoChannelTransport::IncomingRTCPPacket(const int8_t* incoming_rtcp_packet, const int32_t packet_length, const char* /*from_ip*/, const uint16_t /*from_port*/) {
	vie_network_->ReceivedRTCPPacket(channel_, incoming_rtcp_packet, packet_length);
}

int VideoChannelTransport::SetLocalReceiver(uint16_t rtp_port) {
	int return_value = socket_transport_->InitializeReceiveSockets(this, rtp_port, NULL, NULL, localrtcp);
	if (return_value == 0) {
		return socket_transport_->StartReceiving(500);
	}
	return return_value;
}

int VideoChannelTransport::SetSendDestination(const char* ip_address, uint16_t rtp_port) {
#ifdef FEC
	return socket_transport_->InitializeSendSockets(ip_address, rtp_port, destrtcp, true);
#else
	return socket_transport_->InitializeSendSockets(ip_address, rtp_port, destrtcp, false);
#endif // FEC
}

//
//class MyObserver : public VoiceEngineObserver {
//public:
//	virtual void CallbackOnError(const int channel, const int err_code);
//};
//
//void MyObserver::CallbackOnError(const int channel, const int err_code) {
//	// Add printf for other error codes here
//	if (err_code == VE_TYPING_NOISE_WARNING) {
//		printf("  TYPING NOISE DETECTED \n");
//	}
//	else if (err_code == VE_RECEIVE_PACKET_TIMEOUT) {
//		printf("  RECEIVE PACKET TIMEOUT \n");
//	}
//	else if (err_code == VE_PACKET_RECEIPT_RESTARTED) {
//		printf("  PACKET RECEIPT RESTARTED \n");
//	}
//	else if (err_code == VE_RUNTIME_PLAY_WARNING) {
//		printf("  RUNTIME PLAY WARNING \n");
//	}
//	else if (err_code == VE_RUNTIME_REC_WARNING) {
//		printf("  RUNTIME RECORD WARNING \n");
//	}
//	else if (err_code == VE_SATURATION_WARNING) {
//		printf("  SATURATION WARNING \n");
//	}
//	else if (err_code == VE_RUNTIME_PLAY_ERROR) {
//		printf("  RUNTIME PLAY ERROR \n");
//	}
//	else if (err_code == VE_RUNTIME_REC_ERROR) {
//		printf("  RUNTIME RECORD ERROR \n");
//	}
//	else if (err_code == VE_REC_DEVICE_REMOVED) {
//		printf("  RECORD DEVICE REMOVED \n");
//	}
//}
