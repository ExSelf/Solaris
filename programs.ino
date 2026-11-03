void Strobe(int i, boolean state) {
  if (((mil + mesh_millis_offset - start_millis) % (2 * i) > i) && state) {
    ledState = true;
  } else {
    ledState = false;
  }

  analogWrite(INNER_PWM_PIN, ledState * bright);
  analogWrite(MIDDLE_PWM_PIN, ledState * bright);
  analogWrite(OUTER_PWM_PIN, ledState * bright);
  analogWrite(HANDS_PWM_PIN, HandsValue(ledState * bright * HANDS_MAX_BRIGHT / 255));
}

void CrossFade(int sinInterval, boolean state) {
  if (state) {
    byte bright1 = 128 - 127 * cos(2 * PI / sinInterval * (mil + mesh_millis_offset - start_millis));
    byte bright2 = 128 - 127 * cos(2 * PI / sinInterval * ((mil + mesh_millis_offset - start_millis) + (sinInterval / 3)));
    byte bright3 = 128 - 127 * cos(2 * PI / sinInterval * ((mil + mesh_millis_offset - start_millis) + (2 * sinInterval / 3)));

    if (debug) Serial.println(bright1);

    analogWrite(INNER_PWM_PIN, bright1 * bright / 255);
    pixelStripWrite(inner, bright1 * bright / 255);

    analogWrite(MIDDLE_PWM_PIN, bright2 * bright / 255);
    pixelStripWrite(middle, bright2 * bright / 255);

    analogWrite(OUTER_PWM_PIN, bright3 * bright / 255);
    pixelStripWrite(outer, bright3 * bright / 255);

    analogWrite(HANDS_PWM_PIN, HandsValue(HANDS_MAX_BRIGHT * bright / 255));

  } else {
    analogWrite(INNER_PWM_PIN, 0);
    analogWrite(MIDDLE_PWM_PIN, 0);
    analogWrite(OUTER_PWM_PIN, 0);
    analogWrite(HANDS_PWM_PIN, HandsValue(0));
  }
}



void VariableCrossFade(int t, unsigned long interval) {
  int p;
  if (start_millis + t > (mil + mesh_millis_offset)) {
    p = interval - interval * (mil + mesh_millis_offset - start_millis) / t;
  } else {
    p = 1;
  }

  CrossFade(40 * p, true);
}


void OnOff(boolean state) {

  byte h = bright * HANDS_MAX_BRIGHT / 255;

  analogWrite(INNER_PWM_PIN, bright * state);
  analogWrite(MIDDLE_PWM_PIN, bright * state);
  analogWrite(OUTER_PWM_PIN, bright * state);
  analogWrite(HANDS_PWM_PIN, HandsValue(h * state));

  pixelStripWrite(inner, bright * state);
  pixelStripWrite(middle, bright * state);
  pixelStripWrite(outer, bright * state);
}

