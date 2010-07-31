/*
	lut.cpp
*/

#include "common/application.h"
#include "frame/operations.h"
#include <assert.h>

namespace rikiGlue
{

#if !defined(RIKI_AUTHOR)

const static uint8_t kEncrEncryptLut[896] =
{
	 0x92, 0xDE, 0x2C, 0x2D, 0x9A, 0x18, 0x8F, 0x74, 0x96, 0x91,
	 0x90, 0x39, 0xC8, 0xE1, 0xD8, 0x99, 0xAB, 0x47, 0xBD, 0xD1,
	 0xF0, 0x51, 0x92, 0x54, 0x74, 0x06, 0x93, 0xFA, 0x56, 0xB4,
	 0x58, 0xB9, 0x92, 0x0F, 0x68, 0x0B, 0x5B, 0xCD, 0x88, 0x15,
	 0xAC, 0x14, 0xE0, 0x2D, 0x11, 0xD8, 0x8F, 0xBD, 0x78, 0xF8,
	 0x47, 0x0E, 0x47, 0x15, 0xE3, 0x0A, 0x59, 0x14, 0xD7, 0x5D,
	 0xA8, 0x2E, 0xA0, 0xDC, 0x7A, 0xC1, 0x2E, 0xB6, 0xEF, 0x6A,
	 0xC8, 0xF2, 0x31, 0x57, 0xBB, 0x53, 0xFA, 0x54, 0x12, 0x34,
	 0xFD, 0xC6, 0xA9, 0x38, 0x99, 0xE9, 0x20, 0xEC, 0xFB, 0xC9,
	 0x06, 0xEC, 0x5B, 0x32, 0x66, 0x02, 0x42, 0x97, 0xBA, 0x09,
	 0x1E, 0xA6, 0xEA, 0x48, 0xAD, 0x8D, 0xC4, 0x95, 0x24, 0x1D,
	 0xBE, 0x4D, 0xCB, 0xB8, 0xA6, 0xE9, 0x70, 0x6A, 0xAB, 0x4C,
	 0xDC, 0x67, 0xA4, 0xF6, 0xC4, 0x01, 0x72, 0x87, 0x9D, 0xAB,
	 0x32, 0x73, 0xA4, 0x3B, 0x0F, 0x63, 0xD5, 0x25, 0x8C, 0xE1,
	 0x74, 0xB6, 0x02, 0x99, 0x6E, 0x8D, 0xA3, 0xD1, 0x7F, 0xB4,
	 0x5C, 0x11, 0x2E, 0x90, 0x84, 0x1B, 0x9E, 0x45, 0xF3, 0x6F,
	 0x05, 0x44, 0x47, 0x8D, 0xDA, 0x89, 0x67, 0x7B, 0xD3, 0x8F,
	 0xA4, 0xBD, 0x6B, 0x71, 0x51, 0x07, 0xDC, 0x0E, 0x1B, 0x46,
	 0xF7, 0xA5, 0x8F, 0x14, 0x12, 0xCE, 0xF0, 0x22, 0xAA, 0x0E,
	 0x32, 0x18, 0x87, 0x8E, 0x8E, 0x2D, 0x77, 0xAD, 0xC2, 0xDE,
	 0x5C, 0x75, 0x35, 0x6A, 0x53, 0x37, 0xDB, 0x4B, 0x45, 0x6D,
	 0x65, 0xD7, 0xD3, 0x60, 0xE3, 0xEA, 0x13, 0xCB, 0x3B, 0x15,
	 0x20, 0xFB, 0xBD, 0xB1, 0x10, 0x16, 0x86, 0xAE, 0x30, 0xF1,
	 0x4C, 0x1D, 0xB8, 0xAB, 0x74, 0xDE, 0x66, 0xE3, 0x83, 0xC5,
	 0x78, 0x07, 0x57, 0xEA, 0x43, 0xEA, 0xC6, 0xB5, 0x07, 0xE2,
	 0x64, 0x8F, 0x6A, 0xFB, 0x30, 0x38, 0x31, 0x27, 0x3A, 0xF0,
	 0x1C, 0x47, 0xF8, 0x0C, 0x60, 0xB3, 0x0D, 0x2F, 0xFC, 0x07,
	 0x57, 0x14, 0x4C, 0x24, 0x61, 0xA2, 0xF1, 0xFC, 0xB2, 0xDB,
	 0x64, 0x9E, 0xEE, 0x99, 0xBB, 0x1B, 0x93, 0xCF, 0xCE, 0x77,
	 0x00, 0x1E, 0xB2, 0xAB, 0x3B, 0x84, 0xD3, 0x25, 0xAE, 0x9A,
	 0x83, 0x0A, 0xF6, 0xFE, 0xE7, 0x5E, 0x51, 0x9F, 0xB4, 0xE6,
	 0x6E, 0xBF, 0xA6, 0xD8, 0x9E, 0x3C, 0xAA, 0x75, 0xFC, 0x01,
	 0x08, 0x79, 0x24, 0x84, 0xB0, 0xA3, 0xD4, 0x04, 0x61, 0x91,
	 0xDE, 0xB9, 0xBD, 0x1E, 0x87, 0x1E, 0xFE, 0xD0, 0x34, 0x3F,
	 0x45, 0xE7, 0xB2, 0x01, 0x45, 0x45, 0x72, 0x41, 0x4F, 0xC2,
	 0x44, 0xAC, 0xEA, 0x86, 0x06, 0xED, 0x4B, 0xF7, 0xED, 0x14,
	 0x30, 0x85, 0xE7, 0x67, 0xC9, 0x13, 0xF9, 0x5F, 0x03, 0x4E,
	 0xF4, 0x3D, 0x6E, 0x0C, 0x6B, 0xA6, 0x75, 0xB2, 0x84, 0x5E,
	 0x81, 0x88, 0x13, 0x9B, 0x73, 0x58, 0x84, 0x34, 0xDC, 0x30,
	 0x78, 0x99, 0x3A, 0xB0, 0x33, 0xA1, 0xCE, 0x2C, 0x04, 0x00,
	 0x18, 0xD9, 0x51, 0x3C, 0x69, 0x81, 0x92, 0x32, 0xF7, 0xD5,
	 0x72, 0x84, 0xE3, 0x01, 0x81, 0x86, 0x28, 0xEA, 0xF1, 0xC8,
	 0x1B, 0xA2, 0x71, 0x14, 0xE2, 0xE0, 0xF9, 0x03, 0x78, 0x00,
	 0xB2, 0x26, 0x6B, 0xAA, 0x40, 0xBA, 0x1E, 0xBA, 0x48, 0x5C,
	 0xA7, 0xFF, 0x3A, 0x4A, 0x4A, 0x80, 0x79, 0xD1, 0x76, 0xFA,
	 0x06, 0xDA, 0x80, 0xA3, 0x0E, 0xCF, 0x9B, 0x10, 0xF2, 0xFE,
	 0x30, 0xE1, 0x84, 0xF9, 0x4E, 0x45, 0x98, 0xE1, 0xD4, 0x51,
	 0xF8, 0xD9, 0xCB, 0x3B, 0xBD, 0x8D, 0xD0, 0x67, 0x63, 0xCC,
	 0x05, 0x61, 0x90, 0xED, 0x69, 0x25, 0x5D, 0xC6, 0x72, 0x1D,
	 0x21, 0xC6, 0x15, 0x98, 0x6A, 0x18, 0x30, 0x4C, 0x99, 0xB5,
	 0x85, 0x73, 0x5C, 0xDB, 0x0B, 0xAD, 0xD5, 0x3C, 0x15, 0xB7,
	 0x92, 0xC3, 0x2D, 0xDC, 0xCD, 0x48, 0xD6, 0x7D, 0x95, 0x06,
	 0x10, 0xD6, 0x6E, 0x9B, 0x57, 0xF3, 0x6C, 0xEA, 0x5E, 0x3A,
	 0x67, 0xE8, 0xE8, 0x81, 0x17, 0xA3, 0x0E, 0x5C, 0x83, 0xA3,
	 0x90, 0x1E, 0x1B, 0xF0, 0xF7, 0x74, 0x6B, 0x2C, 0xBB, 0x7A,
	 0xED, 0xD8, 0xF4, 0xBA, 0xB1, 0x33, 0x7C, 0xBC, 0x2B, 0x67,
	 0x70, 0xB0, 0x2F, 0x1C, 0xA5, 0xF2, 0x92, 0x7B, 0xC9, 0x9C,
	 0x6A, 0xA3, 0x87, 0x2C, 0xDC, 0xD1, 0xEE, 0x0A, 0xC6, 0x2D,
	 0x34, 0xD8, 0xA9, 0xB3, 0x49, 0x26, 0x69, 0x93, 0x45, 0x5D,
	 0x5B, 0xBD, 0x5B, 0x86, 0x9E, 0x7C, 0x5B, 0x7E, 0x8D, 0x31,
	 0x8F, 0x6A, 0x8C, 0xC4, 0x58, 0xAE, 0xB3, 0x56, 0xAB, 0xC4,
	 0x86, 0x80, 0x3B, 0x9D, 0x7F, 0xEF, 0xDC, 0x66, 0xD7, 0xE0,
	 0x42, 0x19, 0x57, 0x73, 0x65, 0x63, 0x58, 0x42, 0xA5, 0x6F,
	 0x8B, 0x44, 0xA3, 0xCC, 0x29, 0x88, 0x3A, 0x99, 0xAC, 0xE9,
	 0x9D, 0xB4, 0x73, 0x83, 0x5E, 0x9B, 0x4A, 0x24, 0x39, 0x95,
	 0x66, 0x2A, 0x8D, 0x3B, 0x01, 0x8D, 0xAB, 0x77, 0xA4, 0xA2,
	 0xED, 0x0D, 0x96, 0xE6, 0x0A, 0xE3, 0x2C, 0x94, 0xF3, 0x2E,
	 0x7D, 0x2F, 0xA6, 0xDA, 0x64, 0x73, 0xBC, 0x6A, 0xED, 0x4D,
	 0x3E, 0x9E, 0xB6, 0x7B, 0x20, 0xC8, 0xF2, 0x46, 0x87, 0x72,
	 0x92, 0x4A, 0x47, 0xB0, 0x5D, 0x0B, 0xF8, 0x9A, 0xD7, 0xAF,
	 0xB0, 0x06, 0x9E, 0xF9, 0x18, 0x66, 0xAB, 0x13, 0x5B, 0x77,
	 0x42, 0x12, 0xE1, 0x37, 0x66, 0xC3, 0x5D, 0xD1, 0xB4, 0x50,
	 0x50, 0xA9, 0x83, 0x4F, 0xB1, 0x70, 0x65, 0x21, 0xBC, 0x14,
	 0x5B, 0x1B, 0xC1, 0x49, 0x37, 0x9D, 0x8A, 0x72, 0x12, 0xBE,
	 0xDB, 0xA5, 0xB5, 0xC7, 0xC4, 0xE7, 0xF7, 0x8C, 0xF9, 0x0D,
	 0x79, 0xE4, 0xB0, 0xFE, 0x65, 0xB1, 0x68, 0xA6, 0xA4, 0x1B,
	 0xA2, 0xFD, 0x01, 0x2A, 0x95, 0x47, 0x8D, 0x26, 0xA0, 0x3D,
	 0xFC, 0x2D, 0x31, 0xA2, 0x72, 0x57, 0x0B, 0x7A, 0x98, 0x24,
	 0x3D, 0x8F, 0xA9, 0x00, 0x05, 0x9E, 0xA7, 0x0F, 0x6D, 0xC4,
	 0xB3, 0x04, 0x77, 0x40, 0x2B, 0xCF, 0x38, 0xD6, 0x4D, 0xA0,
	 0xC7, 0x28, 0x7B, 0x46, 0x69, 0x7B, 0x14, 0x6C, 0xED, 0x6B,
	 0xE3, 0x1D, 0x3A, 0x12, 0x51, 0xD3, 0xEC, 0xB7, 0x41, 0x10,
	 0x75, 0xC4, 0xEF, 0x76, 0x84, 0xD7, 0x1A, 0x3A, 0x66, 0x2D,
	 0x00, 0xEF, 0x3C, 0x7A, 0xA0, 0xBE, 0x05, 0x08, 0x22, 0xD2,
	 0xC9, 0x5C, 0x2F, 0xDB, 0x6F, 0x57, 0xE6, 0x27, 0x1C, 0x57,
	 0xCD, 0x91, 0x1D, 0x3A, 0x84, 0x7E, 0xDB, 0x01, 0x95, 0x67,
	 0xC5, 0xD7, 0x9E, 0x5A, 0xC7, 0xB9, 0xC4, 0x01, 0x38, 0x6F,
	 0x45, 0xB0, 0x49, 0x1C, 0x51, 0x99, 0x2D, 0xEB, 0x40, 0xF8,
	 0xBB, 0xF0, 0x71, 0xA4, 0x32, 0xCA, 0xD2, 0xBD, 0x3D, 0xE6,
	 0x86, 0x82, 0x3C, 0x98, 0x6E, 0x61
};

const static uint8_t kEncrDecryptLut[896] =
{
	 0x8C, 0x44, 0x87, 0xD7, 0xE9, 0x68, 0x49, 0xAA, 0x90, 0x2D,
	 0xA6, 0x42, 0xD8, 0xFB, 0xCB, 0xD9, 0x2F, 0x67, 0x41, 0xC5,
	 0x34, 0x17, 0xD2, 0x0C, 0xF4, 0x9D, 0xD5, 0x22, 0x41, 0x36,
	 0x7F, 0x50, 0xB5, 0x5D, 0xD2, 0x3D, 0x42, 0x2E, 0xBA, 0xE8,
	 0xD6, 0x62, 0xC6, 0x9A, 0x17, 0xAD, 0xCB, 0xD9, 0x2B, 0x9C,
	 0x43, 0x74, 0xA8, 0xF7, 0x38, 0xC5, 0x45, 0xD0, 0x82, 0x92,
	 0x7B, 0x5B, 0x8C, 0x2F, 0x43, 0x74, 0xFA, 0x39, 0xA0, 0x30,
	 0xED, 0x72, 0x85, 0x8F, 0x09, 0x3D, 0x3C, 0xE8, 0x44, 0x43,
	 0xB9, 0xC9, 0x37, 0x3D, 0xEF, 0x26, 0x2C, 0xD0, 0xB0, 0xCC,
	 0xDC, 0xC0, 0x19, 0xD1, 0x83, 0xA2, 0x2D, 0x4C, 0x3C, 0xE5,
	 0xA4, 0xF9, 0xE3, 0x66, 0x0D, 0x74, 0x37, 0x9C, 0xBE, 0xDA,
	 0x95, 0x41, 0x8B, 0xFD, 0xB5, 0xDF, 0x8F, 0x9F, 0x26, 0xDA,
	 0xCB, 0x87, 0xD5, 0x9E, 0xF7, 0x4C, 0xFB, 0xFB, 0x12, 0x54,
	 0x4C, 0xF3, 0x7A, 0x0A, 0x5B, 0xC0, 0x3B, 0x98, 0xC5, 0xF7,
	 0x3B, 0xBA, 0x26, 0xB1, 0x9A, 0x86, 0xC0, 0xBC, 0x93, 0x98,
	 0x42, 0xC2, 0x02, 0x56, 0x47, 0xC5, 0xF9, 0xFB, 0x10, 0x5F,
	 0x1E, 0x3D, 0xE1, 0xFC, 0x13, 0x80, 0x5A, 0xF0, 0xE3, 0xC0,
	 0xEF, 0xCC, 0xAD, 0x23, 0x4C, 0xBB, 0x0A, 0xB2, 0x88, 0x15,
	 0x2B, 0x6A, 0x99, 0xD6, 0xF5, 0x4A, 0x00, 0x70, 0xB9, 0x07,
	 0xEC, 0xB3, 0xBB, 0x73, 0x0D, 0x33, 0xD4, 0x6A, 0x87, 0xFB,
	 0xC6, 0x6C, 0x6D, 0x8A, 0xEA, 0x3E, 0x68, 0x55, 0x59, 0x06,
	 0xC7, 0xB6, 0x9E, 0xD1, 0xCD, 0xC1, 0xEE, 0xAB, 0x1D, 0xFA,
	 0xE2, 0x13, 0x2B, 0xF7, 0x0B, 0x20, 0xDF, 0xE0, 0x71, 0x80,
	 0x1C, 0x21, 0xB8, 0x40, 0x93, 0xFB, 0xB9, 0x61, 0x39, 0x95,
	 0x1B, 0xC0, 0xBC, 0x1F, 0x7B, 0x91, 0x4C, 0x81, 0x2A, 0xB8,
	 0x98, 0xB2, 0xA1, 0x9D, 0xA6, 0xED, 0x68, 0x84, 0x53, 0x46,
	 0x4B, 0x8A, 0x11, 0xD6, 0x1D, 0x1F, 0x96, 0x2E, 0xDF, 0x5A,
	 0x43, 0x70, 0x29, 0x6D, 0xA8, 0x1D, 0x46, 0xF0, 0xBE, 0x60,
	 0x68, 0x45, 0x47, 0x91, 0x1C, 0x13, 0xD2, 0x0C, 0xD2, 0x5F,
	 0x55, 0x74, 0x81, 0xEE, 0x63, 0x40, 0xE1, 0x71, 0x9C, 0x11,
	 0xB9, 0xDB, 0x60, 0x5F, 0x9F, 0x9D, 0x22, 0x19, 0x06, 0x6E,
	 0x8E, 0x13, 0x64, 0xBC, 0xB5, 0x35, 0xD4, 0x45, 0xF4, 0xC1,
	 0x90, 0x48, 0x7E, 0xD8, 0xC7, 0x6C, 0x3F, 0xF4, 0x43, 0x6C,
	 0xEC, 0x95, 0x8E, 0x1A, 0x66, 0x76, 0x3A, 0x65, 0x97, 0xAE,
	 0x05, 0xC8, 0xF0, 0x08, 0xEA, 0x03, 0x37, 0x22, 0x2B, 0xD8,
	 0xDF, 0xC5, 0xE9, 0x77, 0x45, 0x88, 0x9B, 0x7A, 0x0F, 0xB9,
	 0xD7, 0x23, 0x5B, 0x11, 0xE9, 0x68, 0xB2, 0xDF, 0x1A, 0x1B,
	 0x9D, 0x1B, 0x93, 0xB0, 0xED, 0x9F, 0x7E, 0xD9, 0x43, 0xFA,
	 0x97, 0x4F, 0x05, 0x03, 0x30, 0x1D, 0x5C, 0x92, 0xEE, 0xB0,
	 0xE6, 0x5B, 0x34, 0xEB, 0x11, 0x61, 0xE7, 0xBF, 0x83, 0x06,
	 0xB5, 0x35, 0x79, 0x6D, 0xAE, 0x04, 0x91, 0x7E, 0x73, 0xF6,
	 0x65, 0x14, 0x8A, 0x19, 0x33, 0x13, 0x2D, 0xA7, 0x40, 0xDC,
	 0x9F, 0x8B, 0x75, 0x6F, 0x26, 0x7E, 0x85, 0x58, 0x3D, 0xD3,
	 0x56, 0xAC, 0xBD, 0x5C, 0x5F, 0x65, 0xCC, 0xE8, 0x0D, 0x45,
	 0xC9, 0x8C, 0x85, 0x38, 0xA0, 0xEA, 0xCE, 0x57, 0x68, 0x61,
	 0x6B, 0xFF, 0x4E, 0xE7, 0x2E, 0xCF, 0xB1, 0x18, 0x25, 0x9F,
	 0xBE, 0x0D, 0xCC, 0xD9, 0xF1, 0xD9, 0x5A, 0xA5, 0xB7, 0x75,
	 0x23, 0xF4, 0xAA, 0x12, 0xA4, 0x12, 0x7D, 0x04, 0x39, 0x43,
	 0xC2, 0x28, 0x7E, 0x84, 0x62, 0x66, 0x2D, 0x55, 0x28, 0x7B,
	 0x07, 0xE4, 0x6B, 0x5C, 0x5A, 0xE2, 0xAC, 0xE7, 0x96, 0x11,
	 0x35, 0xFD, 0x93, 0x8C, 0x2D, 0xB4, 0xC9, 0x8E, 0xDD, 0xE7,
	 0x14, 0x78, 0x0E, 0x87, 0xED, 0xE7, 0x83, 0x9A, 0x12, 0x3C,
	 0x20, 0x88, 0xA5, 0x02, 0x5A, 0x4F, 0x22, 0x51, 0x9F, 0xE3,
	 0xC0, 0x94, 0x43, 0xF1, 0x20, 0x9B, 0x75, 0x2E, 0xA9, 0x1F,
	 0xBA, 0x80, 0x8A, 0xCF, 0xA2, 0x96, 0xEE, 0xFB, 0xBA, 0x60,
	 0x63, 0x43, 0x44, 0xE3, 0x39, 0xF4, 0x9A, 0x41, 0xD3, 0x30,
	 0x4C, 0x87, 0xC2, 0xD4, 0x35, 0xC0, 0x5E, 0xD2, 0xC0, 0x0F,
	 0xC1, 0x18, 0xBC, 0xE5, 0xEA, 0xA2, 0x89, 0x24, 0x51, 0x6D,
	 0xDB, 0xA9, 0xA7, 0x62, 0x0F, 0x7E, 0x75, 0x3D, 0x82, 0x7D,
	 0x3B, 0xAA, 0x91, 0x82, 0x9A, 0xA1, 0x18, 0x52, 0x95, 0xF2,
	 0x9C, 0x2B, 0x6D, 0xD8, 0x35, 0xA8, 0xBB, 0xCA, 0x94, 0xA4,
	 0xE0, 0x6D, 0x79, 0x66, 0x37, 0x7C, 0x47, 0x9F, 0x71, 0x7B,
	 0xCF, 0xF3, 0x7B, 0xC1, 0x27, 0x66, 0x0B, 0x59, 0xD2, 0x7B,
	 0x36, 0x59, 0x90, 0x38, 0xFD, 0xC0, 0xB8, 0xC8, 0xB2, 0x4F,
	 0x74, 0x40, 0xCA, 0x4E, 0xF2, 0xF5, 0x4E, 0x39, 0xB8, 0x46,
	 0x32, 0x5B, 0x16, 0xC4, 0xE3, 0xA3, 0x45, 0xF7, 0x78, 0x37,
	 0x14, 0x8F, 0x07, 0xD0, 0xB7, 0x3C, 0xE4, 0x53, 0x22, 0x11,
	 0x62, 0x5C, 0x5E, 0x54, 0x8D, 0x76, 0xCE, 0xCA, 0x7E, 0xAB,
	 0x45, 0x5D, 0xFC, 0xD2, 0x5D, 0x93, 0x8C, 0x8B, 0x72, 0x5B,
	 0x0C, 0x0D, 0xAD, 0x3E, 0xAE, 0xE3, 0xC0, 0xDC, 0xE9, 0x18,
	 0x88, 0xEA, 0x88, 0xD3, 0x23, 0xBE, 0xAE, 0x27, 0xAC, 0xB8,
	 0xF5, 0x0D, 0x16, 0xDC, 0x86, 0xF1, 0xC7, 0x44, 0x21, 0x29,
	 0xF1, 0x0E, 0xCD, 0x35, 0x24, 0x64, 0x88, 0xED, 0x8D, 0xCA,
	 0xD1, 0x20, 0x76, 0x24, 0xB4, 0xEA, 0x2F, 0x4B, 0x30, 0x09,
	 0x2D, 0xE4, 0xED, 0x50, 0x38, 0xB0, 0xC5, 0x40, 0x10, 0x9F,
	 0xBB, 0x45, 0x81, 0x8D, 0x9A, 0xD8, 0xC2, 0x84, 0x30, 0x1A,
	 0x28, 0x03, 0x31, 0x9D, 0x6B, 0x94, 0x4F, 0xE7, 0xA9, 0x62,
	 0xF9, 0xF6, 0x4A, 0x9F, 0x28, 0xFC, 0x46, 0xBF, 0x46, 0x84,
	 0xA3, 0x91, 0xA7, 0x3D, 0x02, 0xFA, 0xC3, 0xD9, 0x18, 0x5C,
	 0xB0, 0x5C, 0xC1, 0x84, 0xC1, 0xF1, 0x54, 0x5C, 0x76, 0x48,
	 0x8B, 0xDE, 0x49, 0x63, 0x8E, 0x2B, 0x7B, 0xD4, 0x8E, 0x61,
	 0xA6, 0xDC, 0x2E, 0x6B, 0xD2, 0x01, 0x57, 0xA5, 0x7E, 0x2A,
	 0x21, 0x65, 0x8D, 0xFA, 0x98, 0x37, 0x16, 0x10, 0xF7, 0x05,
	 0x62, 0x0E, 0xDE, 0x85, 0x38, 0x9D, 0x40, 0x38, 0x69, 0xDD,
	 0x51, 0xA5, 0x3C, 0xE6, 0x58, 0x05, 0xBD, 0xBF, 0x8E, 0x70,
	 0x0F, 0x68, 0xD6, 0x89, 0xA2, 0x16, 0xFE, 0x89, 0xDC, 0xBB,
	 0x4C, 0x6D, 0x67, 0xA8, 0xD1, 0x4E, 0xD2, 0xE2, 0xB0, 0xC3,
	 0x24, 0x6C, 0xCF, 0x2A, 0xB0, 0x91, 0x0B, 0x8B, 0xBA, 0x62,
	 0xEE, 0x3E, 0x2D, 0x63, 0x7F, 0xA5, 0x07, 0x68, 0xA6, 0xEA,
	 0xED, 0x7F, 0x04, 0xE9, 0x78, 0x15
};

static uint8_t kEncryptLut5[3][256];
static uint8_t kDecryptLut5[3][256];

void
Application::decryptLuts()
{
	Context::bytes_t decrypted;
	if ( !this->decrypt(kEncrEncryptLut, sizeof(kEncrEncryptLut), decrypted) )
		memcpy(kEncryptLut5, kEncrEncryptLut, sizeof(kEncryptLut5));
	else
		memcpy(kEncryptLut5, &decrypted[0], (std::min)(sizeof(kEncryptLut5), decrypted.size()));
		
	
	decrypted.clear();
	if ( !this->decrypt(kEncrDecryptLut, sizeof(kEncrDecryptLut), decrypted) )
		memcpy(kDecryptLut5, kEncrDecryptLut, sizeof(kDecryptLut5));
	else
		memcpy(kDecryptLut5, &decrypted[0], (std::min)(sizeof(kDecryptLut5), decrypted.size()));
}

#else

