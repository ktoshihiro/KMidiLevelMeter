/*
const int32 kPrimaryImageWidth = 32;
const int32 kPrimaryImageHeight = 32;
const color_space kPrimaryImageColorSpace = B_CMAP8;

const unsigned char kPrimaryImageBits [] = {
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x3f,
	0x3f,0x3f,0x3f,0x3f,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x3f,0x3f,
	0x3f,0x3f,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x3f,
	0x3f,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,
	0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,
	0xff,0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,0xff,
	0xff,0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,0xff,
	0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x00,0x00,0x3f,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x00,0x00,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,
	0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,
	0xff,0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,0xff,
	0xff,0xff,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0xff,0xff,
	0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,
	0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x3f,
	0x3f,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,
	0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0x00,
	0x00,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x3f,
	0x3f,0x3f,0x3f,0x3f,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
*/

//const int32 kSecondaryImageWidth = 16;
//const int32 kSecondaryImageHeight = 16;
//const color_space kSecondaryImageColorSpace = B_CMAP8;

//const unsigned char kSecondaryImageBits [] = {
const unsigned char kMidiMiniIcon [] = {
	0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0x00,0x3f,0x00,0x00,0x3f,0x3f,0x00,0x00,0x3f,0x00,0xff,0xff,0xff,
	0xff,0xff,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0xff,0xff,
	0xff,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0xff,
	0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
	0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,
	0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,
	0x00,0x3f,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x3f,0x00,
	0x00,0x3f,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x3f,0x00,
	0xff,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0xff,
	0xff,0x00,0x3f,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x3f,0x00,0xff,
	0xff,0xff,0x00,0x3f,0x00,0x00,0x00,0x3f,0x3f,0x00,0x00,0x00,0x3f,0x00,0xff,0xff,
	0xff,0xff,0xff,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0x00,0x00,0x3f,0x3f,0x3f,0x3f,0x00,0x00,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff
};

