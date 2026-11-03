void read_voltage() {
  voltage_read++;
  if (voltage_read == NUMBER_OF_READS) voltage_read = 0;
  voltage_buffer[voltage_read] = analogRead(VREF_PIN) * VOLTAGE_INDEXER / 100;

  int total = 0;
  for (byte i = 0; i < NUMBER_OF_READS; i++) {
    total += voltage_buffer[i];
  }
  voltage = total / NUMBER_OF_READS;

  charge = constrain(map(voltage, MIN_VOLTAGE, MAX_VOLTAGE, 0, 100), 0, 100);

  bright = constrain(map(charge, 0, SAFE_CHARGE, MIN_BRIGHT, MAX_BRIGHT), MIN_BRIGHT, MAX_BRIGHT);
}
