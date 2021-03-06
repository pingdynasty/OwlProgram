#ifndef __PROGRAM_VECTOR_H
#define __PROGRAM_VECTOR_H

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define OWL_PEDAL_HARDWARE    0x11
#define OWL_MODULAR_HARDWARE  0x12
#define OWL_RACK_HARDWARE     0x13
#define PRISM_HARDWARE        0x14
#define PLAYER_HARDWARE       0x15

#define PROGRAM_VECTOR_CHECKSUM_V11 0x40
#define PROGRAM_VECTOR_CHECKSUM_V12 0x50
#define PROGRAM_VECTOR_CHECKSUM_V13 0x51

#define CHECKSUM_ERROR_STATUS        -10
#define OUT_OF_MEMORY_ERROR_STATUS   -20
#define CONFIGURATION_ERROR_STATUS   -30

#define AUDIO_FORMAT_24B16          0x10
#define AUDIO_FORMAT_24B16_2X       0x12
#define AUDIO_FORMAT_24B16_4X       0x14
#define AUDIO_FORMAT_24B16_6X       0x16
#define AUDIO_FORMAT_24B16_8X       0x18
#define AUDIO_FORMAT_24B32          0x20
#define AUDIO_FORMAT_24B32_2X       0x22
#define AUDIO_FORMAT_24B32_4X       0x24
#define AUDIO_FORMAT_24B32_6X       0x26
#define AUDIO_FORMAT_24B32_8X       0x28
#define AUDIO_FORMAT_FORMAT_MASK    0xf0
#define AUDIO_FORMAT_CHANNEL_MASK   0x0f

  typedef enum { 
    AUDIO_IDLE_STATUS = 0, 
    AUDIO_READY_STATUS, 
    AUDIO_PROCESSING_STATUS, 
    AUDIO_EXIT_STATUS, 
    AUDIO_ERROR_STATUS 
  } ProgramVectorAudioStatus;

  typedef struct {
    uint8_t* location;
    uint32_t size;
  } MemorySegment;

   typedef struct {
     uint8_t checksum;
     uint8_t hardware_version;
     int32_t* audio_input;
     int32_t* audio_output;
     uint8_t audio_format;
     uint16_t audio_blocksize;
     uint32_t audio_samplingrate;
     int16_t* parameters;
     uint8_t parameters_size;
     uint16_t buttons;
     int8_t error;
     void (*registerPatch)(const char* name, uint8_t inputChannels, uint8_t outputChannels);
     void (*registerPatchParameter)(uint8_t id, const char* name);
     void (*programReady)(void);
     void (*programStatus)(ProgramVectorAudioStatus status);
     int (*serviceCall)(int service, void** params, int len);
     uint32_t cycles_per_block;
     uint32_t heap_bytes_used;
     char* message;
     void (*setButton)(uint8_t id, uint16_t state, uint16_t samples);
     void (*setPatchParameter)(uint8_t id, int16_t value);
     void (*buttonChangedCallback)(uint8_t bid, uint16_t state, uint16_t samples);
     MemorySegment* heapLocations;
   } ProgramVector;

#define CHECKSUM_ERROR_STATUS      -10
#define OUT_OF_MEMORY_ERROR_STATUS -20
#define CONFIGURATION_ERROR_STATUS -30

  extern ProgramVector programVector;
#define getProgramVector() (&programVector)

#ifdef __cplusplus
}
#endif

#endif /* __PROGRAM_VECTOR_H */
