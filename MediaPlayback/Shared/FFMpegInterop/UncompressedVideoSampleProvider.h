//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#pragma once
#include "UncompressedSampleProvider.h"

extern "C"
{
#include <libswscale/swscale.h>
}


namespace FFmpegInterop
{
    class UncompressedVideoSampleProvider 
        : public UncompressedSampleProvider
    {
    public:
        UncompressedVideoSampleProvider(
            _In_ std::weak_ptr<FFmpegReader> reader,
            _In_ AVFormatContext* avFormatCtx,
            _In_ AVCodecContext* avCodecCtx);
        virtual ~UncompressedVideoSampleProvider();

        virtual HRESULT WriteAVPacketToStream(
            _In_ ABI::Windows::Storage::Streams::IDataWriter* writer, 
            _In_ AVPacket* avPacket) override;
        virtual HRESULT DecodeAVPacket(
            _In_ ABI::Windows::Storage::Streams::IDataWriter* dataWriter, 
            _In_ AVPacket* avPacket, 
            _Inout_ int64_t& framePts, 
            _Inout_ int64_t& frameDuration) override;
        virtual HRESULT AllocateResources() override;

    private:
        SwsContext* m_pSwsCtx;
        int m_rgVideoBufferLineSize[4];
        uint8_t* m_rgVideoBufferData[4];
    };
}

