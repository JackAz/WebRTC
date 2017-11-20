#pragma once

#define RECV
#define SEND
#define SENDON
//#define AUDIO
//#define FEC
//#define targetip  "10.0.1.198"
#define targetip  "127.0.0.1"
//#define targetip  "10.60.103.185"
//#define targetip	"202.120.189.236"
#define targetport	6250
#define localport	6250
#define Addport		10
#define destrtcp	6150
#define localrtcp	6150
#define NK			2


#include "webrtc/video_engine/vie_base.h"
#include "webrtc/video_engine/vie_network.h"
#include "webrtc/video_engine/vie_codec.h"
#include "webrtc/video_engine/vie_network.h"
#include "webrtc/video_engine/vie_rtp_rtcp.h"
#include "webrtc/video_engine/vie_image_process.h"
#include "webrtc/video_engine/vie_capture.h"
#include "webrtc/modules/video_capture/include/video_capture_factory.h"
#include "webrtc/video_engine/vie_render.h"
#include "webrtc/video_engine/vie_external_codec.h"
#include "webrtc/test/channel_transport/udp_transport.h"
#include "webrtc/video_engine/vie_autotest_window_manager_interface.h"
#include "webrtc/video_engine/vie_window_creator.h"

#include "webrtc\voice_engine\voe_base.h"
#include "webrtc\voice_engine\voe_network.h"
#include "webrtc\voice_engine\voe_codec.h"
#include "webrtc\voice_engine\voe_rtp_rtcp.h"
#include "webrtc\voice_engine\voe_errors.h"
#include "webrtc\voice_engine\voe_audio_processing.h"


//#pragma comment(lib,"libwebrtc_full.lib")


#pragma comment(lib,"audio_coding_module.lib")
#pragma comment(lib,"audio_conference_mixer.lib")
#pragma comment(lib,"audio_device.lib")
#pragma comment(lib,"audio_processing.lib")
#pragma comment(lib,"audio_processing_sse2.lib")
#pragma comment(lib,"audioproc_debug_proto.lib")
#pragma comment(lib,"bitrate_controller.lib")

#pragma comment(lib,"CNG.lib")
#pragma comment(lib,"common_audio.lib")
#pragma comment(lib,"common_audio_sse2.lib")
#pragma comment(lib,"common_video.lib")
#pragma comment(lib,"channel_transport.lib")
#pragma comment(lib,"directshow_baseclasses.lib")
#pragma comment(lib,"field_trial_default.lib")
#pragma comment(lib,"G711.lib")
#pragma comment(lib,"G722.lib")
#pragma comment(lib,"icui18n.lib")
#pragma comment(lib,"icuuc.lib")
#pragma comment(lib,"iLBC.lib")
#pragma comment(lib,"iSAC.lib")
#pragma comment(lib,"iSACFix.lib")
#pragma comment(lib,"jsoncpp.lib")
#pragma comment(lib,"libjpeg.lib")
#pragma comment(lib,"libvpx.lib")
#pragma comment(lib,"libvpx_asm_offsets_vp8.lib")
#pragma comment(lib,"libvpx_intrinsics_mmx.lib")
#pragma comment(lib,"libvpx_intrinsics_sse2.lib")
#pragma comment(lib,"libvpx_intrinsics_sse4_1.lib")
#pragma comment(lib,"libvpx_intrinsics_ssse3.lib")
#pragma comment(lib,"libyuv.lib")
#pragma comment(lib,"media_file.lib")
#pragma comment(lib,"neteq.lib")
#pragma comment(lib,"opus.lib")
#pragma comment(lib,"paced_sender.lib")
#pragma comment(lib,"PCM16B.lib")
#pragma comment(lib,"protobuf_lite.lib")
#pragma comment(lib,"rbe_components.lib")
#pragma comment(lib,"remote_bitrate_estimator.lib")
#pragma comment(lib,"rtp_rtcp.lib")
#pragma comment(lib,"sqlite3.lib")
#pragma comment(lib,"system_wrappers.lib")
#pragma comment(lib,"usrsctplib.lib")

#pragma comment(lib,"video_capture_module.lib")
#pragma comment(lib,"video_coding_utility.lib")
#pragma comment(lib,"video_engine_core.lib")
#pragma comment(lib,"video_processing.lib")
#pragma comment(lib,"video_processing_sse2.lib")
#pragma comment(lib,"video_render_module.lib")

#pragma comment(lib,"voice_engine.lib")

#pragma comment(lib,"webrtc_i420.lib")
#pragma comment(lib,"webrtc_opus.lib")
#pragma comment(lib,"webrtc_utility.lib")
#pragma comment(lib,"webrtc_video_coding.lib")
#pragma comment(lib,"webrtc_vp8.lib")
#pragma comment(lib,"webrtc_base.lib")
