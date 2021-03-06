#include "Patch.h"
#include "ScreenBuffer.h"

#define	BLACK           0
#define	WHITE           1

typedef uint8_t Colour;
typedef ScreenBuffer<uint8_t, BLACK, WHITE> MonochromeScreenBuffer;

/**
 * Abstract base class for patches that use a monochrome screen.
 * Compatible OLED drivers: SSD1309
 */
class MonochromeScreenPatch : public Patch {
public:
  MonochromeScreenPatch();
  virtual ~MonochromeScreenPatch();
  uint16_t getScreenWidth();
  uint16_t getScreenHeight();
  virtual void processScreen(MonochromeScreenBuffer& screen) = 0;
};
