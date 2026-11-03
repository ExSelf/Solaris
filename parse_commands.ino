unsigned long command_millis;

void parse_commands() {
  byte numBytes = Udp.parsePacket();
  if ( numBytes ) {
    byte packetBuffer[22];
    Udp.read(packetBuffer, numBytes); // read the packet into the buffer

    if (debug) {
      for (byte p = 0; p < 22; p++) {
        Serial.print(packetBuffer[p]);
        Serial.print(" - ");
      }
      Serial.println();
    }

    unsigned long  server_time = (packetBuffer[7] << 24) + (packetBuffer[6] << 16) + (packetBuffer[5] << 8) + packetBuffer[4];
    unsigned long command_time = (packetBuffer[3] << 24) + (packetBuffer[2] << 16) + (packetBuffer[1] << 8) + packetBuffer[0];

    if (command_time != command_millis) {
      command_millis = command_time;

      start_millis = mil + command_millis - server_time;
      command = packetBuffer[20];
      parameter = packetBuffer[21];

    }

    for (byte c = 0; c < 12; c++) {
      constant_commands[c] = packetBuffer[c + 8];
    }
  }
}
