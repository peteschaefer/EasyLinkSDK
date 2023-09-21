#include "../sdk/easy_link_c.h"
#include <stdio.h>
#include <stdlib.h>

enum {
  recommended_sdk_version_length = 20,
  recommended_mcu_version_length = 100,
  recommended_ble_version_length = 100
};

int main(void) {
  char sdk_version[recommended_sdk_version_length];
  const size_t sdk_version_length = cl_version(sdk_version);
  printf("[DEBUG] SDK version: %.*s\n", (int)sdk_version_length, sdk_version);

  // Connect to chessboard device with HID.  If the device is not connected,
  // it will automatically connect when the device is plugged into the computer.
  printf("[DEBUG] Connecting to chessboard via HID ...\n");
  if (cl_connect()) {
    printf("Successfully connected to chessboard\n");
  } else {
    printf("[ERROR] Failed to connect to chessboard.  Exiting ...\n");
    return EXIT_FAILURE;
  }

  // MCU
  char mcu_version[recommended_mcu_version_length];
  const size_t mcu_version_length = cl_get_mcu_version(mcu_version);
  if (mcu_version_length > 0) {
    printf("MCU hardware version: %.*s\n", (int)mcu_version_length, mcu_version);
  }

  // BLE (Bluetooth Low Energy)
  char ble_version[recommended_ble_version_length];
  const size_t ble_version_length = cl_get_ble_version(ble_version);
  if (ble_version_length > 0) {
    printf("BLE hardware version: %.*s\n", (int)ble_version_length, ble_version);
  }

  // Battery
  const int battery_level = cl_get_battery();
  if (battery_level >= 0) {
    printf("Battery level: %d%%\n", battery_level);
  }

  printf("Enabling LEDs for squares d5 and e4\n");
  const char *led[8] = {"00000000", "00000000", "00000000", "00010000", "00001000", "00000000", "00000000", "00000000"};
  if (!cl_led(led)) {
    printf("[ERROR] Could not enable/disable LEDs\n");
  }

  const int file_count = cl_get_file_count();
  if (file_count >= 0) {
    printf("Stored game files: %d\n", file_count);
    if (file_count > 0) {
      char f[1024 * 10];
      const int game_file_len = cl_get_file(f, sizeof(f));
      if (game_file_len > 0) {
        printf("Game file content: %.*s\n", game_file_len, f);
      } else if (game_file_len == 0) {
        // Could happen if another device is connected and using
        // the SDK API in parallel?
        printf("No game files available anymore\n");
      } else {
        printf("[ERROR] Could not retrieve game file\n");
      }
    }
  } else {
    printf("[ERROR] Could not retrieve number of stored game files");
  }

  cl_disconnect();
  printf("[DEBUG] Disconnecting from chessboard\n");
  cl_disconnect();
  return EXIT_SUCCESS;
}
