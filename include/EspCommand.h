
void espBeginSerial();

bool espCmd(const String command, const String expected, const uint64_t timeout, const bool debug);

bool espCmd(const String command, const String expected, const uint64_t timeout);

bool espCmd(const String command);
