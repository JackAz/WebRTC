#include <iostream>
#include "RTCinit.h"
#include "network.h"


using namespace webrtc;

int main()
{
	int error = 0;
#ifdef AUDIO



	////创建voiceEngine实例//
	webrtc::VoiceEngine *_voiceEngine = NULL;
	_voiceEngine = webrtc::VoiceEngine::Create();

	VoEBase*             _veBase;				//
	VoENetwork*          _veNetwork;			//声明网络传输类
	VoECodec*            _veCodec;				//声明编解码器
	VoERTP_RTCP*         _veRTCP;				//声明RTP/RTCP类
	VoEAudioProcessing*     _veApmPtr;			//声明音频处理类

	_veBase = VoEBase::GetInterface(_voiceEngine);
	_veNetwork = VoENetwork::GetInterface(_voiceEngine);
	_veCodec = VoECodec::GetInterface(_voiceEngine);
	_veRTCP = VoERTP_RTCP::GetInterface(_voiceEngine);
	_veApmPtr = VoEAudioProcessing::GetInterface(_voiceEngine);

	//编码器选择，编码的配置参数可以配置CodecInst：  
	CodecInst voiceCodec;
	int voicenumOfVeCodecs = _veCodec->NumOfCodecs();
	for (int i = 0; i < voicenumOfVeCodecs;++i) {
		if (_veCodec->GetCodec(i, voiceCodec) != -1) {
			if (strncmp(voiceCodec.plname, "ISAC", 4) == 0)
				break;
		}
	}

	strcpy(voiceCodec.plname, "ISAC");
	voiceCodec.plfreq = 16000;    // iSAC宽带模式  
	voiceCodec.pltype = 103;      // 默认动态负载类型  
	voiceCodec.pacsize = 480;      // 480kbps，即使用30ms的packet size  
	voiceCodec.channels = 1;        // 单声道  
	voiceCodec.rate = -1;       // 信道自适应模式，单位bps  

	int ret = 0;

	ret = _veBase->Init();
	if (ret < 0) {
		printf("ERR _veBase->Init() \n");
		return -1;
	}
	//网络传输  
	int audioChannel = _veBase->CreateChannel();
	VoiceChannelTransport  *voice_channel_transport =new VoiceChannelTransport(_veNetwork, audioChannel);

	int error = voice_channel_transport->SetLocalReceiver(localport + Addport);
	if (error == -1) {
		printf("ERROR in SetLocalReceiver\n");
		return -1;
	}

	error = voice_channel_transport->SetSendDestination(targetip, targetport + Addport);
	if (error == -1) {
		printf("ERROR in SetSendDestination\n");
		return -1;
	}

	ret = _veRTCP->SetRTCPStatus(audioChannel, true);
	if (ret < 0) {
		printf("ERR SetRTCPStatus \n");
		return -1;
	}

	ret = _veCodec->SetSendCodec(audioChannel, voiceCodec);
	if (ret < 0) {
		printf("ERR _veCodec->SetSendCodec\n");
		return -1;
	}

	ret = _veBase->StartPlayout(audioChannel);
	if (ret < 0) {
		printf("ERR __veBase->StartPlayout\n");
		return -1;
	}

	ret = _veBase->StartSend(audioChannel);
	if (ret < 0) {
		printf("ERR _veBase->StartSend\n");
		return -1;
	}

	ret = _veBase->StartReceive(audioChannel);
	if (ret < 0) {
		printf("ERR _veBase->StartReceive\n");
		return -1;
	}

	NsModes mode(kNsDefault);
	ret = _veApmPtr->SetRxNsStatus(audioChannel, true, mode);
	if (ret < 0) {
		printf("ERR _veBase->SetRxNsStatus\n");
		return -1;
	}

	AgcModes agcmode(kAgcDefault);
	ret = _veApmPtr->SetRxAgcStatus(audioChannel, true, agcmode);
	if (ret < 0) {
		printf("ERR _veApmptr->SetRxAgcStatus\n");
		return -1;
	}

	ret = _veApmPtr->SetEcStatus(true, kEcAec);
	if (ret < 0) {
		printf("ERR _veApmptr->SetRxAgcStatus\n");
		return -1;
	}

#endif // Audio

	/////创建一个videoEngine实例///////////////////////////////////////
	webrtc::VideoEngine * ptrVie = NULL;
	ptrVie = webrtc::VideoEngine::Create();
	if (ptrVie == NULL) {
		printf("Error in vieEngine create\n");
		return -1;
	}

	////初始化videoEngine并创建一个Channel////////////////////////////
	webrtc::ViEBase *ptrViEBase = webrtc::ViEBase::GetInterface(ptrVie);
	if (ptrViEBase == NULL) {
		printf("Error in vieBase::GetInterface\n");
		return -1;
	}

	error = ptrViEBase->Init();//Init()其实是对videoEngine的初始化
	if (error == -1) {
		printf("Error in vieBase::Init\n");
		return -1;
	}
#ifdef AUDIO
	ptrViEBase->SetVoiceEngine(_voiceEngine);				//音视频同步设置video&voice
#endif // Audio

	

	webrtc::ViERTP_RTCP * ptrVieRtpRtcp = webrtc::ViERTP_RTCP::GetInterface(ptrVie);
	if (ptrVieRtpRtcp == NULL) {
		printf("Error in ViERTP_RTCP::GetInterface\n");
		return -1;
	}

	int videoChannel = -1;
	error = ptrViEBase->CreateChannel(videoChannel);
	if (error == -1) {
		printf("Error in VieBase::CreateChannel\n");
		return -1;
	}

#ifdef AUDIO
	ptrViEBase->ConnectAudioChannel(videoChannel, audioChannel);	//音视频同步设置video&voice
#endif // Audio

#ifdef SEND



	////列出可用的capture devices等待用户进行选择, 然后进行allocate和connect，最后start选中的capture device//
	webrtc::ViECapture * ptrVieCapture = webrtc::ViECapture::GetInterface(ptrVie);
	if (ptrVieCapture == NULL) {
		printf("Error in VieCapture::GetInterface\n");
		return -1;
	}

	const unsigned int KMaxDeviceNameLength = 128;
	const unsigned int KMaxUniqueIDLength = 256;
	char deviceName[KMaxDeviceNameLength];
	memset(deviceName, 0, KMaxDeviceNameLength);
	char uniqueId[KMaxUniqueIDLength];
	memset(uniqueId, 0, KMaxUniqueIDLength);

	printf("Available capture devices :\n");
	int captureIdx = 0;
	for (captureIdx = 0; captureIdx < ptrVieCapture->NumberOfCaptureDevices(); captureIdx++) {
		memset(deviceName, 0, KMaxDeviceNameLength);
		memset(uniqueId, 0, KMaxUniqueIDLength);

		error = ptrVieCapture->GetCaptureDevice(captureIdx, deviceName,
			KMaxDeviceNameLength, uniqueId, KMaxUniqueIDLength);
		if (error == -1) {
			printf("ERROR in ViECapture::GetCaptureDevice\n");
			return -1;
		}
		printf("\t %d. %s\n", captureIdx + 1, deviceName);
	}
	//printf("\nChoose capture device :\n");
	//if (scanf("%d", &captureIdx) != 1)
	//{
	//	printf("Error in scanf()\n");
	//	return -1;
	//}
	//getchar();
	//captureIdx = captureIdx - 1; // Compensate for idx start at 1. 

	captureIdx = 0;	//默认设备
	error = ptrVieCapture->GetCaptureDevice(captureIdx, deviceName,KMaxDeviceNameLength, uniqueId, KMaxUniqueIDLength);
	if (error == -1) {
		printf("ERROR in ViECapture::GetCaptureDevice\n");
		return -1;
	}

	int captureId = 0;
	error = ptrVieCapture->AllocateCaptureDevice(uniqueId, KMaxUniqueIDLength, captureId);
	if (error == -1) {
		printf("ERROR in ViECapture::AllocateCaptureDevice\n");
		return -1;
	}

	error = ptrVieCapture->ConnectCaptureDevice(captureId, videoChannel);
	if (error == -1) {
		printf("ERROR in ViECapture::ConnectCaptureDevice\n");
		return -1;
	}

	error = ptrVieCapture->StartCapture(captureId);
	if (error == -1) {
		printf("ERROR in ViECapture::StartCapture\n");
		return -1;
	}

#endif // SEND

	////设置RTP/RTCP所采用的模式///////////////////////////////////
	error = ptrVieRtpRtcp->SetRTCPStatus(videoChannel,webrtc::kRtcpCompound_RFC4585);
	if (error == -1) {
		printf("ERROR in ViERTP_RTCP::SetRTCPStatus\n");
		return -1;
	}

	////设置接收端解码器出问题的时候，比如关键帧丢失或损坏，如何重新请求关键帧的方式//
	error = ptrVieRtpRtcp->SetKeyFrameRequestMethod(videoChannel, webrtc::kViEKeyFrameRequestPliRtcp);
	if (error == -1) {
		printf("ERROR in ViERTP_RTCP::SetKeyFrameRequestMethod\n");
		return -1;
	}

	////设置是否为当前channel使用REMB（Receiver Estimated Max Bitrate）包，发送端可以用它表明正在编码当前channel
	////接收端用它来记录当前channel的估计码率//////////////////////////////////////
	error = ptrVieRtpRtcp->SetRembStatus(videoChannel, true, true);
	if (error == -1) {
		printf("ERROR in ViERTP_RTCP::SetTMMBRStatus\n");
		return -1;
	}

	//////设置rendering用于显示/////////////////////////////////////////////////////
	webrtc::ViERender* ptrVieRender = webrtc::ViERender::GetInterface(ptrVie);
	if (ptrVieRender == NULL) {
		printf("ERROR in ViERender::GetInterface\n");
		return -1;
	}

	////使用了webrtc已经为我们定义好的类ViEWindowCreator，它有一个成员函数CreateTwoWindows可以直接创建两个窗口，
	////只需实现定义好窗口名称、窗口大小以及坐标即可////////
	ViEWindowCreator windowCreator;
	ViEAutoTestWindowManagerInterface* windowManager = windowCreator.CreateTwoWindows();
	//////////////////////////////////////////////////////////////////////////////////
#ifdef SENDON
	////显示本地摄像头数据window1//////////////////////////////////////////////////////
	error = ptrVieRender->AddRenderer(captureId, windowManager->GetWindow1(), 10, 0.0, 0.0, 1.0, 1.0);
	if (error == -1)
	{
		printf("ERROR in ViERender::AddRenderer\n");
		return -1;
	}

	error = ptrVieRender->StartRender(captureId);
	if (error == -1)
	{
		printf("ERROR in ViERender::StartRender\n");
		return -1;
	}
#endif // SEND

#ifdef  RECV 
	
	/////显示接收到的解码数据//
	error = ptrVieRender->AddRenderer(videoChannel, windowManager->GetWindow2(), 1, 0.0, 0.0, 1.0,
		1.0);
	if (error == -1) {
		printf("ERROR in ViERender::AddRenderer\n");
		return -1;
	}

	error = ptrVieRender->StartRender(videoChannel);
	if (error == -1) {
		printf("ERROR in ViERender::StartRender\n");
		return -1;
	}
#endif //  RECV

	
	///////设置编解码器//////////////////////////////////////
	webrtc::ViECodec* ptrViECodec = webrtc::ViECodec::GetInterface(ptrVie);
	if (ptrViECodec == NULL)
	{
		printf("ERROR in ViECodec::GetInterface\n");
		return -1;
	}

	VideoCodec videoCodec;

	int numOfVeCodecs = ptrViECodec->NumberOfCodecs();
	for (int i = 0; i < numOfVeCodecs; ++i) {
		if (ptrViECodec->GetCodec(i, videoCodec) != -1) {
			if (videoCodec.codecType == kVideoCodecVP8)
				break;
		}
	}
	///////设置获取视频的参数
	videoCodec.targetBitrate = 256*NK;
	videoCodec.minBitrate = 200*NK;
	videoCodec.maxBitrate = 300*NK;
	videoCodec.maxFramerate = 25*NK;

	videoCodec.width = 480*NK;
	videoCodec.height = 270*NK;

	error = ptrViECodec->SetSendCodec(videoChannel, videoCodec);
	assert(error != -1);

	error = ptrViECodec->SetReceiveCodec(videoChannel, videoCodec);
	assert(error != -1);

	///////设置接收和发送地址，然后开始发送和接收///////////////////////
	
	webrtc::ViENetwork* ptrViENetwork = webrtc::ViENetwork::GetInterface(ptrVie);
	if (ptrViENetwork == NULL) {
		printf("ERROR in ViENetwork::GetInterface\n");
		return -1;
	}
	//VideoChannelTransport是由我们自己定义的类，后面将会详细介绍
	VideoChannelTransport* video_channel_transport = NULL;
	video_channel_transport = new VideoChannelTransport(ptrViENetwork, videoChannel);

#ifdef  RECV
	error = video_channel_transport->SetLocalReceiver(localport);
	if (error == -1) {
		printf("ERROR in SetLocalReceiver\n");
		return -1;
	}
#endif 

#ifdef SEND
	error = video_channel_transport->SetSendDestination(targetip, targetport);
	if (error == -1) {
		printf("ERROR in SetSendDestination\n");
		return -1;
	}
#endif // SEND

#ifdef  RECV
	error = ptrViEBase->StartReceive(videoChannel);
	if (error == -1) {
		printf("ERROR in ViENetwork::StartReceive\n");
		return -1;
	}
#endif 

#ifdef SEND
	error = ptrViEBase->StartSend(videoChannel);
	if (error == -1) {
		printf("ERROR in ViENetwork::StartSend\n");
		return -1;
	}
#endif // SEND

	/////////////设置按下回车键停止通话/////////////////////////////////////////////
	{
		printf("\ncall started\n\n");
		printf("Press enter to stop...");
		system("pause");
#ifdef AUDIO
		_veBase->StopReceive(audioChannel);
		_veBase->StopSend(audioChannel);
		//结束，释放资源  
		if (_voiceEngine) {
			_veBase->DeleteChannel(audioChannel);
			_veBase->Release();
			_veNetwork->Release();
			_veCodec->Release();
			_veRTCP->Release();
			_veApmPtr->Release();

			VoiceEngine::Delete(_voiceEngine);
		}

#endif // AUDIO

		
		error = ptrViEBase->StopReceive(videoChannel);
		if (error == -1) {
			printf("ERROR in ViEBase::StopReceive\n");
			return -1;
		}

		error = ptrViEBase->StopSend(videoChannel);
		if (error == -1) {
			printf("ERROR in ViEBase::StopSend\n");
			return -1;
		}

#ifdef SEND
		error = ptrVieRender->StopRender(captureId);
		if (error == -1) {
			printf("ERROR in ViERender::StopRender\n");
			return -1;
		}

		error = ptrVieRender->RemoveRenderer(captureId);
		if (error == -1) {
			printf("ERROR in ViERender::RemoveRenderer\n");
			return -1;
		}
#endif // SEND

		error = ptrVieRender->StopRender(videoChannel);
		if (error == -1) {
			printf("ERROR in ViERender::StopRender\n");
			return -1;
		}

		error = ptrVieRender->RemoveRenderer(videoChannel);
		if (error == -1) {
			printf("ERROR in ViERender::RemoveRenderer\n");
			return -1;
		}

#ifdef SEND
		error = ptrVieCapture->StopCapture(captureId);
		if (error == -1) {
			printf("ERROR in ViECapture::StopCapture\n");
			return -1;
		}

		error = ptrVieCapture->DisconnectCaptureDevice(videoChannel);
		if (error == -1) {
			printf("ERROR in ViECapture::DisconnectCaptureDevice\n");
			return -1;
		}

		error = ptrVieCapture->ReleaseCaptureDevice(captureId);
		if (error == -1) {
			printf("ERROR in ViECapture::ReleaseCaptureDevice\n");
			return -1;
		}
#endif // SEND

		error = ptrViEBase->DeleteChannel(videoChannel);
		if (error == -1) {
			printf("ERROR in ViEBase::DeleteChannel\n");
			return -1;
		}

		delete video_channel_transport;

		int remainingInterfaces = 0;
		remainingInterfaces = ptrViECodec->Release();

#ifdef SEND
		remainingInterfaces += ptrVieCapture->Release();
#endif // SEND

		remainingInterfaces += ptrVieRtpRtcp->Release();
		remainingInterfaces += ptrVieRender->Release();
		remainingInterfaces += ptrViENetwork->Release();
		remainingInterfaces += ptrViEBase->Release();

		if (remainingInterfaces > 0) {
			printf("ERROR: Could not release all interfaces\n");
			return -1;
		}

		bool deleted = webrtc::VideoEngine::Delete(ptrVie);
		if (deleted == false) {
			printf("ERROR in VideoEngine::Delete\n");
			return -1;
		}
	}
	return 0;
}

