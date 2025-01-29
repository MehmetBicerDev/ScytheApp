#pragma once
#include <netincludes.h>
#include <portaudio.h>
#include <opus/opus.h>

#include <c_voice_data_ack.h>



namespace scythe
{

    class c_voicechat {
    private:
        PaStream* inputStream;
        PaStream* outputStream;
        OpusEncoder* encoder;
        OpusDecoder* decoder;
        SOCKET sockfd;
        float inputVolume;
        float outputVolume;

        static const int SAMPLE_RATE = 16000;
        static const int FRAMES_PER_BUFFER = 960;
        static const int OPUS_MAX_PACKET = 4000;

    public:
        // Constructor to initialize volumes and socket
        c_voicechat(float inputVol = 1.0f, float outputVol = 1.0f)
            : inputStream(nullptr), outputStream(nullptr),
            encoder(nullptr), decoder(nullptr), sockfd(INVALID_SOCKET),
            inputVolume(inputVol), outputVolume(outputVol) {}
        static bool is_muted;
        static bool is_deafened;
        static std::deque<net::c_voice_data_ack> voice_data;
        // Initialize the voice chat system (WSA, PortAudio, Opus)
        bool initialize() {


            int error;
            encoder = opus_encoder_create(SAMPLE_RATE, 1, OPUS_APPLICATION_VOIP, &error);
            decoder = opus_decoder_create(SAMPLE_RATE, 1, &error);
            if (error != OPUS_OK) {
                logger::error("opus initialization failed!");
                return false;
            }
            Pa_Initialize();
            return true;
        }

        // Start recording and sending audio to the server
        void startRecording() {
            // Open PortAudio stream for input
            Pa_OpenDefaultStream(&inputStream, 1, 0, paInt16, SAMPLE_RATE, FRAMES_PER_BUFFER, recordCallback, this);
            Pa_StartStream(inputStream);
        }

        // Start receiving and playing audio from the server
        void startPlayback() {
            // Open PortAudio stream for output
            Pa_OpenDefaultStream(&outputStream, 0, 1, paInt16, SAMPLE_RATE, FRAMES_PER_BUFFER, playCallback, this);
            Pa_StartStream(outputStream);
        }

        // Stop the voice chat (close streams and clean up resources)
        void stop() {
            if (inputStream) {
                Pa_StopStream(inputStream);
                Pa_CloseStream(inputStream);
            }
            if (outputStream) {
                Pa_StopStream(outputStream);
                Pa_CloseStream(outputStream);
            }

            closesocket(sockfd);
            Pa_Terminate();
            opus_encoder_destroy(encoder);
            opus_decoder_destroy(decoder);
        }

        // Setters for volume control
        void setInputVolume(float volume) { inputVolume = volume; }
        void setOutputVolume(float volume) { outputVolume = volume; }

    private:
        // Callback for capturing and sending audio (recording)
        static int recordCallback(const void* inputBuffer, void* outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags, void* userData) {
            (void)outputBuffer;
            (void)timeInfo;
            (void)statusFlags;

            if (!is_muted)
            {
                c_voicechat* vc = (c_voicechat*)userData;
                opus_int16* inputSamples = (opus_int16*)inputBuffer;

                // Adjust the input volume by scaling the samples
                for (unsigned long i = 0; i < framesPerBuffer; ++i) {
                    inputSamples[i] = static_cast<opus_int16>(inputSamples[i] * vc->inputVolume);
                }

                // Encode and send the audio
                unsigned char opusData[OPUS_MAX_PACKET];
                int opusDataSize = opus_encode(vc->encoder, inputSamples, framesPerBuffer, opusData, sizeof(opusData));

                if (opusDataSize > 0) {
                    send_voice_data(opusData, opusDataSize);
                    // send(vc->sockfd, (const char*)opusData, opusDataSize, 0);  // Send to server
                }
            }
            return paContinue;
        }
        static void send_voice_data(unsigned char*, int );
        // Callback for receiving and playing audio (playback)
        static int playCallback(const void* inputBuffer, void* outputBuffer,
            unsigned long framesPerBuffer,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags statusFlags, void* userData) {
            (void)inputBuffer;
            (void)timeInfo;
            (void)statusFlags;

            c_voicechat* vc = (c_voicechat*)userData;
            opus_int16* outputSamples = (opus_int16*)outputBuffer;
            if (is_deafened) {
                std::fill(outputSamples, outputSamples + framesPerBuffer, 0);  // Zero out buffer
                voice_data.empty();
                return paContinue;  // Continue the stream with silence
            }
            if (voice_data.empty())
            {
                std::fill(outputSamples, outputSamples + framesPerBuffer, 0);

            }
            else
            while (!voice_data.empty())
            {
                auto data = voice_data.front();

                int bytesReceived = data.data.size();
                if (bytesReceived > 0) {
                    // Decode the audio
                    opus_decode(vc->decoder, data.data.buffer(), bytesReceived, outputSamples, framesPerBuffer, 0);

                    // Adjust the output volume by scaling the samples
                    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
                        outputSamples[i] = static_cast<opus_int16>(outputSamples[i] * vc->outputVolume);
                    }
                    voice_data.pop_front();
                }
            }
          
            return paContinue;
        }
    };
}