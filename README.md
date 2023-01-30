# apex-seraph
Seraph is a Cheating-Software for the battle royale game Apex Legends.
It uses multiple system-privilege levels (Ring 0 and 3)

# Requirements

## The Hardware ##
1. A [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)

![image](https://user-images.githubusercontent.com/49228523/151983408-e43c0f48-a46e-4437-8ad2-87fafc2d470e.png)

2. Seraph requires you to have the latest Windows10 Version

3. Visual Studio including everything for C++ and WDK (Windows Driver Development Kit) to compile the Projects.

# It consists of 4 components: #

# [Usermode Application](https://github.com/Jiingz/apex-seraph/tree/main/sources/core)

Which is the main cheat itself, containing all the logic and reverse engineering work including offsets.
It also communicates with the Teensy through serial-port-communication.

# [Kernel Driver](https://github.com/Jiingz/apex-seraph/tree/main/sources/kernelmode_server)

The kernelmode driver uses [MmCopyVirtualMemory](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ntddk/nf-ntddk-mmcopymemory) which gets a pointer to the Windows-Internal [EPROCESS](https://www.nirsoft.net/kernel_struct/vista/EPROCESS.html) structure through [PsLookupProcessByProcessId](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-pslookupprocessbyprocessid)

Since Easy Anti-Cheat detects IOCTL communication to drivers which are manually mapped (pointing to invalid driver device in memory) , socket communication was used.

Although it is generally considered slower than IOCTL, the performance was great.

**IMPORTANT** 
In order to use the Driver, you need to load it somehow (WINDOWS TEST MODE DOES NOT WORK WITH EAC!).
I would advice to map it cleanly, since that's the way I did. There are still no bans so far for about a half year now.

# [The Loader](https://github.com/Jiingz/apex-seraph/tree/main/sources/loader) #
Including a Login System (You need to implement it your own, I've used Cryptolens) and settings for every Feature.
![menu](https://user-images.githubusercontent.com/49228523/155356531-75307d7e-799f-4df6-ad5d-c2b17e0eb926.jpeg)


# [The Hardware](https://github.com/Jiingz/apex-seraph/blob/main/sources/Hardware/serial_connection/serial_connection.ino)

We use the Teensy 4.1 to move the mouse(to aim). There are still detection vectors in here since the Teensy emulates
a real mouse. Although having multiple mouses attached isn't a DT vector, playing with multiple is, especially when the device is a Teensy.

I did not investigate if EAC flags for doing so, but they definetly don't, and probably never will ban for it.

## How does it work? ##

This Diagram was **NOT** done before writing this, I quickly created it to make my explanations more clear.
If you are really interested into this project, I would recommend to read everything.

![Diagram](https://user-images.githubusercontent.com/49228523/151992550-3724f8ca-5023-449d-9ca2-131f5451fd81.png)

The Usermode Application sends a [PacketCopyMemory](https://github.com/Jiingz/apex-seraph/blob/main/sources/core/driver/server_shared.h#L15) structure as packet to pass info to the Kernel Driver.

Once received, the kernel driver reads the information in the game process and returns that info to the usermode application, which executes the logic
and communicates with the Teensy (as described below) for aiming.

The Teensy receives a array of size which holds the X and Y coordinates where the aimbot should aim at. [Communication Function](https://github.com/Jiingz/apex-seraph/blob/main/sources/core/hardware/serial_controller.cpp#L9)
The serial port sends the data sequential, so we can handle the values [sequential](https://github.com/Jiingz/apex-seraph/blob/main/sources/Hardware/serial_connection/serial_connection.ino#L17) aswell since we've never received an array. Instead we receive the values in a sequential order.


# Features #

## Aimbot ##
The Aimbot is smooth, and has the option to be smoothed out to a point where no one will recognize it.

## Glow ##
Glowing enemys through walls (or not), including the option to choose between multiple [Glow Contexts](https://github.com/Jiingz/apex-seraph/blob/main/sources/core/game/structs.h#L8)

## Why am I releasing this? ##

EDIT: It's an old, unused source :)
