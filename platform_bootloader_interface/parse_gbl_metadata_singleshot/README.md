# Platform - Parse GBL Metadata in Single Shot

![Type badge](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/SiliconLabs/application_examples_ci/master/platform_applications/parse_gbl_metadata_singleshot_common.json&label=Type&query=type&color=green)
![Technology badge](https://img.shields.io/badge/Technology-Platform-green)
![License badge](https://img.shields.io/badge/License-Zlib-green)
![SDK badge](https://img.shields.io/badge/SDK-v4.5.0-green)
![Build badge](https://img.shields.io/badge/Build-passing-green)
![Flash badge](https://img.shields.io/badge/Flash-22.33%20KB-blue)
![RAM badge](https://img.shields.io/badge/RAM-5.16%20KB-blue)

## Description

This example demonstrates how to use the Application Parser Interface described in UG266. The GBL image in the storage slot is first verified and the metadata in the stored image is parsed in a single shot using the image parser function of the gecko bootloader interface API.

## SDK version

- [Gecko SDK v4.5.0](https://github.com/SiliconLabs/gecko_sdk/releases/tag/v4.5.0)

## Hardware Required

[EFR32MG12 2.4GHz 19 dBm Radio Board - BRD4161A](https://www.silabs.com/documents/public/user-guides/ug260-brd4161a-user-guide.pdf)

[EFR32MG13 2.4 GHz 915 MHz +19 dBm - BRD4158A](https://www.silabs.com/development-tools/wireless/zigbee/slwrb4158a-efr32mg13-radio-board)

## Connections Required

- Connect the radio board to the WSTK, and connect the WSTK via a micro-USB cable to your PC.

## Setup

To test this application, you can either create a project based on an example project or start with an "Empty C Project" project based on your hardware.

### Create a project based on an example project

1. From the Launcher Home, add your hardware to My Products, click on it, and click on the **EXAMPLE PROJECTS & DEMOS** tab. Find the example project with filter "gbl".

2. Click the **Create** button on the **Platform - Parse GBL Metadata in Single Shot** example. The example project creation dialog pops up -> click Create and Finish and the project should be generated.
![create_project](image/create_project.png)

3. Build and flash this example to the board.

### Start with an "Empty C Project" project

1. Create an **Empty C Project** project for your device using Simplicity Studio 5.

2. Copy all attached files in *inc* and *src* folders into the project root folder.

3. Open the .slcp file. Select the SOFTWARE COMPONENTS tab and install the software components:

- [Platform] > [Bootloader] > [Bootloader Application Interface]
- [Platform] > [Bootloader] > [App Properties] → Navigate to the config file and change the value of  `SL_APPLICATION_SIGNATURE` to `APPLICATION_SIGNATURE_ECDSA_P256` and `SL_APPLICATION_SIGNATURE_LOCATION` to `0`
- [Platform] → [Board] → [Board Control]: enable **Enable Virtual COM UART**
- [Services] → [IO Stream] → [Driver] → [IO Stream: USART] → default instance name: vcom
- [Services] → [IO Stream] → [Driver] → [IO Stream: Retarget STDIO]
- [Services] → [IO Stream] → [Driver] → [IO Stream: STDLIB Configuration]
- [Services] → [Power Manager] → [Power Manager: Deepsleep]

## How It Works

In this example, the encrypted/signed GBL image is stored in slot0. When PB1 is pressed, LED1 turns on, the GBL Image in slot0 is verified, decrypted/parsed in a single shot and raw metadata is transmitted over USART0(VCOM).

**Below are the steps to perform the test:**

1. Perform erase on the device before starting the test. Below is the Simplicity Commander CLI command:

   Run: `commander device pageerase --region @mainflash --region @userdata <--region @bootloader> <--region @lockbits>`

   Note: Series 2 devices don't have a dedicated bootloader and lockbits area. Also EFx32xG1 devices don't have a dedicated bootloader area.

2. Create a bootloader-storage-internal-single project and enable security features i.e., signing, encryption and secureboot

3. Build the bootloader-storage-internal-single project and flash the bootloader project at the start address of the bootloader area

   Run: `commander flash <bootloader_project>.s37 --address <start_address>`

4. Generate signing keys

   Run: `commander gbl keygen --type ecc-p256 --outfile signing-key`

5. Generate encryption key

    Run: `commander gbl keygen --type aes-ccm --outfile encryption-key`

6. Write the encryption key and public key to the EFR32

   Run: `commander flash --tokengroup znet --tokenfile encryption-key --tokenfile signing-key-tokens.txt`

7. Create and build the parse_gbl_metadata_singleshot project to generate .s37/.hex files

8. Sign the application image

   Run: `commander convert <application_project>.s37 --secureboot --keyfile signing-key --outfile <application_project_signed>.s37`

9. Flash the signed application image at the start address of the application area

   Run: `commander flash <application_project_signed>.s37 --address <start_address>`

10. Create a signed and encrypted GBL file using the metadata binary file

    Run: `commander gbl create <waveform_metadata>.gbl -- metadata <waveform_test1>.bin --sign signing-key --encrypt encryption-key`

11. Flash *waveform_metadata*.gbl at the start address of *Slot0*

    Run: `commander flash <waveform_metadata>.gbl.bin --address <slot0_start_address>`

    Note: To flash a GBL image, add the .bin extension to the GBL file. To perform OTA/OTW upgrade, the .bin extension is not needed.

12. Connect the radio board to a terminal program (115200 8N1) and press PB1 for GBL image verification and parsing. The status of the application can be seen in the terminal application.

    ![log](image/log.png)

**Resources:**

- For more details about the gecko bootloader, refer to UG266.
- For more details about Simplicity Commander CLI, refer to UG162.

## Porting to Another EFx32 Series 1 or Series 2 Device

Apart from any issues of pin availability on a given radio board, this code should run as-is on any Series 1 or Series 2 radio board having gecko bootloader support.
