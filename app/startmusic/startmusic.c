/*****************************************************************************
 * Module    : Startmusic
 * File      : startmusic.c
 * Author    :
 * Email     :
 * Function  : ���ſ�������
 *****************************************************************************/
#include "include.h"

#if PLAY_STARTMUSIC

type_startmusic_ctl startmusic_ctl;

#pragma location="STARTMUSIC_CONST"
const u8 startmusic_table[] =
{
	0xFF, 0xE2, 0x18, 0xC0, 0xD8, 0x5D, 0x00, 0x09, 0x38, 0x06, 0x0A, 0x59, 0x41, 0x00, 0x00, 0x21,
	0x49, 0x13, 0x9A, 0x41, 0x40, 0x82, 0x11, 0x78, 0x7D, 0x47, 0x25, 0xDE, 0xA3, 0x90, 0xC5, 0x41,
	0x81, 0x23, 0xB8, 0x21, 0xCA, 0x1C, 0xFF, 0xC4, 0xEF, 0x0F, 0xF2, 0xEF, 0xE1, 0xFE, 0x5D, 0xFF,
	0xFF, 0xFF, 0x67, 0x29, 0xB0, 0x04, 0xEA, 0x00, 0x00, 0x3B, 0x60, 0x9C, 0x11, 0xAA, 0x82, 0x02,
	0x0B, 0xC8, 0x60, 0x56, 0x2B, 0x02, 0xD7, 0x46, 0xFF, 0xE2, 0x18, 0xC0, 0xF4, 0xB7, 0x0C, 0x0C,
	0x89, 0x3A, 0xF2, 0x39, 0x81, 0x53, 0x02, 0xCC, 0x1C, 0xDC, 0xE4, 0xFF, 0x9B, 0xE5, 0xFB, 0xFC,
	0x83, 0xFF, 0xFD, 0xB6, 0x3F, 0xCD, 0x50, 0xC4, 0x41, 0x24, 0xE6, 0x82, 0xFE, 0x0F, 0x33, 0xFE,
	0x03, 0x4F, 0x94, 0xFF, 0xF3, 0x8F, 0xFA, 0xD9, 0xFF, 0xFF, 0xE9, 0xFF, 0x11, 0x7E, 0x0D, 0x08,
	0x21, 0xD7, 0xE4, 0xF4, 0xFF, 0x5F, 0xF5, 0xFD, 0x31, 0xC4, 0xB7, 0xFF, 0xFF, 0xFF, 0xAD, 0x68,
	0xFF, 0xE2, 0x18, 0xC0, 0x3E, 0x5C, 0x12, 0x0C, 0x33, 0x96, 0xE8, 0xF9, 0xC2, 0x68, 0x02, 0xB4,
	0xC4, 0xB0, 0x01, 0x24, 0xBC, 0xDF, 0xFF, 0xFF, 0xFF, 0xEB, 0xA9, 0x25, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFB, 0x37, 0xFF, 0xFF, 0xFF, 0xFF, 0x4D, 0xBF, 0xFF, 0xFF, 0xFF, 0xFD, 0x67, 0x5B, 0x19, 0x24,
	0x21, 0xD7, 0xE5, 0xE7, 0xFE, 0x7F, 0xF6, 0xFE, 0x80, 0x46, 0xFF, 0xFF, 0xB9, 0x17, 0xFF, 0xF5,
	0x01, 0x8E, 0xBF, 0xFF, 0xFF, 0xFF, 0xF8, 0x80, 0xFF, 0xE2, 0x18, 0xC0, 0xC8, 0x63, 0x1A, 0x09,
	0x71, 0xC6, 0xF4, 0xF8, 0x09, 0x4A, 0x6E, 0xB7, 0xFF, 0xFE, 0x63, 0xFF, 0xFF, 0x0E, 0x7F, 0xFF,
	0xA8, 0x32, 0x53, 0x16, 0xB8, 0x80, 0x3C, 0x27, 0xE2, 0x1F, 0xE5, 0x5F, 0xFA, 0x1F, 0xF4, 0xA0,
	0x4E, 0x2C, 0x7F, 0xFD, 0xD1, 0x52, 0x44, 0x90, 0xCE, 0x04, 0xA0, 0x44, 0xA0, 0x91, 0xDB, 0xA9,
	0xD1, 0xFE, 0x62, 0x38, 0x44, 0x41, 0xBF, 0xFF, 0xFC, 0x44, 0x05, 0x22, 0xC4, 0x48, 0xFD, 0x3E,
	0xFF, 0xE2, 0x18, 0xC0, 0xD3, 0x7C, 0x2D, 0x0A, 0x01, 0x3A, 0xE4, 0xF0, 0x29, 0xDA, 0x4C, 0x3F,
	0x7C, 0xAB, 0xFF, 0x4F, 0xF4, 0x08, 0x5B, 0xFF, 0xFC, 0x70, 0x2A, 0x47, 0xFF, 0xFC, 0x71, 0xFF,
	0xFF, 0xE5, 0xC5, 0x5D, 0xFF, 0xFF, 0x35, 0xFE, 0x21, 0xFF, 0xFF, 0xFF, 0xE1, 0x74, 0xF7, 0x20,
	0x20, 0x48, 0xFC, 0x17, 0xA7, 0x93, 0x95, 0xFF, 0x92, 0xFE, 0x4F, 0xFF, 0xFE, 0x12, 0x5F, 0xFF,
	0xFA, 0x7E, 0x84, 0x20, 0xA3, 0xBF, 0xFC, 0x24, 0xFF, 0xE2, 0x18, 0xC0, 0x98, 0x7F, 0x3E, 0x09,
	0x09, 0x32, 0xF8, 0xD8, 0x09, 0xCE, 0x4E, 0xCF, 0xFF, 0xF5, 0x90, 0xFF, 0x09, 0x82, 0x1F, 0xFF,
	0xFF, 0xF5, 0x44, 0xF8, 0x41, 0x16, 0x23, 0xB9, 0xEE, 0xDB, 0x80, 0x3D, 0x1B, 0xDF, 0xFA, 0x1F,
	0xFC, 0x23, 0xF3, 0xFF, 0xFA, 0x09, 0x5F, 0xFF, 0xF9, 0x3F, 0x7B, 0x20, 0x9C, 0x10, 0x3B, 0xFF,
	0xE5, 0xDD, 0xFF, 0xFE, 0x54, 0x09, 0xFC, 0xB1, 0x6F, 0xFF, 0xFF, 0xFF, 0x4C, 0x51, 0x2A, 0x21,
	0xFF, 0xE2, 0x18, 0xC0, 0xCA, 0xA4, 0x52, 0x09, 0x59, 0x56, 0xF8, 0xD8, 0x10, 0x4A, 0x6E, 0x9B,
	0xE5, 0x79, 0x81, 0xB3, 0xE5, 0x3C, 0x66, 0xEB, 0xFA, 0x3F, 0xF8, 0x37, 0xFF, 0xFC, 0x17, 0x83,
	0xDF, 0xFF, 0xF5, 0x08, 0xCD, 0x37, 0xEA, 0x02, 0xCC, 0x9D, 0xFF, 0xFF, 0xFE, 0x50, 0xC7, 0xFF,
	0xFF, 0xFE, 0x5C, 0x97, 0xFF, 0xFF, 0xFE, 0xA3, 0xA4, 0x77, 0xC2, 0xE4, 0x47, 0x7D, 0x5F, 0x40,
	0x0C, 0x6D, 0x3C, 0xC7, 0xFF, 0x7F, 0xF0, 0x4D, 0xFF, 0xE2, 0x18, 0xC0, 0x0C, 0x2E, 0x65, 0x0A,
	0x29, 0x5A, 0xF0, 0xF8, 0x28, 0x8E, 0x6C, 0xFF, 0xFE, 0x22, 0x00, 0x37, 0xAB, 0xFF, 0xE5, 0x57,
	0xFC, 0x45, 0x09, 0xF4, 0x53, 0x10, 0x4D, 0xDD, 0xF5, 0xBB, 0xFF, 0xFF, 0xFF, 0x9C, 0x6F, 0xFC,
	0xBF, 0xFF, 0xF2, 0xA1, 0x20, 0x50, 0x2E, 0x08, 0x77, 0xE9, 0xCF, 0x05, 0xFD, 0x0B, 0xF7, 0x7B,
	0x7C, 0x82, 0x3F, 0xCE, 0xDC, 0xBF, 0xFF, 0x39, 0xBF, 0xFF, 0xE6, 0x3F, 0xE7, 0x9E, 0x35, 0xFE,
	0xFF, 0xE2, 0x18, 0xC0, 0xC6, 0xE0, 0x75, 0x0B, 0x51, 0xC2, 0xEC, 0xF8, 0x51, 0x4E, 0x6C, 0x88,
	0x02, 0xCF, 0xFC, 0xA1, 0xDF, 0xFF, 0xF8, 0x87, 0xFC, 0x1F, 0x7F, 0xFD, 0xAD, 0xFF, 0xFF, 0x16,
	0x2D, 0x11, 0x16, 0x19, 0xCF, 0xE9, 0xCC, 0x0B, 0x79, 0x7E, 0x45, 0xE5, 0xA7, 0xF7, 0x0C, 0xFE,
	0xA7, 0xEA, 0x7F, 0xFF, 0x20, 0xBF, 0xFF, 0xDF, 0xA0, 0xA7, 0xEA, 0x1C, 0x07, 0x7F, 0xE1, 0x00,
	0xD6, 0xFF, 0xEF, 0xFF, 0x50, 0x20, 0x27, 0x10, 0xFF, 0xE2, 0x18, 0xC0, 0x09, 0x76, 0x80, 0x0B,
	0x79, 0x86, 0xFC, 0xD8, 0x38, 0x8A, 0x6E, 0x7F, 0xFF, 0xFC, 0xBB, 0xBF, 0xFF, 0xC3, 0x6A, 0x22,
	0x48, 0xBD, 0x66, 0x07, 0xAF, 0x38, 0x09, 0x7D, 0x38, 0x18, 0x69, 0xF3, 0x1F, 0x61, 0x37, 0xFC,
	0x28, 0xFA, 0xA7, 0xFF, 0x98, 0x58, 0xBF, 0xFF, 0x2F, 0xAB, 0x7E, 0x70, 0xF2, 0x37, 0xF1, 0xE0,
	0x5E, 0xFF, 0xFF, 0xD0, 0xFF, 0xFF, 0xAF, 0xF9, 0xCF, 0x34, 0x75, 0x81, 0xF0, 0x61, 0x01, 0x16,
	0xFF, 0xE2, 0x18, 0xC0, 0x8A, 0xDE, 0x8B, 0x0A, 0xB9, 0x86, 0xF0, 0x30, 0x38, 0x94, 0x6E, 0x45,
	0xAC, 0x46, 0x07, 0xA2, 0xFB, 0x09, 0x73, 0xEA, 0x23, 0x54, 0xE1, 0x32, 0x23, 0x19, 0x9B, 0x48,
	0x9C, 0x98, 0xDF, 0xE3, 0x1F, 0x8E, 0xE8, 0xDF, 0xCA, 0x03, 0x0B, 0x7A, 0x8A, 0x7F, 0x7A, 0x07,
	0x05, 0xBF, 0x52, 0x82, 0x28, 0x92, 0xDF, 0x9E, 0x40, 0x1D, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1,
	0xF0, 0x8C, 0x27, 0x2C, 0xC6, 0x16, 0x99, 0xAE, 0xFF, 0xE2, 0x18, 0xC0, 0x8A, 0x4B, 0x99, 0x0B,
	0xC1, 0x82, 0xF4, 0xF8, 0x39, 0x4A, 0x6C, 0x30, 0xF2, 0x9D, 0x04, 0x4F, 0x41, 0xCD, 0x00, 0xD9,
	0x2A, 0x9D, 0x38, 0xB5, 0xC8, 0x8F, 0xFA, 0x1D, 0x95, 0xF4, 0x15, 0x7D, 0x1B, 0xF8, 0x90, 0x6B,
	0xF9, 0x7F, 0xF4, 0x0F, 0x37, 0xE6, 0x9A, 0x1F, 0xFD, 0x47, 0x41, 0xE9, 0x3F, 0xFA, 0x7F, 0xFF,
	0xFF, 0xFF, 0xE4, 0x03, 0x62, 0x1A, 0x16, 0x49, 0xAA, 0x30, 0xF3, 0xDB, 0x82, 0xEE, 0x50, 0xAD,
	0xFF, 0xE2, 0x18, 0xC0, 0x72, 0x08, 0xA3, 0x0B, 0xD2, 0x46, 0xF9, 0x98, 0x50, 0x8E, 0x90, 0x54,
	0x08, 0x87, 0xA8, 0xC4, 0x56, 0x5B, 0x53, 0xBF, 0x3D, 0x8C, 0x19, 0xD0, 0x14, 0xE8, 0x3D, 0xFE,
	0x8F, 0xFF, 0xE7, 0x8C, 0xEA, 0x4B, 0xFC, 0xB2, 0x7F, 0x42, 0x82, 0x3B, 0xFF, 0xFF, 0xB7, 0xFF,
	0xFF, 0xFE, 0x79, 0x30, 0xA8, 0xB0, 0x16, 0x45, 0x6A, 0x30, 0xF3, 0x7C, 0x29, 0x5B, 0x28, 0x36,
	0xC8, 0x42, 0x40, 0x9C, 0x46, 0x34, 0x89, 0x3D, 0xFF, 0xE2, 0x18, 0xC0, 0x1E, 0x02, 0xAC, 0x0D,
	0x9A, 0x42, 0xF1, 0x98, 0x69, 0x4E, 0x90, 0x44, 0xCF, 0xC4, 0x45, 0x3E, 0x08, 0x2F, 0x42, 0xFF,
	0x0E, 0xFF, 0x43, 0xFE, 0x15, 0x70, 0x02, 0x7F, 0x15, 0x7F, 0xAB, 0x85, 0x86, 0xCF, 0xFE, 0x7A,
	0x3F, 0xFF, 0xFF, 0xFF, 0xF8, 0xD4, 0x0B, 0x13, 0x3C, 0x16, 0x49, 0xA8, 0x66, 0x07, 0x94, 0xEA,
	0x1F, 0xCE, 0x2F, 0xA0, 0x1C, 0xC6, 0x83, 0x10, 0x29, 0xE2, 0x17, 0xF9, 0xC8, 0x23, 0xCC, 0x2C,
	0xFF, 0xE2, 0x18, 0xC0, 0xA4, 0xBF, 0xAE, 0x0C, 0x52, 0x3A, 0xF5, 0x90, 0x51, 0x4E, 0x91, 0xFC,
	0x24, 0x3B, 0xF2, 0x8A, 0x3F, 0xCA, 0xFF, 0x84, 0x07, 0xCC, 0x72, 0x7E, 0xA6, 0xFF, 0x9A, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDE, 0xA8, 0xE0, 0xF8, 0x45, 0x0D, 0xAC, 0x12, 0x85, 0x58, 0x66,
	0x07, 0xBB, 0x70, 0xA4, 0xF1, 0xDC, 0xE1, 0x28, 0x70, 0xA0, 0x87, 0x8D, 0x87, 0x2C, 0x20, 0xC0,
	0x68, 0xBF, 0xC0, 0x04, 0xF9, 0xC2, 0x3E, 0xA0, 0xFF, 0xE2, 0x18, 0xC0, 0x5B, 0xAE, 0xB5, 0x0C,
	0x32, 0x42, 0xF1, 0x90, 0x39, 0xCE, 0x90, 0x5F, 0xC2, 0x17, 0xEB, 0x7F, 0xD3, 0x29, 0x9B, 0xF1,
	0xF1, 0x18, 0x5D, 0xFD, 0x07, 0x53, 0xFF, 0xFF, 0xF9, 0xFF, 0xCD, 0xFF, 0xA4, 0x72, 0x10, 0x30,
	0xBB, 0x17, 0x99, 0xAA, 0x76, 0x07, 0xAF, 0xAC, 0x97, 0xF2, 0x9F, 0x0A, 0xF5, 0x99, 0x10, 0x96,
	0x20, 0xA9, 0xD8, 0x7D, 0x2C, 0x6F, 0x83, 0x7C, 0x04, 0x00, 0xFA, 0xBB, 0x7E, 0x24, 0x5F, 0xAB,
	0xFF, 0xE2, 0x18, 0xC0, 0xA3, 0x9F, 0xBD, 0x0C, 0xF2, 0x42, 0xF1, 0x90, 0x50, 0x8E, 0x90, 0xFE,
	0x53, 0xE2, 0x5F, 0xEC, 0x7F, 0xF9, 0xCF, 0xFF, 0x59, 0xDD, 0x51, 0xBD, 0x9F, 0xFF, 0xFE, 0xD9,
	0xC1, 0x71, 0x21, 0x0C, 0x06, 0x45, 0x5A, 0x20, 0xF3, 0x0E, 0xC4, 0xCE, 0x66, 0x39, 0xAB, 0x44,
	0x28, 0x45, 0xBB, 0x90, 0xDC, 0x7D, 0x29, 0x68, 0x9E, 0x6F, 0x81, 0x1C, 0x4F, 0x84, 0x80, 0x27,
	0xE1, 0xC0, 0xCF, 0xD0, 0x9F, 0xFF, 0xBB, 0xD4, 0xFF, 0xE2, 0x18, 0xC0, 0x6F, 0x07, 0xC2, 0x0C,
	0xEA, 0x46, 0xF1, 0x98, 0x39, 0x4E, 0x90, 0x3D, 0xFC, 0xA1, 0x41, 0x9F, 0xF7, 0x00, 0xB7, 0xFF,
	0xFE, 0x79, 0x2F, 0xFF, 0xFF, 0xFC, 0x74, 0x60, 0x6A, 0x74, 0x16, 0x05, 0x59, 0x06, 0x37, 0x9D,
	0xCF, 0x16, 0x78, 0xBC, 0x3E, 0x65, 0x34, 0x26, 0x82, 0x63, 0xD0, 0xB4, 0xA4, 0x4F, 0xA1, 0xBF,
	0x47, 0x39, 0x3B, 0xB7, 0x41, 0x16, 0xFC, 0xC3, 0xFF, 0xFC, 0x60, 0x2B, 0xD6, 0xDF, 0xC0, 0xC5,
	0xFF, 0xE2, 0x18, 0xC0, 0x92, 0x1F, 0xC7, 0x0D, 0x72, 0x42, 0xE9, 0x98, 0x50, 0x8E, 0x90, 0xFF,
	0x94, 0x50, 0x67, 0xFF, 0xFA, 0x31, 0x5B, 0xFF, 0xFF, 0xFF, 0x0A, 0x28, 0x37, 0xFF, 0xFF, 0xFF,
	0xFF, 0xF4, 0x89, 0x8A, 0xDA, 0x02, 0x04, 0x5A, 0x30, 0xF5, 0xEA, 0x37, 0xEC, 0x28, 0xAC, 0x40,
	0x02, 0x65, 0x03, 0xF5, 0x0C, 0x4B, 0xEA, 0x77, 0xD0, 0x1D, 0xCB, 0xC2, 0x22, 0x0D, 0xCF, 0xFE,
	0x3F, 0xF6, 0xFA, 0x28, 0xE3, 0xC6, 0x93, 0xFA, 0xFF, 0xE2, 0x18, 0xC0, 0xA3, 0xBA, 0xCA, 0x0C,
	0xD2, 0x3E, 0xF1, 0x98, 0x69, 0x4E, 0x90, 0x0F, 0x37, 0xF3, 0x84, 0x61, 0x67, 0xFE, 0x9F, 0x35,
	0xFF, 0xFF, 0xFF, 0xF1, 0x50, 0x07, 0x0B, 0x08, 0x3F, 0xFF, 0x4D, 0x02, 0x44, 0x5A, 0x20, 0xF4,
	0x1F, 0x50, 0xF4, 0xE7, 0x0E, 0xD6, 0xE3, 0x50, 0xA4, 0x66, 0x2D, 0xD4, 0x39, 0x0D, 0xB6, 0x20,
	0xB7, 0xC2, 0x2E, 0x5E, 0x50, 0xC6, 0xD4, 0x43, 0xFA, 0x87, 0x7F, 0x76, 0xF8, 0x80, 0xC3, 0x39,
	0xFF, 0xE2, 0x18, 0xC0, 0x54, 0x27, 0xCF, 0x0D, 0xAA, 0x42, 0xE1, 0x90, 0x69, 0x4E, 0x90, 0xCD,
	0xFC, 0x4C, 0x0C, 0xFF, 0xCE, 0x24, 0x0D, 0xFF, 0xDF, 0xA3, 0x0B, 0x7F, 0xFF, 0xFF, 0xE6, 0x0A,
	0x0F, 0x97, 0x57, 0x45, 0x69, 0xC6, 0x07, 0xA5, 0xD3, 0x1F, 0xFA, 0x49, 0x6B, 0x1A, 0x86, 0xE4,
	0x43, 0x3A, 0x42, 0x56, 0xFA, 0x9F, 0xE8, 0x22, 0xCF, 0xC6, 0x82, 0xF1, 0xA7, 0xFE, 0x35, 0xBF,
	0xFE, 0x42, 0xD4, 0x9F, 0xD4, 0x87, 0xFA, 0x8E, 0xFF, 0xE2, 0x18, 0xC0, 0xFD, 0x76, 0xD1, 0x0E,
	0xCA, 0x3E, 0xE1, 0x98, 0x51, 0x44, 0x90, 0x17, 0xFF, 0xCD, 0x4E, 0x8E, 0x9F, 0xFF, 0xFF, 0xFA,
	0x81, 0x84, 0xC9, 0xFF, 0xFA, 0x7E, 0x11, 0x6A, 0x88, 0x3D, 0x7B, 0x01, 0x2E, 0x81, 0x8E, 0x50,
	0x64, 0xF0, 0x1C, 0xA2, 0x28, 0x6F, 0xBF, 0xD0, 0x18, 0x70, 0xCF, 0x09, 0x93, 0x50, 0x89, 0xFF,
	0x28, 0xAF, 0xEE, 0xBF, 0x98, 0xF3, 0x0B, 0xFF, 0x38, 0x4A, 0x5F, 0xF1, 0xF0, 0x79, 0xFF, 0xFF,
	0xFF, 0xE2, 0x18, 0xC0, 0x51, 0x27, 0xCE, 0x0D, 0xAA, 0x42, 0xE1, 0x90, 0x51, 0x4E, 0x90, 0x33,
	0xFF, 0xFF, 0xFF, 0x41, 0xF0, 0x58, 0xB4, 0x96, 0x6B, 0x79, 0x81, 0xED, 0xE0, 0xB5, 0xB1, 0x31,
	0x1E, 0x50, 0xBA, 0x89, 0x2C, 0x14, 0x17, 0x6B, 0xFA, 0x84, 0x18, 0x5F, 0x88, 0x29, 0x75, 0x41,
	0x6F, 0xDC, 0x7F, 0xED, 0xF9, 0x4A, 0x6B, 0x7F, 0x8A, 0xCB, 0x3F, 0xF1, 0x30, 0x39, 0x12, 0x5B,
	0xFD, 0xAD, 0xA9, 0xE5, 0x4D, 0xF6, 0xFF, 0xFF, 0xFF, 0xE2, 0x18, 0xC0, 0x5C, 0xC7, 0xD0, 0x0D,
	0xF2, 0x42, 0xD9, 0x90, 0x69, 0x4A, 0x90, 0xBD, 0xCA, 0x9C, 0x12, 0x8F, 0x1C, 0x93, 0xD1, 0x5A,
	0x55, 0x01, 0x9F, 0xF9, 0x6C, 0x2D, 0x70, 0x05, 0x3F, 0x85, 0xA7, 0x5E, 0xA0, 0xF2, 0x86, 0xE0,
	0x58, 0xFC, 0x3C, 0x1F, 0x97, 0x49, 0xC0, 0xA5, 0xE8, 0x63, 0x74, 0x0F, 0x2F, 0xEC, 0x66, 0xFF,
	0xEA, 0x45, 0xA3, 0x97, 0xE9, 0x72, 0x3F, 0xEA, 0x44, 0xEF, 0xFF, 0xD5, 0x8E, 0xFF, 0xFF, 0xFD,
	0xFF, 0xE2, 0x18, 0xC0, 0x8D, 0x44, 0xD1, 0x0D, 0x12, 0x42, 0xDD, 0x98, 0x69, 0x4E, 0x90, 0x51,
	0x8F, 0x50, 0x12, 0x2C, 0x3F, 0x41, 0xAF, 0x8D, 0x6E, 0xCA, 0xC4, 0x41, 0x30, 0xC9, 0xD1, 0xE9,
	0x0A, 0x86, 0x41, 0x60, 0x74, 0x28, 0x1E, 0x0A, 0x99, 0x09, 0x11, 0xF8, 0xA9, 0x99, 0x66, 0xBE,
	0xDC, 0x7F, 0x81, 0x58, 0x05, 0x67, 0xAC, 0x8B, 0xBA, 0xFF, 0xFF, 0x01, 0x0B, 0x1A, 0x0A, 0x92,
	0x7F, 0xFE, 0x02, 0x17, 0x01, 0x6A, 0xFF, 0xD2, 0xFF, 0xE2, 0x18, 0xC0, 0xEE, 0x48, 0xD5, 0x0C,
	0x5A, 0x42, 0xD0, 0xF0, 0x39, 0x4E, 0x90, 0xDB, 0x1F, 0x8F, 0x46, 0x80, 0xA1, 0xAF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xE2, 0x18, 0xC0, 0x58, 0xAD, 0xDC, 0x0D, 0xE2, 0x42, 0xD0, 0xF8, 0x39, 0x4E, 0x90, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE2, 0x18, 0xC0, 0xED, 0x35, 0xDD, 0x0D,
	0xF2, 0x3E, 0xC8, 0xF8, 0x49, 0x4E, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xE2, 0x18, 0xC0, 0x5C, 0x91, 0xDE, 0x0E, 0x60, 0x02, 0xA0, 0xF8, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE2, 0x18, 0xC0, 0x84, 0x80, 0xDD, 0x05,
	0xC0, 0x02, 0x5C, 0x00, 0x00, 0x00, 0x00
};


//���ſ�������
#pragma location="STARTMUSIC_SEG"
void startmusic_play(void)
{
	//printf("startmusic_play.\n");

	startmusic_ctl.len = (sizeof(startmusic_table))/512;
	startmusic_ctl.flag = 1;
	startmusic_ctl.ofs = 0;

	if(music_init() == TYPE_MP3)    //ֻ֧��MP3��ʽ
	{
		music_play();
	}

	while(1)
	{
		music_event();

		if(msc_ctl.status == STATUS_PLAY_STOPPING)
		{
			break;
		}
	}

	startmusic_ctl.flag = 0;

	dac_fade_out();
	dac_fade_wait();
}


#endif