void Breathe(int a, int b) {
  byte breathe_bright;

  long t = (mil + mesh_millis_offset - start_millis);

  if (t <= a) {  // первый вздох
    breathe_bright = 255 * t / a;
  } else if (t <= (2 * a)) {  // первый выдох
    breathe_bright = 255 - (t - a) * 255 / a;
  } else if (t <= (3 * a)) {  // второй вздох
    breathe_bright = 255 * (t - (2 * a)) / a;
  } else if (t <= ((3 * a) + b)) {  // второй длинный выдох
    breathe_bright = 255 - (t - (3 * a)) * 255 / b;
  } else {  // после второго выдоха
    breathe_bright = 0;
    lastprogram = 0;
  }

  analogWrite(INNER_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(MIDDLE_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(OUTER_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(HANDS_PWM_PIN, HandsValue(HANDS_MAX_BRIGHT * bright * breathe_bright / 65025));

  pixelStripWrite(inner, bright * breathe_bright / 255);
  pixelStripWrite(middle, bright * breathe_bright / 255);
  pixelStripWrite(outer, bright * breathe_bright / 255);
}

void BreatheSingle(int a, int b) {
  byte breathe_bright;

  long br = mil + mesh_millis_offset - start_millis;

  if (br <= a) {  // Вдох
    breathe_bright = 255 * br / a;
  } else if (br <= (a + b)) {  // Выдох
    breathe_bright = 255 - (br - a) * 255 / b;
  } else {  // После выдоха
    breathe_bright = 0;
  }

  analogWrite(INNER_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(MIDDLE_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(OUTER_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(HANDS_PWM_PIN, HandsValue(HANDS_MAX_BRIGHT * bright * breathe_bright / 65025));

  pixelStripWrite(inner, bright * breathe_bright / 255);
  pixelStripWrite(middle, bright * breathe_bright / 255);
  pixelStripWrite(outer, bright * breathe_bright / 255);
}

void BreatheHard(int a, int b) {
  byte breathe_bright;

  long br_h = mil + mesh_millis_offset - start_millis;
  if (br_h <= a) {  // Вдох
    breathe_bright = 255 * br_h / a;
    //    if (debug) Serial.println(bright);
  } else if (br_h <= (a + b)) {  // Конец выдоха (1/(x+1-1/t)-t/(t-1))
    float s = 4 * (br_h - a) / b;
    breathe_bright = 127 * (2 - sqrt(s));
    if (debug) {
      Serial.print(br_h - a);
      Serial.print(" - ");
      Serial.print(s);
      Serial.print(" - ");
      Serial.println(bright);
    }
  } else {  // После выдоха
    breathe_bright = 0;
  }

  analogWrite(INNER_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(MIDDLE_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(OUTER_PWM_PIN, bright * breathe_bright / 255);
  analogWrite(HANDS_PWM_PIN, HandsValue(HANDS_MAX_BRIGHT * bright * breathe_bright / 65025));

  pixelStripWrite(inner, bright * breathe_bright / 255);
  pixelStripWrite(middle, bright * breathe_bright / 255);
  pixelStripWrite(outer, bright * breathe_bright / 255);
}

void FadeParameter(byte startvalue, byte endvalue, int t) {
  byte fade_bright;

  unsigned long CurrentMillis = millis();
  if (start_millis + t > CurrentMillis) {
    if (startvalue < endvalue) {
      fade_bright = startvalue + (endvalue - startvalue) * (CurrentMillis - start_millis) / t;
    } else {
      fade_bright = startvalue - (startvalue - endvalue) * (CurrentMillis - start_millis) / t;
    }
  } else {
    fade_bright = endvalue;
  }

  analogWrite(INNER_PWM_PIN, bright * fade_bright / 255);
  analogWrite(MIDDLE_PWM_PIN, bright * fade_bright / 255);
  analogWrite(OUTER_PWM_PIN, bright * fade_bright / 255);
  analogWrite(HANDS_PWM_PIN, HandsValue(HANDS_MAX_BRIGHT * bright * fade_bright / 65025));

  pixelStripWrite(inner, bright * fade_bright / 255);
  pixelStripWrite(middle, bright * fade_bright / 255);
  pixelStripWrite(outer, bright * fade_bright / 255);
}

void FadeToStrobe(int t, unsigned long interval) {
  int p;
  unsigned long CurrentMillis = millis();
  if (start_millis + t > CurrentMillis) {
    p = interval - interval * (CurrentMillis - start_millis) / t;
  } else {
    p = 0;
  }

  Strobe(p, true);
}

void Brightness(byte brightness) {
  analogWrite(INNER_PWM_PIN, brightness * bright / 255);
  analogWrite(MIDDLE_PWM_PIN, brightness * bright / 255);
  analogWrite(OUTER_PWM_PIN, brightness * bright / 255);
  analogWrite(HANDS_PWM_PIN, HandsValue(HANDS_MAX_BRIGHT * bright * brightness / 65025));

  pixelStripWrite(inner, brightness * bright / 255);
  pixelStripWrite(middle, brightness * bright / 255);
  pixelStripWrite(outer, brightness * bright / 255);
}

void pixelStripWrite(Adafruit_NeoPixel &strip, uint8_t brightness) {
  strip.fill(strip.Color(brightness, brightness, brightness), 0, strip.numPixels());
}

uint8_t HandsValue(uint8_t input) {
  uint8_t output = map(input, 0, 255, 255, 0);
  return output;
}
