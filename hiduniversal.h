/* Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.

This software may be distributed and modified under the terms of the GNU
General Public License version 2 (GPL2) as published by the Free Software
Foundation and appearing in the file GPL2.TXT included in the packaging of
this file. Please note that GPL2 Section 2[b] requires that all works based
on this software must also be made publicly available under the terms of
the GPL2 ("Copyleft").

Contact information
-------------------

Circuits At Home, LTD
Web      :  http://www.circuitsathome.com
e-mail   :  support@circuitsathome.com
 */

#if !defined(__HIDUNIVERSAL_H__)
#define __HIDUNIVERSAL_H__

#include "hid.h"
//#include "hidescriptorparser.h"

class HIDUniversal : public HID {

        struct ReportParser {
                uint8_t rptId;
                HIDReportParser *rptParser;
        } rptParsers[MAX_REPORT_PARSERS];

        // HID class specific descriptor type and length info obtained from HID descriptor
        HID_CLASS_DESCRIPTOR_LEN_AND_TYPE descrInfo[HID_MAX_HID_CLASS_DESCRIPTORS];

        // Returns HID class specific descriptor length by its type and order number
        uint16_t GetHidClassDescrLen(uint8_t type, uint8_t num);

        struct HIDInterface {
                struct {
                        uint8_t bmInterface : 3;
                        uint8_t bmAltSet : 3;
                        uint8_t bmProtocol : 2;
                };
                uint8_t epIndex[maxEpPerInterface];
        };

        uint8_t bConfNum; // configuration number
        uint8_t bNumIface; // number of interfaces in the configuration
        uint8_t bNumEP; // total number of EP in the configuration
        uint32_t qNextPollTime; // next poll time
        uint8_t pollInterval;
        bool bPollEnable; // poll enable flag

        static const uint16_t constBuffLen = 64; // event buffer length
        uint8_t prevBuf[constBuffLen]; // previous event buffer

        void Initialize();
        HIDInterface* FindInterface(uint8_t iface, uint8_t alt, uint8_t proto);

        void ZeroMemory(uint8_t len, uint8_t *buf);
        bool BuffersIdentical(uint8_t len, uint8_t *buf1, uint8_t *buf2);
        void SaveBuffer(uint8_t len, uint8_t *src, uint8_t *dest);

protected:
        EpInfo epInfo[totalEndpoints];
        HIDInterface hidInterfaces[maxHidInterfaces];

        bool bHasReportId;

        uint16_t PID, VID; // PID and VID of connected device

        // HID implementation
        virtual HIDReportParser* GetReportParser(uint32_t id);

        virtual uint32_t OnInitSuccessful() {
                return 0;
        };

        virtual void ParseHIDData(HID * /* hid */, bool /* is_rpt_id */, uint8_t /* len */, uint8_t * /* buf */) {
                return;
        };

public:
        HIDUniversal(USBHost *p);

        // HID implementation
        virtual uint32_t SetReportParser(uint32_t id, HIDReportParser *prs);

        // USBDeviceConfig implementation
        virtual uint32_t Init(uint32_t parent, uint32_t port, uint32_t lowspeed);
        virtual uint32_t Release();
        virtual uint32_t Poll();

        virtual uint32_t GetAddress() {
                return bAddress;
        };

        virtual uint32_t isReady() {
                return bPollEnable;
        };

        // UsbConfigXtracter implementation
        virtual void EndpointXtract(uint32_t conf, uint32_t iface, uint32_t alt, uint32_t proto, const USB_ENDPOINT_DESCRIPTOR *ep);
};

#endif // __HIDUNIVERSAL_H__
