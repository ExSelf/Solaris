void sendMessage() {
  //  unsigned long global_time = mil + mesh_millis_offset;

  byte msg[22];
  msg[0] = NUM + GROUP_NUM;
  msg[1] = command_millis & 255;  // command_timestamp
  msg[2] = (command_millis >> 8) & 255;
  msg[3] = (command_millis >> 16) & 255;
  msg[4] = (command_millis >> 24) & 255;
  msg[5] = voltage & 255;
  msg[6] = (voltage >> 8) & 255;
  for (byte c = 0; c < 12; c++) {
    msg[c + 7] = constant_commands[c];
  }

  msg[19] = command;
  msg[20] = parameter;
  msg[21] = charge;

#ifdef ESP32
  Udp.beginPacket(ipAnswer, port);
  Udp.write(msg, 22);
  Udp.endPacket();
#elif ESP8266
  Udp.beginPacketMulticast(ipAnswer, 6000, WiFi.localIP());
  Udp.write(msg, 22);
  Udp.endPacket();
#endif


  //  WifiEspNowBroadcast.send(reinterpret_cast<const uint8_t*>(msg), sizeof(msg));
}