/*
ViEBase用于

- 创建和销毁 VideoEngine 实例

- 创建和销毁 channels
- 将 video channel 和相应的 voice channel 连接到一起并同步
- 发送和接收的开始与停止
ViECapture用于

- 分配capture devices.
- 将 capture device 与一个或多个 channels连接起来.
- 启动或停止 capture devices.
- 获得capture device 的可用性.

ViECodec用于

- 设置发送和接收的编解码器.

- 设置编解码器特性.

- Key frame signaling.

- Stream management settings.

ViEError即一些预定义的错误消息

ViEExternalCodec用于注册除VP8之外的其他编解码器

ViEImageProcess提供以下功能
- Effect filters
- 抗闪烁
- 色彩增强
ViENetwork用于
- 配置发送和接收地址.
- External transport support.
- 端口和地址过滤.
- Windows GQoS functions and ToS functions.
- Packet timeout notification.
- Dead‐or‐Alive connection observations.
ViERender用于
- 为输入视频流、capture device和文件指定渲染目标.
- 配置render streams.

ViERTP_RTCP用于
- Callbacks for RTP and RTCP events such as modified SSRC or CSRC.

- SSRC handling.
- Transmission of RTCP reports.
- Obtaining RTCP data from incoming RTCP sender reports.
- RTP and RTCP statistics (jitter, packet loss, RTT etc.).
- Forward Error Correction (FEC).
- Writing RTP and RTCP packets to binary files for off‐line analysis of the call quality.
- Inserting extra RTP packets into active audio stream.
*/