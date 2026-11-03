void MIDI() {

  switch (command) {

    /////////////////////////////// MIDI ///////////////////////////////

    // Выкл
    case 12:
      OnOff(false);
      break;

    // Вкл
    case 13:
      OnOff(true);
      break;

    // Strobe
    case 14:
      Strobe(interval, true);
      break;

    // Cross-Fade
    case 15:
      CrossFade(sinInterval, true);
      break;

    // Вздохи
    case 16:
      Breathe(breatheint1, breatheint2);
      break;

    // Вздох
    case 17:
      BreatheSingle(breatheint1, breatheint2);
      break;

    // Вздох (от параметра)
    case 18:
      BreatheSingle(breatheint1, 40 * parameter);
      break;

    // Strobe (от параметра)
    case 19:
      Strobe(parameter, true);
      break;

    // FadeOut (от параметра)
    case 20:
      FadeParameter(255, 0, 40 * parameter);
      break;

    // FadeIn (от параметра)
    case 21:
      FadeParameter(0, 255, 40 * parameter);
      break;

    // Cross-Fade (от параметра)
    case 22:
      CrossFade(40 * parameter, true);
      break;

    // Сложный вздох (от параметра)
    case 23:
      BreatheHard(breatheint1, constant_commands[0]);
      break;

    // Разрастающийся стробоскоп (от параметра)
    case 24:
      FadeToStrobe(100 * parameter, constant_commands[0] * 10);
      break;

    // Ускоряющийся Cross-Fade
    case 25:
      VariableCrossFade(50 * parameter, constant_commands[0]);
      break;

    // Яркость костюма (от параметра)
    case 26:
      {
        byte b = map(parameter, 1, 127, 0, 255);
        Brightness(b);
        break;
      }

    // Fade с 0 до 32 (от параметра)
    case 27:
      FadeParameter(0, 32, 40 * parameter);
      break;

    // Fade с 255 до 32 (от параметра)
    case 28:
      FadeParameter(255, 32, 40 * parameter);
      break;

    // Fade с 32 до 0 (от параметра)
    case 29:
      FadeParameter(32, 0, 40 * parameter);
      break;

    // Fade с 32 до 255 (от параметра)
    case 30:
      FadeParameter(32, 255, 40 * parameter);
      break;

    case 31:
      FadeParameter(4 * (parameter - 48), 1, 250);
      break;

    // Fade с 32 до 0 (от параметра)
    case 32:
      FadeParameter(6, 0, 40 * parameter);
      break;

    case 40:
      analogWrite(HANDS_PWM_PIN, parameter * 2);
      break;

    default:
      Strobe(interval, true);
      break;
  }

  inner.show();
  middle.show();
  outer.show();
}


void OTA() {
  // ota_mode = true;

  // WifiEspNowBroadcast.end();

  // WiFi.softAPdisconnect(true);
  // WiFi.setAutoReconnect(true);

  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, pass);

  // String host = "Solaris " + String(NUM);
  // ArduinoOTA.setHostname(host.c_str());
  // ArduinoOTA.begin();
}
