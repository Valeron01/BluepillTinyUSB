/*
 * usb_descriptors.c
 *
 *  Created on: May 8, 2025
 *      Author: elect
 */


#include "tusb.h"

// Device descriptor
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,

    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0xCafe, // Replace with your VID
    .idProduct          = 0x4001, // Replace with your PID
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

// Invoked to return device descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
    return (uint8_t const *) &desc_device;
}

// Configuration Descriptor
enum {
  ITF_NUM_CDC = 0,
  ITF_NUM_CDC_DATA,
  ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)
#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x82

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attributes, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

    // CDC: Interface number, string index, EP notification address and size, EP data out & in address and size
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    (void) index;
    return desc_configuration;
}

// String Descriptors
char const* string_desc_arr[] = {
    (const char[]) {0x09, 0x04}, // 0: English (0x0409)
    "Your Company",              // 1: Manufacturer
    "TinyUSB CDC Device",        // 2: Product
    "123456",                    // 3: Serial
    "TinyUSB CDC Interface"      // 4: CDC
};

static uint16_t _desc_str[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid;
    uint8_t chr_count;

    if ( index == 0 ) {
        _desc_str[1] = 0x0409;
        return (uint16_t const *) _desc_str;
    }

    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];
    chr_count = strlen(str);

    if ( chr_count > 31 ) chr_count = 31;

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);
    for(uint8_t i = 0; i < chr_count; i++) {
        _desc_str[1 + i] = str[i];
    }

    return _desc_str;
}
