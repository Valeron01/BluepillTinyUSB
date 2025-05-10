/*
 * usb_descriptors.c
 *
 *  Created on: May 8, 2025
 *      Author: elect
 */


#include "tusb.h"

// Microsoft OS Descriptor (WCID 1.0)
#define WCID_DESCRIPTOR_SIZE (sizeof(wcid_descriptor))

// Microsoft OS 1.0 Descriptor
uint8_t const wcid_descriptor[] = {
    0x12,           // Descriptor size (18 bytes)
    0x03,           // Descriptor type: Microsoft OS Descriptor (0x03)
    0x00, 0x01,     // Version 1.0
    0x00, 0x00,     // Reserved
    0x08,           // Total length
    0x00, 0x01,     // Vendor ID (custom)
    0x00, 0x01,     // Product ID (custom)
    0x00,           // Flags
    0x01,           // Number of descriptors
    0x01, 0x00,     // WCID descriptor type
    0x00, 0x01,     // Vendor specific
    0x02, 0x00      // Index of descriptors (always 1)
};


tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,    // USB 2.0
    .bDeviceClass       = TUSB_CLASS_VENDOR_SPECIFIC,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0x1234,    // Vendor ID
    .idProduct          = 0x5678,    // Product ID
    .bcdDevice          = 0x0100,    // Device version
    .iManufacturer      = 0x01,      // String index 1
    .iProduct           = 0x02,      // String index 2
    .iSerialNumber      = 0x03,      // String index 3
    .bNumConfigurations = 0x01
};


uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

uint8_t const * tud_descriptor_get_cb(uint8_t type, uint8_t index) {
    // Handle the WCID descriptor request (GET_DESCRIPTOR 0xEE)
    if (type == TUSB_DESC_DEVICE) {
        return (uint8_t const *) &desc_device;
    }

    if (type == 0xEE) {  // WCID Microsoft OS Descriptor
        return wcid_descriptor;
    }

    return NULL;
}


enum
{
    ITF_NUM_VENDOR,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN    (TUD_CONFIG_DESC_LEN + TUD_VENDOR_DESC_LEN)
#define EPNUM_VENDOR_OUT    0x01
#define EPNUM_VENDOR_IN     0x81

uint8_t const desc_configuration[] =
{
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),
    TUD_VENDOR_DESCRIPTOR(ITF_NUM_VENDOR, 0, EPNUM_VENDOR_OUT, EPNUM_VENDOR_IN, 64)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    (void) index;
    return desc_configuration;
}

char const* string_desc_arr[] =
{
    (const char[]) { 0x09, 0x04 },
    "Valeron01Company",
    "SpecificUSBDevice",
    "123456",
};

static uint16_t _desc_str[32];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid;
    uint8_t chr_count;

    if ( index == 0 ) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        const char* str = string_desc_arr[index];
        chr_count = strlen(str);
        if ( chr_count > 31 ) chr_count = 31;
        for ( uint8_t i = 0; i < chr_count; i++ ) {
            _desc_str[1 + i] = str[i];
        }
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2 * chr_count + 2);
    return _desc_str;
}
