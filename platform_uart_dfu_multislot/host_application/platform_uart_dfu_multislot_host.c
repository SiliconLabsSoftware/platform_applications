/***************************************************************************//**
 * @file
 * @brief Platform UART DFU Multislot Host Application
 *******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#define MESSAGE_SIZE    64
#define READ_BUF_SIZE   80

typedef enum {
  CMD_TYPE_DOWNLOAD_FW = 0,
  CMD_TYPE_UPGRADE_FW,
  CMD_TYPE_INVALID,
}cmd_type_t;

/*******************************************************************************
 ***************************  LOCAL VARIABLES   ********************************
 ******************************************************************************/

static char read_buf[READ_BUF_SIZE];

static DCB deviceConfig =
{ sizeof (DCB), CBR_115200, TRUE, FALSE, TRUE, FALSE, DTR_CONTROL_DISABLE,
  FALSE, TRUE, FALSE, FALSE, FALSE, FALSE,
  RTS_CONTROL_HANDSHAKE,
  0, 0, 0, 0, 8, NOPARITY, ONESTOPBIT, };

static HANDLE hComPort;

/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/
static cmd_type_t process_command_line(int argc, char **argv)
{
  cmd_type_t cmd_type;

  if ((argc == 5)
      && !strcmp(argv[1],
                 "-d") && (!strcmp(argv[3], "0") || !strcmp(argv[3], "1"))) {
    cmd_type = CMD_TYPE_DOWNLOAD_FW;
  } else if ((argc == 4)
             && !strcmp(argv[1],
                        "-u")
             && (!strcmp(argv[2], "0") || !strcmp(argv[2], "1"))) {
    cmd_type = CMD_TYPE_UPGRADE_FW;
  } else {
    cmd_type = CMD_TYPE_INVALID;
  }

  return cmd_type;
}

static void open_com_port(char *port)
{
  hComPort = CreateFileA(port,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         0,
                         NULL);

  if (hComPort == INVALID_HANDLE_VALUE) {
    printf("Error opening serial port\n");
    exit(1);
  } else {
    printf("Serial port opened successfully\n");
  }
}

static int read_com_port(HANDLE handle, void *ptr, int size)
{
  DWORD bytes_read;
  char *p = ptr;
  while (size > 0)
  {
    ReadFile(handle, p, size, &bytes_read, FALSE);
    size -= bytes_read;
    p += bytes_read;
  }
  return 0;
}

static int write_com_port(HANDLE handle, void *ptr, int size)
{
  DWORD bytes_written;

  if (ptr == 0) {
    return 0;
  }
  WriteFile(handle, ptr, size, &bytes_written, FALSE);
  if (bytes_written != size) {
    printf("%d bytes not written\n", size - bytes_written);
  }
  return bytes_written != size;
}

static void print_buf(uint8_t num)
{
  for (uint8_t i = 0; i < num; i++) {
    printf("%c", read_buf[i]);
  }
  printf("\n");
}

static void send_start_string(uint8_t slot)
{
  uint8_t msg0[3] = { 49, 0, 10 };
  uint8_t msg1[3] = { 49, 1, 10 };

  switch (slot) {
    case 0:
      printf("Sending start string\n");
      write_com_port(hComPort, msg0, 3);
      read_com_port(hComPort, read_buf, 2);
      break;
    case 1:
      printf("Sending start string\n");
      write_com_port(hComPort, msg1, 3);
      read_com_port(hComPort, read_buf, 2);
      break;
    default:
      break;
  }
}

static void send_gbl_file(const char *filename)
{
  uint8_t cmd_buf[2];
  uint8_t data_buf[MESSAGE_SIZE];
  uint32_t size, offset = 0;
  uint8_t current_size;
  uint32_t current_count = 0;

  FILE *f;

  cmd_buf[1] = '\n';

  f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Error opening firmware file %s\n", filename);
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  size = ftell(f);
  fseek(f, 0, SEEK_SET);

  printf("FW Image Size is %d bytes\n", size);
  printf("Sending FW Image...\n");

  while (size > 0) {
    if (size >= MESSAGE_SIZE) {
      current_size = MESSAGE_SIZE;
    } else {
      current_size = size;
    }

    fread(data_buf, 1, current_size, f);

    current_count++;

    cmd_buf[0] = current_size;
    write_com_port(hComPort, cmd_buf, 2);
    read_com_port(hComPort, read_buf, 2);

    write_com_port(hComPort, data_buf, current_size);
    read_com_port(hComPort, read_buf, 2);

    size -= current_size;
  }

  cmd_buf[0] = 255;
  write_com_port(hComPort, cmd_buf, 2);
  read_com_port(hComPort, read_buf, 2);
  printf("FW Image Downloaded to Device\n");

  read_com_port(hComPort, read_buf, 2);
  printf("FW Image Stored to Flash\n");

  fclose(f);
}

static uint8_t get_slot(char *p)
{
  if (!strcmp(p, "0")) {
    return 0;
  } else if (!strcmp(p, "1")) {
    return 1;
  } else {
    return -1;
  }
}

static void process_firmware_download(char **argv)
{
  const char *filename = argv[2];
  uint8_t slot = get_slot(argv[3]);

  open_com_port(argv[4]);

  send_start_string(slot);
  send_gbl_file(filename);
}

static void process_firmware_upgrade(char **argv)
{
  uint8_t msg0[3] = { 50, 0, 10 };
  uint8_t msg1[3] = { 50, 1, 10 };
  uint8_t slot = get_slot(argv[2]);

  open_com_port(argv[3]);

  switch (slot) {
    case 0:
      printf("Sending upgrade string\n");
      write_com_port(hComPort, msg0, 3);
      read_com_port(hComPort, read_buf, 2);
      break;
    case 1:
      printf("Sending upgrade string\n");
      write_com_port(hComPort, msg1, 3);
      read_com_port(hComPort, read_buf, 2);
      break;
    default:
      break;
  }

  print_buf(2);
}

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/
int main(int argc, char **argv)
{
  cmd_type_t cmd_type;

  cmd_type = process_command_line(argc, argv);

  switch (cmd_type) {
    case CMD_TYPE_DOWNLOAD_FW: {
      printf("Command Type: FW Download\n");
      process_firmware_download(argv);
      break;
    }
    case CMD_TYPE_UPGRADE_FW: {
      printf("Command Type: FW Upgrade\n");
      process_firmware_upgrade(argv);
      break;
    }
    case CMD_TYPE_INVALID: {
      printf("Command Type Invalid\n");
      exit(1);
      break;
    }
    default:
      break;
  }

  return 0;
}
