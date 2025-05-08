# BluepillTinyUSB

This repository demonstrates how to run TinyUSB on an STM32F103C6T6 ("Bluepill") board.  
While the setup used here isn't the standard or recommended one, it works well for basic USB communication.  
If you only need USB functionality (without the full feature set of TinyUSB), this guide may be useful.

A step-by-step setup guide is provided below to help you get started from scratch.

---

## Why this setup?

I only needed USB functionality and wanted to avoid unnecessary features or complexity.  
This method strips TinyUSB down to its core components—just enough to get USB up and running on Bluepill.

---

## Setup Steps

### 1. Enable Serial Wire Debugging
In **System Core > Sys > Debug**, set the debug option to **Serial Wire**.  
This makes it easier to flash the STM32 without manually toggling the boot jumper.

---

### 2. Add TinyUSB to Your Project

Clone the TinyUSB repository:

```
tinyusb/src -> {your_project_root}/tinyusb/src
```

---

### 3. Create BSP Folder

Create a folder at:
```
{your_project_root}/tinyusb/bsp
```

Copy the following two files from TinyUSB into the BSP folder:
- `tinyusb/hw/bsp/stm32f1/family.c`
- `tinyusb/hw/bsp/stm32f1/boards/stm32f103_bluepill/board.h`

---

### 4. USB Initialization

Create a file named `board_usb_init.h`.

In this file, define a function, for example:

```c
void board_stm32f1_usb_init(void);
```

Inside this function:
- Copy the clock setup code from `board.h`.
- Copy the USB pin setup and clock enabling code (see this repository for examples).

---

### 5. Configure Your Project in CubeIDE

Go to:
```
Project > Properties > C/C++ General > Paths and Symbols
```

- Add the following **Include paths**:
  - `tinyusb/src`
  - `tinyusb/bsp`

- Add the following **Source folder**:
  - `tinyusb/src`

---

### 6. Add Required Source Files

- Copy `_usb_descriptors.c` from this repository (a simplified version based on TinyUSB examples).
- Copy `tusb_config.h` from this repository as well.

---

### 7. Modify `main.c`

In your `main.c`:

1. Before the main loop, add:

```c
board_stm32f1_usb_init();
tusb_init();
```

2. Inside the main loop, add:

```c
tud_task(); // Must be called regularly to handle USB events
```

---

## Result

TinyUSB should now be fully operational on your Bluepill.

For demonstration, this project echoes back whatever is sent to the Bluepill over USB.

---

## Final Notes

I hope this setup helps others trying to use TinyUSB with STM32F103C6T6.  
Let me know if you run into issues or have suggestions for improving the guide.