	extern const uint8_t kEncryptLut5[3][256];
	extern const uint8_t kDecryptLut5[3][256];

#endif

static void
lutCrypt( const uint8_t      lut[3][256],
          const Block        &block )
{
	const register_t blockDiv = block.dstSize / 3;
	const uint8_t *srcData = block.srcData;
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < blockDiv; ++i )
	{
#if 0
		*dstData++ = lut[0][*srcData++];
		*dstData++ = lut[1][*srcData++];
		*dstData++ = lut[2][*srcData++];
#else
		if ( srcData[0] > srcData[2] )
			dstData[1] = lut[0][ srcData[1] ];
		else
			dstData[1] = lut[1][ srcData[1] ];

		dstData += 3;
		srcData += 3;
#endif
	}
}

register_t
lutEncrypt( const Block     &block )
{
	lutCrypt(kEncryptLut5, block);
	return ( 0 );
}

register_t
lutDecrypt( const Block     &block )
{
	lutCrypt(kDecryptLut5, block);
	return ( 0 );
}

register_t
splitterRGBA( const Block    &block )
{
	assert(block.dstSize < block.srcSize );

	uint8_t *srcData = const_cast<uint8_t*>(block.srcData);
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < block.dstSize; i += 3 )
	{
		srcData++;
		const uint8_t b = srcData[2];
		if ( srcData[0] > b )
			memset(srcData, kDecryptLut5[0][ srcData[1] ], 3);
		else
			memset(srcData, kDecryptLut5[1][ srcData[1] ], 3);
	
		memset(dstData, b, 3);

		dstData += 3;
		srcData += 3;
	}
	return ( 0 );
}

register_t
splitterBGRA( const Block    &block )
{
	assert(block.dstSize < block.srcSize );

	uint8_t *srcData = const_cast<uint8_t*>(block.srcData);
	uint8_t *dstData = block.dstData;

	for ( register_t i = 0; i < block.dstSize; i += 3 )
	{
		const uint8_t b = srcData[0];
		if ( srcData[2] > b )
			memset(srcData, kDecryptLut5[0][ srcData[1] ], 3);
		else
			memset(srcData, kDecryptLut5[1][ srcData[1] ], 3);
	
		memset(dstData, b, 3);

		dstData += 3;
		srcData += 4;
	}
	return ( 0 );
}

} /* namespace rikiGlue */
