/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "webrtc/video_engine/vie_window_creator.h"
#include "webrtc/video_engine/vie_autotest_window_manager_interface.h"
#include "webrtc/video_engine/vie_window_manager_factory.h"
#include "RTCinit.h"

#if defined(WIN32)
#include <tchar.h>
#endif

ViEWindowCreator::ViEWindowCreator() {
	window_manager_ =
		ViEWindowManagerFactory::CreateWindowManagerForCurrentPlatform();
}

ViEWindowCreator::~ViEWindowCreator() {
  delete window_manager_;
}

ViEAutoTestWindowManagerInterface*
  ViEWindowCreator::CreateTwoWindows() {
#if defined(WIN32)
  TCHAR window1Title[1024] = _T("ViE test Window 1");
  TCHAR window2Title[1024] = _T("ViE test Window 2");
#else
  char window1Title[1024] = "ViE test Window 1";
  char window2Title[1024] = "ViE test Window 2";
#endif


#ifdef SEND
  AutoTestRect window1Size(960, 540, 200, 250);
#else
  AutoTestRect window1Size(960, 540, 5000, 250);
#endif // SEND
  
#ifdef RECV
  AutoTestRect window2Size(960, 540, 1000, 250);
#else
  AutoTestRect window2Size(960, 540, 5000, 250);
#endif // DEBUG


  window_manager_->CreateWindows(window1Size, window2Size, window1Title,window2Title);
  window_manager_->SetTopmostWindow();

  return window_manager_;
}

void ViEWindowCreator::TerminateWindows() {
  window_manager_->TerminateWindows();
}
