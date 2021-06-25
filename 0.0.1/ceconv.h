#pragma once

#ifndef CECONV_H
#define CECONV_H

unsigned long long cec_utf8_to_ucs2le(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size);

unsigned long long cec_utf8_to_ucs2be(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size);

unsigned long long cec_ucs2le_to_utf8(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size);

unsigned long long cec_ucs2be_to_utf8(unsigned char* load_buffer, unsigned char* store_buffer, unsigned long long size);

#endif