#include <Arduino.h>
#include <FastLED.h>

#define LED_COUNT 29
#define DATA_PIN 11
#define CLOCK_PIN 13
#define BUTTON_PIN 12

constexpr uint8_t BRIGHTNESS = 100;
constexpr uint32_t EFFECT_DURATION_MS = 5000;
constexpr uint32_t BUTTON_DEBOUNCE_MS = 30;
constexpr uint16_t FRAME_DELAY_MS = 20;

CRGB leds[LED_COUNT];
uint8_t heat[LED_COUNT] = {};

enum Effect : uint8_t {
  SOLID_COLOR,
  COLOR_WIPE,
  CHASE,
  TWINKLE,
  BREATHING,
  METEOR,
  FIRE,
  PALETTE,
  RAINBOW,
  EFFECT_COUNT
};

Effect currentEffect = SOLID_COLOR;
uint32_t effectStartedAt = 0;
uint8_t animationStep = 0;
bool lastButtonReading = HIGH;////////////////////////
bool stableButtonState = HIGH;
uint32_t buttonChangedAt = 0;

void renderEffect(uint32_t now) {
  const uint32_t elapsed = now - effectStartedAt;

  switch (currentEffect) {
    case SOLID_COLOR:
      fill_solid(leds, LED_COUNT, CRGB::Purple);
      break;

    case COLOR_WIPE: {
      fill_solid(leds, LED_COUNT, CRGB::Black);
      const uint8_t litCount =
          static_cast<uint8_t>((elapsed * LED_COUNT) / EFFECT_DURATION_MS + 1);
      fill_solid(leds, litCount, CRGB::Orange);
      break;
    }

    case CHASE:
      fill_solid(leds, LED_COUNT, CRGB::Black);
      for (uint8_t offset = 0; offset < 3; ++offset) {
        const uint8_t position = (animationStep + offset * 5) % LED_COUNT;
        leds[position] = CHSV(animationStep * 8, 255, 255);
      }
      break;

    case TWINKLE:
      fadeToBlackBy(leds, LED_COUNT, 35);
      if (random8() < 100) {
        leds[random8(LED_COUNT)] = CHSV(random8(), 180, 255);
      }
      break;

    case BREATHING: {
      const uint8_t breath = beatsin8(12, 12, 255);
      fill_solid(leds, LED_COUNT, CHSV(160, 255, breath));
      break;
    }

    case METEOR: {
      fadeToBlackBy(leds, LED_COUNT, 45);
      const uint8_t position = animationStep % LED_COUNT;
      leds[position] = CRGB::White;
      if (position > 0) {
        leds[position - 1] = CRGB(80, 80, 80);
      }
      if (position > 1) {
        leds[position - 2] = CRGB(25, 25, 25);
      }
      break;
    }

    case FIRE:
      for (uint8_t i = 0; i < LED_COUNT; ++i) {
        heat[i] = qsub8(heat[i], random8(0, 40));
      }
      for (int i = LED_COUNT - 1; i >= 2; --i) {
        heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
      }
      if (random8() < 120) {
        const uint8_t spark = random8(LED_COUNT < 7 ? LED_COUNT : 7);
        heat[spark] = qadd8(heat[spark], random8(160, 255));
      }
      for (uint8_t i = 0; i < LED_COUNT; ++i) {
        leds[i] = ColorFromPalette(HeatColors_p, heat[i]);
      }
      break;

    case PALETTE:
      for (uint8_t i = 0; i < LED_COUNT; ++i) {
        leds[i] = ColorFromPalette(RainbowColors_p, i * 16 + animationStep);
      }
      break;

    case RAINBOW:
      fill_rainbow(leds, LED_COUNT, animationStep, 16);
      break;

    case EFFECT_COUNT:
      break;
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<HD107S, DATA_PIN, CLOCK_PIN, RGB>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  random16_add_entropy(analogRead(A0));
}

void loop() {
  const uint32_t now = millis();
  const bool buttonReading = digitalRead(BUTTON_PIN);
  if (buttonReading != lastButtonReading) {
    lastButtonReading = buttonReading;
    buttonChangedAt = now;
  }

  if (now - buttonChangedAt >= BUTTON_DEBOUNCE_MS &&
      buttonReading != stableButtonState) {
    stableButtonState = buttonReading;
    if (stableButtonState == LOW) {
      currentEffect = static_cast<Effect>((currentEffect + 1) % EFFECT_COUNT);
      effectStartedAt = now;
      animationStep = 0;
      fill_solid(leds, LED_COUNT, CRGB::Black);
      for (uint8_t i = 0; i < LED_COUNT; ++i) {
        heat[i] = 0;
      }
    }
  }

  renderEffect(now);
  FastLED.show();
  ++animationStep;
  delay(FRAME_DELAY_MS);
}
