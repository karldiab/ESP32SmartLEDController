
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, LED_COUNT, gHue, 7);
}
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(LED_COUNT) ] += CRGB::White;
  }
}
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}



void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, LED_COUNT, 10);
  int pos = random16(LED_COUNT);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, LED_COUNT, 20);
  int pos = beatsin16( 13, 0, LED_COUNT-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < LED_COUNT; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, LED_COUNT, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, LED_COUNT-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
//void pulseColor(int R, int G, int B, int wait) {
//  uint32_t colorDimmest = strip.Color(R/32,G/32,B/32);
//  uint32_t colorDimmeerer = strip.Color(R/16,G/16,B/16);
//  uint32_t colorDimmer = strip.Color(R/8,G/8,B/8);
//  uint32_t colorDim = strip.Color(R/4,G/4,B/4);
//  uint32_t color = strip.Color(R,G,B);
//  uint32_t black = strip.Color(0,0,0);
//  for(int i=0; i<strip.numPixels()+6; i++) { 
//    if (currentDisplayMode != normal) {
//      return;
//    }
//    for (int j=0; j<5;j++) {
//      strip.setPixelColor(i-6, black);
//      strip.setPixelColor(i-5, colorDimmest);
//      strip.setPixelColor(i+5, colorDimmest);  
//      strip.setPixelColor(i-4, colorDimmeerer);
//      strip.setPixelColor(i+4, colorDimmeerer);   
//      strip.setPixelColor(i-3, colorDimmer);
//      strip.setPixelColor(i+3, colorDimmer);  
//      strip.setPixelColor(i-1, colorDim);
//      strip.setPixelColor(i+1, colorDim);
//      strip.setPixelColor(i, color);       
//      strip.show();                          
//      delay(wait);  
//    }
//                         
//  }
//}
