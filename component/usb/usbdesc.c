/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbdesc.c
 * Purpose: USB Descriptors
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Changed string descriptor handling
 *                Changed HID Report Descriptor
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbdesc.h"

#include "hid.h"
#include "cdc.h"

// USB Standard Device Descriptor
const U8 USB_DeviceDescriptor[] = {
	USB_DEVICE_DESC_SIZE,              // bLength
	USB_DEVICE_DESCRIPTOR_TYPE,        // bDescriptorType
	WBVAL(0x0200), // 2.00             // bcdUSB
	0xEF,                              // bDeviceClass
	0x02,                              // bDeviceSubClass
	0x01,                              // bDeviceProtocol
	USB_MAX_PACKET0,                   // bMaxPacketSize0
	WBVAL(0x0471),                     // idVendor
	WBVAL(0x0661),                     // idProduct
	WBVAL(0x0100), // 1.00             // bcdDevice
	0x01,                              // iManufacturer
	0x02,                              // iProduct
	0x00,                              // iSerialNumber
	0x01                               // bNumConfigurations: one possible configuration
};

// USB Configuration Descriptor
//   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
const U8 USB_ConfigDescriptor[] = {
// Configuration 1
	USB_CONFIGUARTION_DESC_SIZE,       // bLength
	USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType
	WBVAL(                             // wTotalLength
		USB_CONFIGUARTION_DESC_SIZE +

		0x08                        +  // IAD
		USB_INTERFACE_DESC_SIZE     +  // Communication interface
		0x0013                      +  // CDC functions
		USB_ENDPOINT_DESC_SIZE      +  // Interrupt endpoint
		USB_INTERFACE_DESC_SIZE     +  // Data interface
		2*USB_ENDPOINT_DESC_SIZE    +  // Bulk endpoints

		0x08                        +  // IAD
		USB_INTERFACE_DESC_SIZE     +  // Communication interface
		0x0013                      +  // CDC functions
		USB_ENDPOINT_DESC_SIZE      +  // Interrupt endpoint
		USB_INTERFACE_DESC_SIZE     +  // Data interface
		2*USB_ENDPOINT_DESC_SIZE    +  // Bulk endpoints
		
		0x08                        +  // IAD
		USB_INTERFACE_DESC_SIZE     +  // Communication interface
		0x0013                      +  // CDC functions
		USB_ENDPOINT_DESC_SIZE      +  // Interrupt endpoint
		USB_INTERFACE_DESC_SIZE     +  // Data interface
		2*USB_ENDPOINT_DESC_SIZE       // Bulk endpoints
	),
	0x06,                              // bNumInterfaces
	0x01,                              // bConfigurationValue: 0x01 is used to select this configuration
	0x00,                              // iConfiguration: no string to describe this configuration
	USB_CONFIG_BUS_POWERED,            // bmAttributes
	USB_CONFIG_POWER_MA(100),          // bMaxPower, device power consumption is 100 mA

		0x08,   // bLength: Interface Descriptor size
		USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,  // bDescriptorType: IAD
		0x00,   // bFirstInterface
		0x02,   // bInterfaceCount
		USB_DEVICE_CLASS_COMMUNICATIONS,    // bFunctionClass: CDC
		0x00,   // bFunctionSubClass
		0x00,   // bFunctionProtocol
		0x00,   // iFunction

// Interface 0, Alternate Setting 0, CDC Class
	USB_INTERFACE_DESC_SIZE,           // bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,     // bDescriptorType
	0x00,                              // bInterfaceNumber: Number of Interface
	0x00,                              // bAlternateSetting: Alternate setting
	0x01,                              // bNumEndpoints: One endpoint used
	CDC_COMMUNICATION_INTERFACE_CLASS, // bInterfaceClass: Communication Interface Class
	CDC_ABSTRACT_CONTROL_MODEL,        // bInterfaceSubClass: Abstract Control Model
	0x00,                              // bInterfaceProtocol: no protocol used
	0x00,                              // iInterface:
// Header Functional Descriptor
	0x05,                              // bLength: Endpoint Descriptor size
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_HEADER,                        // bDescriptorSubtype: Header Func Desc
	WBVAL(CDC_V1_10), // 1.10          // bcdCDC
// Call Management Functional Descriptor
	0x05,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_CALL_MANAGEMENT,               // bDescriptorSubtype: Call Management Func Desc
	0x00,                              // bmCapabilities: device handles call management
	0x02,                              // bDataInterface: CDC data IF ID
// Abstract Control Management Functional Descriptor
	0x04,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_ABSTRACT_CONTROL_MANAGEMENT,   // bDescriptorSubtype: Abstract Control Management desc
	0x02,                              // bmCapabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
// Union Functional Descriptor
	0x05,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_UNION,                         // bDescriptorSubtype: Union func desc
	0x00,                              // bMasterInterface: Communication class interface is master
	0x01,                              // bSlaveInterface0: Data class interface is slave 0

// Endpoint, EP1 Interrupt IN          // event notification (optional)
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_IN(1),                // bEndpointAddress
	USB_ENDPOINT_TYPE_INTERRUPT,       // bmAttributes
	WBVAL(0x0010),                     // wMaxPacketSize
	0xff,                              // bInterval

// Interface 1, Alternate Setting 0, Data class interface descriptor
	USB_INTERFACE_DESC_SIZE,           // bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,     // bDescriptorType
	0x01,                              // bInterfaceNumber: Number of Interface
	0x00,                              // bAlternateSetting: no alternate setting
	0x02,                              // bNumEndpoints: two endpoints used
	CDC_DATA_INTERFACE_CLASS,          // bInterfaceClass: Data Interface Class
	0x00,                              // bInterfaceSubClass: no subclass available
	0x00,                              // bInterfaceProtocol: no protocol used
	0x00,                              // iInterface

// Endpoint, EP2 Bulk OUT
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_OUT(2),               // bEndpointAddress
	USB_ENDPOINT_TYPE_BULK,            // bmAttributes
	WBVAL(USB_CDC_BUFSIZE),            // wMaxPacketSize
	0x00,                              // bInterval: ignore for Bulk transfer

// Endpoint, EP2 Bulk IN
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_IN(2),                // bEndpointAddress
	USB_ENDPOINT_TYPE_BULK,            // bmAttributes
	WBVAL(USB_CDC_BUFSIZE),            // wMaxPacketSize
	0x00,                              // bInterval: ignore for Bulk transfer

		0x08,   // bLength: Interface Descriptor size
		USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,  // bDescriptorType: IAD
		0x02,   // bFirstInterface
		0x02,   // bInterfaceCount
		USB_DEVICE_CLASS_COMMUNICATIONS,    // bFunctionClass: CDC
		0x00,   // bFunctionSubClass
		0x00,   // bFunctionProtocol
		0x00,   // iFunction

// Interface 2, Alternate Setting 0, CDC Class
	USB_INTERFACE_DESC_SIZE,           // bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,     // bDescriptorType
	0x02,                              // bInterfaceNumber: Number of Interface
	0x00,                              // bAlternateSetting: Alternate setting
	0x01,                              // bNumEndpoints: One endpoint used
	CDC_COMMUNICATION_INTERFACE_CLASS, // bInterfaceClass: Communication Interface Class
	CDC_ABSTRACT_CONTROL_MODEL,        // bInterfaceSubClass: Abstract Control Model
	0x00,                              // bInterfaceProtocol: no protocol used
	0x00,                              // iInterface:
// Header Functional Descriptor
	0x05,                              // bLength: Endpoint Descriptor size
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_HEADER,                        // bDescriptorSubtype: Header Func Desc
	WBVAL(CDC_V1_10), // 1.10          // bcdCDC
// Call Management Functional Descriptor
	0x05,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_CALL_MANAGEMENT,               // bDescriptorSubtype: Call Management Func Desc
	0x00,                              // bmCapabilities: device handles call management
	0x02,                              // bDataInterface: CDC data IF ID
// Abstract Control Management Functional Descriptor
	0x04,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_ABSTRACT_CONTROL_MANAGEMENT,   // bDescriptorSubtype: Abstract Control Management desc
	0x02,                              // bmCapabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
// Union Functional Descriptor
	0x05,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_UNION,                         // bDescriptorSubtype: Union func desc
	0x02,                              // bMasterInterface: Communication class interface is master
	0x03,                              // bSlaveInterface0: Data class interface is slave 0

// Endpoint, EP3 Interrupt IN          // event notification (optional)
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_IN(3),                // bEndpointAddress
	USB_ENDPOINT_TYPE_INTERRUPT,       // bmAttributes
	WBVAL(0x0010),                     // wMaxPacketSize
	0xff,                              // bInterval

// Interface 3, Alternate Setting 0, Data class interface descriptor
	USB_INTERFACE_DESC_SIZE,           // bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,     // bDescriptorType
	0x03,                              // bInterfaceNumber: Number of Interface
	0x00,                              // bAlternateSetting: no alternate setting
	0x02,                              // bNumEndpoints: two endpoints used
	CDC_DATA_INTERFACE_CLASS,          // bInterfaceClass: Data Interface Class
	0x00,                              // bInterfaceSubClass: no subclass available
	0x00,                              // bInterfaceProtocol: no protocol used
	0x00,                              // iInterface

// Endpoint, EP4 Bulk OUT
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_OUT(4),               // bEndpointAddress
	USB_ENDPOINT_TYPE_BULK,            // bmAttributes
	WBVAL(USB_CDC_BUFSIZE),            // wMaxPacketSize
	0x00,                              // bInterval: ignore for Bulk transfer

// Endpoint, EP4 Bulk IN
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_IN(4),                // bEndpointAddress
	USB_ENDPOINT_TYPE_BULK,            // bmAttributes
	WBVAL(USB_CDC_BUFSIZE),            // wMaxPacketSize
	0x00,                              // bInterval: ignore for Bulk transfer
	
		0x08,   // bLength: Interface Descriptor size
		USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,  // bDescriptorType: IAD
		0x04,   // bFirstInterface
		0x02,   // bInterfaceCount
		USB_DEVICE_CLASS_COMMUNICATIONS,    // bFunctionClass: CDC
		0x00,   // bFunctionSubClass
		0x00,   // bFunctionProtocol
		0x00,   // iFunction

// Interface 4, Alternate Setting 0, CDC Class
	USB_INTERFACE_DESC_SIZE,           // bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,     // bDescriptorType
	0x04,                              // bInterfaceNumber: Number of Interface
	0x00,                              // bAlternateSetting: Alternate setting
	0x01,                              // bNumEndpoints: One endpoint used
	CDC_COMMUNICATION_INTERFACE_CLASS, // bInterfaceClass: Communication Interface Class
	CDC_ABSTRACT_CONTROL_MODEL,        // bInterfaceSubClass: Abstract Control Model
	0x00,                              // bInterfaceProtocol: no protocol used
	0x00,                              // iInterface:
// Header Functional Descriptor
	0x05,                              // bLength: Endpoint Descriptor size
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_HEADER,                        // bDescriptorSubtype: Header Func Desc
	WBVAL(CDC_V1_10), // 1.10          // bcdCDC
// Call Management Functional Descriptor
	0x05,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_CALL_MANAGEMENT,               // bDescriptorSubtype: Call Management Func Desc
	0x00,                              // bmCapabilities: device handles call management
	0x02,                              // bDataInterface: CDC data IF ID
// Abstract Control Management Functional Descriptor
	0x04,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_ABSTRACT_CONTROL_MANAGEMENT,   // bDescriptorSubtype: Abstract Control Management desc
	0x02,                              // bmCapabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
// Union Functional Descriptor
	0x05,                              // bFunctionLength
	CDC_CS_INTERFACE,                  // bDescriptorType: CS_INTERFACE
	CDC_UNION,                         // bDescriptorSubtype: Union func desc
	0x04,                              // bMasterInterface: Communication class interface is master
	0x05,                              // bSlaveInterface0: Data class interface is slave 0

// Endpoint, EP5 Interrupt IN          // event notification (optional)
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_IN(5),                // bEndpointAddress
	USB_ENDPOINT_TYPE_INTERRUPT,       // bmAttributes
	WBVAL(0x0010),                     // wMaxPacketSize
	0xff,                              // bInterval

// Interface 5, Alternate Setting 0, Data class interface descriptor
	USB_INTERFACE_DESC_SIZE,           // bLength
	USB_INTERFACE_DESCRIPTOR_TYPE,     // bDescriptorType
	0x05,                              // bInterfaceNumber: Number of Interface
	0x00,                              // bAlternateSetting: no alternate setting
	0x02,                              // bNumEndpoints: two endpoints used
	CDC_DATA_INTERFACE_CLASS,          // bInterfaceClass: Data Interface Class
	0x00,                              // bInterfaceSubClass: no subclass available
	0x00,                              // bInterfaceProtocol: no protocol used
	0x00,                              // iInterface

// Endpoint, EP6 Bulk OUT
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_OUT(6),               // bEndpointAddress
	USB_ENDPOINT_TYPE_BULK,            // bmAttributes
	WBVAL(USB_CDC_BUFSIZE),            // wMaxPacketSize
	0x00,                              // bInterval: ignore for Bulk transfer

// Endpoint, EP6 Bulk IN
	USB_ENDPOINT_DESC_SIZE,            // bLength
	USB_ENDPOINT_DESCRIPTOR_TYPE,      // bDescriptorType
	USB_ENDPOINT_IN(6),                // bEndpointAddress
	USB_ENDPOINT_TYPE_BULK,            // bmAttributes
	WBVAL(USB_CDC_BUFSIZE),            // wMaxPacketSize
	0x00,                              // bInterval: ignore for Bulk transfer

// Terminator
	0                                  // bLength
};

// USB String Descriptor (optional)
const U8 USB_StringDescriptor[] = {
// Index 0x00: LANGID Codes
	0x04,                              // bLength
	USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
	WBVAL(0x0409), // US English       // wLANGID
// Index 0x01: Manufacturer
	(10*2 + 2),                        // bLength (13 Char + Type + lenght)
	USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
	'U',0,
	'S',0,
	'B',0,
	' ',0,
	'S',0,
	't',0,
	'u',0,
	'd',0,
	'i',0,
	'o',0,
// Index 0x02: Product
	(15*2 + 2),                        // bLength (13 Char + Type + lenght)
	USB_STRING_DESCRIPTOR_TYPE,        // bDescriptorType
	'S',0,
	'T',0,
	'M',0,
	'3',0,
	'2',0,
	'-',0,
	'C',0,
	'D',0,
	'C',0,
	'-',0,
	'U',0,
	'S',0,
	'A',0,
	'R',0,
	'T',0,

// Terminator
	0
};
