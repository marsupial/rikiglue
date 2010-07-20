/*
	lut.cpp
*/

#include "frame/frame.h"

namespace rikiGlue
{

unsigned char kEncryptLut[3][256] = 
{
	{
		 0x79, 0x35, 0xF4, 0x44, 0x95, 0xCB, 0x05, 0x5D, 0xFB, 0x1D,
		 0x25, 0x78, 0x34, 0x3C, 0xE3, 0xC6, 0xCA, 0x04, 0xED, 0x14,
		 0x1C, 0x70, 0x77, 0x33, 0xBE, 0xE2, 0x93, 0x51, 0x03, 0xA4,
		 0x13, 0x1B, 0x6F, 0x76, 0xB8, 0xBD, 0x41, 0x92, 0x50, 0xCE,
		 0x0A, 0x12, 0x1A, 0x6E, 0x75, 0x31, 0xF3, 0xE1, 0x91, 0xE7,
		 0x54, 0x09, 0x11, 0x19, 0x6D, 0xDC, 0x30, 0x38, 0xE0, 0x90,
		 0x4E, 0x53, 0x08, 0x10, 0x18, 0x6C, 0x73, 0xDD, 0x81, 0x88,
		 0x8F, 0x4D, 0xCC, 0xD0, 0x0F, 0x64, 0x6B, 0x72, 0x2E, 0x80,
		 0xC0, 0x45, 0xE6, 0xEA, 0x06, 0xD2, 0xEF, 0x6A, 0xDB, 0x2D,
		 0xDF, 0x86, 0xC2, 0x4B, 0xE9, 0x57, 0x0D, 0x15, 0xD6, 0xAF,
		 0x2C, 0x7E, 0x85, 0x43, 0xE5, 0x52, 0x56, 0x0C, 0xFA, 0x68,
		 0xDA, 0x2B, 0xDE, 0x84, 0x42, 0xF6, 0xC9, 0xEC, 0x0B, 0x61,
		 0x67, 0xAE, 0x2A, 0x32, 0x83, 0xFE, 0xE4, 0x99, 0x9E, 0xA3,
		 0xFF, 0xAC, 0xD9, 0x29, 0xB7, 0xBC, 0x40, 0xC5, 0x4F, 0xCD,
		 0xA2, 0xD3, 0xD4, 0xAD, 0x74, 0xF1, 0xF2, 0x3F, 0xC4, 0x97,
		 0x9D, 0x59, 0xA8, 0xAB, 0xD8, 0x27, 0x2F, 0xBB, 0x3E, 0xF5,
		 0x96, 0x9C, 0xA1, 0xA7, 0xAA, 0x1E, 0x26, 0x00, 0xBA, 0x3D,
		 0x8E, 0x4C, 0x9B, 0xCF, 0xA6, 0x63, 0xF0, 0x71, 0xB5, 0x7F,
		 0xBF, 0x8D, 0x94, 0x9A, 0xA0, 0xA5, 0xEE, 0x69, 0x24, 0xFD,
		 0xB9, 0x3B, 0x8C, 0x4A, 0xE8, 0x9F, 0x5C, 0x62, 0xD5, 0x23,
		 0xB4, 0x7D, 0x3A, 0x8B, 0x49, 0xF8, 0x55, 0xD1, 0xA9, 0xFC,
		 0x22, 0xB3, 0x7C, 0x39, 0x8A, 0x48, 0xC8, 0x02, 0x5B, 0x60,
		 0x66, 0x21, 0xB2, 0x7B, 0x82, 0x89, 0x47, 0x98, 0xEB, 0x5A,
		 0x5F, 0x65, 0x20, 0x28, 0x7A, 0x37, 0xC1, 0x46, 0xC7, 0x01,
		 0x07, 0x5E, 0x17, 0x1F, 0xB1, 0xB6, 0x36, 0x87, 0xC3, 0xF7,
		 0xF9, 0x58, 0x0E, 0x16, 0xD7, 0xB0
	},
	{
		 0x9A, 0xD4, 0x5C, 0xDD, 0x12, 0x1B, 0xF8, 0x7D, 0xE9, 0x8A,
		 0xCB, 0x4D, 0x53, 0x5B, 0xAA, 0x11, 0xB3, 0x22, 0x7C, 0x33,
		 0x89, 0x90, 0x98, 0xF3, 0x5A, 0xDC, 0xDE, 0x6E, 0x21, 0x7B,
		 0x32, 0x88, 0xFA, 0x97, 0xF2, 0xA5, 0x06, 0x0F, 0x6D, 0x20,
		 0xE6, 0x81, 0x87, 0x8F, 0xCD, 0x9D, 0xA4, 0x5F, 0xAD, 0x6C,
		 0x73, 0xBA, 0x80, 0x38, 0x8E, 0x95, 0x50, 0xA3, 0x04, 0xAC,
		 0x15, 0x72, 0xE5, 0xC0, 0x37, 0xC8, 0x94, 0x9B, 0xA2, 0x03,
		 0x64, 0x14, 0xB5, 0xB9, 0xBF, 0x84, 0x8C, 0xEE, 0x4E, 0xA1,
		 0x02, 0x63, 0xDF, 0xE1, 0x24, 0xBE, 0xFF, 0x3D, 0x92, 0xFB,
		 0xA0, 0xFC, 0x09, 0xAF, 0xB4, 0x23, 0xBD, 0x34, 0x3C, 0x91,
		 0x99, 0xD3, 0x01, 0x08, 0xF7, 0x6F, 0xB8, 0xBC, 0xFE, 0x3B,
		 0xCA, 0xEF, 0xD2, 0xA6, 0x07, 0x67, 0x18, 0xB7, 0x29, 0x82,
		 0x3A, 0x42, 0xCE, 0x52, 0xD7, 0xFD, 0xF6, 0x17, 0xB6, 0x7A,
		 0xC1, 0x39, 0xC9, 0x96, 0x51, 0xD6, 0x05, 0x66, 0x16, 0xE3,
		 0x27, 0xE7, 0x86, 0x40, 0xCC, 0x9C, 0xF5, 0xDA, 0x65, 0xB0,
		 0x1D, 0x26, 0x7F, 0x85, 0x3F, 0x47, 0x4F, 0xD5, 0x5D, 0x0B,
		 0x6A, 0x71, 0x25, 0x7E, 0xEA, 0x3E, 0x46, 0x00, 0x54, 0xA8,
		 0x0A, 0x69, 0x70, 0x76, 0x2C, 0x83, 0xC7, 0x45, 0xCF, 0xF4,
		 0xA7, 0x62, 0x68, 0x1A, 0xE4, 0x2B, 0xE8, 0xC6, 0x44, 0x4C,
		 0x9F, 0xD8, 0x61, 0x10, 0x19, 0x75, 0x2A, 0xC2, 0xEB, 0x43,
		 0x4B, 0x9E, 0x59, 0x60, 0xAE, 0xE0, 0x74, 0xBB, 0x31, 0xC5,
		 0xF9, 0x4A, 0xD1, 0x58, 0xA9, 0x0E, 0xB2, 0x1F, 0x28, 0x30,
		 0xC4, 0x41, 0x49, 0xF1, 0x57, 0xDB, 0x0D, 0xB1, 0x1E, 0x79,
		 0x2F, 0xC3, 0x8D, 0x48, 0xD0, 0x56, 0x5E, 0x0C, 0x6B, 0xE2,
		 0x78, 0x2E, 0x36, 0xEC, 0x93, 0xF0, 0x55, 0xD9, 0xAB, 0x13,
		 0x1C, 0x77, 0x2D, 0x35, 0x8B, 0xED
	},
	{
		 0x67, 0xC0, 0x14, 0x7D, 0xFF, 0xED, 0xEE, 0xA0, 0xA7, 0xAE,
		 0x5D, 0xBE, 0x6D, 0x13, 0x7C, 0x26, 0x8D, 0x39, 0xD6, 0xA6,
		 0xAD, 0xB6, 0x65, 0x6C, 0x72, 0xC7, 0x25, 0x8C, 0x38, 0x41,
		 0xF1, 0xDE, 0xB5, 0x64, 0x6B, 0xEB, 0x1A, 0xCA, 0x2D, 0x93,
		 0x40, 0x49, 0xDD, 0xF6, 0xE7, 0xFD, 0x0F, 0x19, 0x22, 0x2C,
		 0x92, 0x9B, 0x48, 0x51, 0x58, 0xE6, 0x04, 0x70, 0x77, 0x21,
		 0x88, 0xD1, 0x9A, 0xA3, 0x50, 0x57, 0x60, 0xE8, 0x6F, 0x16,
		 0x7F, 0xCC, 0x33, 0xEF, 0xA2, 0xF3, 0xB0, 0x5F, 0x68, 0xEA,
		 0x15, 0x7E, 0x28, 0x32, 0x3B, 0xD7, 0xA8, 0xAF, 0xF7, 0xF9,
		 0x0A, 0x74, 0xC8, 0x27, 0x8E, 0x3A, 0x43, 0xF2, 0xE0, 0xE4,
		 0x01, 0x09, 0x73, 0x1C, 0x84, 0x2F, 0x95, 0x42, 0x4B, 0xDF,
		 0xE3, 0xF8, 0xBF, 0xC3, 0x1B, 0x83, 0x2E, 0x94, 0x9D, 0x4A,
		 0xF5, 0x5A, 0xBB, 0x06, 0x10, 0x79, 0x23, 0x8A, 0xD2, 0x9C,
		 0xD9, 0x52, 0x59, 0xBA, 0x05, 0x71, 0x78, 0x81, 0x89, 0x35,
		 0xD4, 0xF0, 0xF4, 0xB2, 0x61, 0xFA, 0xC1, 0xC5, 0x80, 0xCD,
		 0x34, 0x3D, 0xD8, 0xDC, 0xB1, 0xB8, 0x03, 0x0C, 0x76, 0xFC,
		 0x29, 0x8F, 0x3C, 0x45, 0xDB, 0xE1, 0xFE, 0x00, 0x0B, 0x75,
		 0x1E, 0x86, 0x31, 0x97, 0x44, 0x4D, 0x54, 0xB7, 0x66, 0xE9,
		 0xEC, 0x1D, 0x85, 0x30, 0x96, 0x9F, 0x4C, 0x53, 0x5C, 0xBD,
		 0x08, 0x12, 0x7B, 0xCB, 0xCF, 0xD3, 0x9E, 0xA5, 0xAC, 0x5B,
		 0xBC, 0x07, 0x11, 0x7A, 0x24, 0x8B, 0x37, 0xD5, 0xDA, 0xAB,
		 0xB4, 0x63, 0x6A, 0xC2, 0xC6, 0x82, 0xCE, 0x36, 0x3F, 0xA4,
		 0xAA, 0xB3, 0x62, 0xFB, 0x0E, 0x18, 0xC9, 0x2B, 0x91, 0x3E,
		 0x47, 0xA9, 0xE2, 0xB9, 0x69, 0x0D, 0x17, 0x20, 0x2A, 0x90,
		 0x99, 0x46, 0x4F, 0x56, 0xE5, 0x02, 0x6E, 0xC4, 0x1F, 0x87,
		 0xD0, 0x98, 0xA1, 0x4E, 0x55, 0x5E
	}
};

unsigned char kDecryptLut[3][256] =
{
	{
		 0xA7, 0xEF, 0xD9, 0x1C, 0x11, 0x06, 0x54, 0xF0, 0x3E, 0x33,
		 0x28, 0x76, 0x6B, 0x60, 0xFC, 0x4A, 0x3F, 0x34, 0x29, 0x1E,
		 0x13, 0x61, 0xFD, 0xF2, 0x40, 0x35, 0x2A, 0x1F, 0x14, 0x09,
		 0xA5, 0xF3, 0xE8, 0xDD, 0xD2, 0xC7, 0xBC, 0x0A, 0xA6, 0x9B,
		 0xE9, 0x85, 0x7A, 0x6F, 0x64, 0x59, 0x4E, 0x9C, 0x38, 0x2D,
		 0x7B, 0x17, 0x0C, 0x01, 0xF6, 0xEB, 0x39, 0xD5, 0xCA, 0xBF,
		 0x0D, 0xA9, 0x9E, 0x93, 0x88, 0x24, 0x72, 0x67, 0x03, 0x51,
		 0xED, 0xE2, 0xD7, 0xCC, 0xC1, 0x5D, 0xAB, 0x47, 0x3C, 0x8A,
		 0x26, 0x1B, 0x69, 0x3D, 0x32, 0xCE, 0x6A, 0x5F, 0xFB, 0x97,
		 0xE5, 0xDA, 0xC4, 0x07, 0xF1, 0xE6, 0xDB, 0x77, 0xC5, 0xAF,
		 0x4B, 0xE7, 0xDC, 0x78, 0x6D, 0xBB, 0x57, 0x4C, 0x41, 0x36,
		 0x2B, 0x20, 0x15, 0xB1, 0x4D, 0x42, 0x90, 0x2C, 0x21, 0x16,
		 0x0B, 0x00, 0xEA, 0xDF, 0xD4, 0xC9, 0x65, 0xB3, 0x4F, 0x44,
		 0xE0, 0x7C, 0x71, 0x66, 0x5B, 0xF7, 0x45, 0xE1, 0xD6, 0xCB,
		 0xC0, 0xB5, 0xAA, 0x46, 0x3B, 0x30, 0x25, 0x1A, 0xB6, 0x04,
		 0xA0, 0x95, 0xE3, 0x7F, 0xB7, 0xAC, 0xA1, 0x96, 0x80, 0xC3,
		 0xB8, 0xA2, 0x8C, 0x81, 0x1D, 0xB9, 0xAE, 0xA3, 0x98, 0xD0,
		 0xA4, 0x99, 0x83, 0x8F, 0x79, 0x63, 0xFF, 0xF4, 0xDE, 0xD3,
		 0xC8, 0xB2, 0xF5, 0x86, 0x22, 0xBE, 0xA8, 0x9D, 0x87, 0x23,
		 0x18, 0xB4, 0x50, 0xEC, 0x5C, 0xF8, 0x94, 0x89, 0x0F, 0xEE,
		 0xD8, 0x74, 0x10, 0x05, 0x48, 0x8B, 0x27, 0xAD, 0x49, 0xCF,
		 0x55, 0x8D, 0x8E, 0xC6, 0x62, 0xFE, 0x9A, 0x84, 0x6E, 0x58,
		 0x37, 0x43, 0x70, 0x5A, 0x3A, 0x2F, 0x19, 0x0E, 0x7E, 0x68,
		 0x52, 0x31, 0xC2, 0x5E, 0x53, 0xE4, 0x75, 0x12, 0xBA, 0x56,
		 0xB0, 0x91, 0x92, 0x2E, 0x02, 0x9F, 0x73, 0xF9, 0xCD, 0xFA,
		 0x6C, 0x08, 0xD1, 0xBD, 0x7D, 0x82
	},
	{
		 0xA7, 0x66, 0x50, 0x45, 0x3A, 0x88, 0x24, 0x72, 0x67, 0x5C,
		 0xAA, 0x9F, 0xED, 0xE2, 0xD7, 0x25, 0xC1, 0x0F, 0x04, 0xF9,
		 0x47, 0x3C, 0x8A, 0x7F, 0x74, 0xC2, 0xB7, 0x05, 0xFA, 0x96,
		 0xE4, 0xD9, 0x27, 0x1C, 0x11, 0x5F, 0x54, 0xA2, 0x97, 0x8C,
		 0xDA, 0x76, 0xC4, 0xB9, 0xAE, 0xFC, 0xF1, 0xE6, 0xDB, 0xD0,
		 0x1E, 0x13, 0x61, 0xFD, 0xF2, 0x40, 0x35, 0x83, 0x78, 0x6D,
		 0x62, 0x57, 0xA5, 0x9A, 0x8F, 0xDD, 0x79, 0xC7, 0xBC, 0xB1,
		 0xA6, 0x9B, 0xE9, 0xDE, 0xD3, 0xC8, 0xBD, 0x0B, 0x4E, 0x9C,
		 0x38, 0x86, 0x7B, 0x0C, 0xA8, 0xF6, 0xEB, 0xE0, 0xD5, 0xCA,
		 0x18, 0x0D, 0x02, 0x9E, 0xEC, 0x2F, 0xCB, 0xC0, 0xB5, 0x51,
		 0x46, 0x94, 0x89, 0x73, 0xB6, 0xAB, 0xA0, 0xEE, 0x31, 0x26,
		 0x1B, 0x69, 0xAC, 0xA1, 0x3D, 0x32, 0xCE, 0xC3, 0xAD, 0xFB,
		 0xF0, 0xE5, 0x81, 0x1D, 0x12, 0x07, 0xA3, 0x98, 0x34, 0x29,
		 0x77, 0xAF, 0x4B, 0x99, 0x8E, 0x2A, 0x1F, 0x14, 0x09, 0xFE,
		 0x4C, 0xE8, 0x36, 0x2B, 0x15, 0x63, 0x58, 0xF4, 0x42, 0x37,
		 0x85, 0x21, 0x16, 0x64, 0x00, 0x43, 0x91, 0x2D, 0xC9, 0xBE,
		 0x5A, 0x4F, 0x44, 0x39, 0x2E, 0x23, 0x71, 0xB4, 0xA9, 0xD6,
		 0x0E, 0xF8, 0x3B, 0x30, 0xCC, 0x5D, 0x95, 0xE3, 0xD8, 0x10,
		 0x5E, 0x48, 0x80, 0x75, 0x6A, 0x49, 0x33, 0xCF, 0x6B, 0x60,
		 0x55, 0x4A, 0x3F, 0x82, 0xC5, 0xE7, 0xDC, 0xD1, 0xBB, 0xB0,
		 0x41, 0x84, 0x6E, 0x0A, 0x90, 0x2C, 0x7A, 0xB2, 0xEA, 0xD4,
		 0x70, 0x65, 0x01, 0x9D, 0x87, 0x7C, 0xBF, 0xF7, 0x93, 0xE1,
		 0x19, 0x03, 0x1A, 0x52, 0xCD, 0x53, 0xEF, 0x8B, 0xB8, 0x3E,
		 0x28, 0x8D, 0xBA, 0x08, 0xA4, 0xC6, 0xF3, 0xFF, 0x4D, 0x6F,
		 0xF5, 0xDF, 0x22, 0x17, 0xB3, 0x92, 0x7E, 0x68, 0x06, 0xD2,
		 0x20, 0x59, 0x5B, 0x7D, 0x6C, 0x56
	},
	{
		 0xA7, 0x64, 0xF5, 0x9C, 0x38, 0x86, 0x7B, 0xC9, 0xBE, 0x65,
		 0x5A, 0xA8, 0x9D, 0xEB, 0xE0, 0x2E, 0x7C, 0xCA, 0xBF, 0x0D,
		 0x02, 0x50, 0x45, 0xEC, 0xE1, 0x2F, 0x24, 0x72, 0x67, 0xB5,
		 0xAA, 0xF8, 0xED, 0x3B, 0x30, 0x7E, 0xCC, 0x1A, 0x0F, 0x5D,
		 0x52, 0xA0, 0xEE, 0xE3, 0x31, 0x26, 0x74, 0x69, 0xB7, 0xAC,
		 0x53, 0x48, 0x96, 0x8B, 0xD9, 0xCE, 0x1C, 0x11, 0x5F, 0x54,
		 0xA2, 0x97, 0xE5, 0xDA, 0x28, 0x1D, 0x6B, 0x60, 0xAE, 0xA3,
		 0xF1, 0xE6, 0x34, 0x29, 0x77, 0x6C, 0xBA, 0xAF, 0xFD, 0xF2,
		 0x40, 0x35, 0x83, 0xBB, 0xB0, 0xFE, 0xF3, 0x41, 0x36, 0x84,
		 0x79, 0xC7, 0xBC, 0x0A, 0xFF, 0x4D, 0x42, 0x90, 0xDE, 0xD3,
		 0x21, 0x16, 0xB2, 0x00, 0x4E, 0xEA, 0xD4, 0x22, 0x17, 0x0C,
		 0xF6, 0x44, 0x39, 0x87, 0x18, 0x66, 0x5B, 0xA9, 0x9E, 0x3A,
		 0x88, 0x7D, 0xCB, 0xC0, 0x0E, 0x03, 0x51, 0x46, 0x94, 0x89,
		 0xD7, 0x73, 0x68, 0xB6, 0xAB, 0xF9, 0x3C, 0x8A, 0x7F, 0xCD,
		 0x1B, 0x10, 0x5E, 0xA1, 0xEF, 0xE4, 0x32, 0x27, 0x75, 0x6A,
		 0xB8, 0xAD, 0xFB, 0xF0, 0x3E, 0x33, 0x81, 0x76, 0xC4, 0xB9,
		 0x07, 0xFC, 0x4A, 0x3F, 0xDB, 0xC5, 0x13, 0x08, 0x56, 0xE7,
		 0xDC, 0xD1, 0xC6, 0x14, 0x09, 0x57, 0x4C, 0x9A, 0x8F, 0xDD,
		 0xD2, 0x20, 0x15, 0xB1, 0x9B, 0xE9, 0x85, 0x7A, 0xC8, 0xBD,
		 0x0B, 0x70, 0x01, 0x92, 0xD5, 0x71, 0xF7, 0x93, 0xD6, 0x19,
		 0x5C, 0xE2, 0x25, 0xC1, 0x47, 0x95, 0xD8, 0xC2, 0xFA, 0x3D,
		 0x80, 0xC3, 0x8C, 0xCF, 0x12, 0x55, 0x98, 0x82, 0xD0, 0xA4,
		 0x99, 0x2A, 0x1F, 0x6D, 0x62, 0xA5, 0xE8, 0x6E, 0x63, 0xF4,
		 0x37, 0x2C, 0x43, 0xB3, 0x4F, 0x23, 0xB4, 0x05, 0x06, 0x49,
		 0x8D, 0x1E, 0x61, 0x4B, 0x8E, 0x78, 0x2B, 0x58, 0x6F, 0x59,
		 0x91, 0xDF, 0x9F, 0x2D, 0xA6, 0x04
	}
};

static void
lutCrypt( unsigned char    lut[3][256],
          Frame::Block     &block )
{
	const register_t blockDiv = block.size() / 3;
	uint8_t *data = &block[0];
	for ( register_t i = 0; i < blockDiv; ++i )
	{
		if ( *data++ > 128 )
		{
			if ( *data++ > 128 )
				*data++ = lut[0][*data];
			else
				*data++ = lut[1][*data];
		}
		else
		{
			if ( *data++ > 128 )
				*data++ = lut[2][*data];
			else
				*data++ = lut[0][*data];
		}

	}
}

register_t
lutEncrypt( Frame::Block     &block )
{
	lutCrypt(kEncryptLut, block);
	return ( 0 );
}

register_t
lutDecrypt( Frame::Block     &block )
{
	lutCrypt(kDecryptLut, block);
	return ( 0 );
}

} /* namespace rikiGlue */
