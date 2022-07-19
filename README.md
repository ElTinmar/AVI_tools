# AVI_tools

# View motion vectors

ffplay -flags2 +export_mvs input.avi -vf codecview=mv=pf+bf+bb

# Cool stuff

```
sudo apt install xawtv-tools  
showriff input.avi  
```

```
Contents of file input.avi (7303152568/0x303152568 bytes):

(0x00000000)       ID:<RIFF>   Size: 0x40332b16
                   Form Type = <AVI >
(0x0000000c)           ID:<LIST>   Size: 0x000011ec
                       List Type = <hdrl>
(0x00000018)               ID:<avih>   Size: 0x00000038
	us_frame     = 41666
	max_bps      = 184320
	pad_gran     = 0
	flags        = 0x910
		0x10: hasindex
		0x100: interleaved
	tot_frames   = 139672
	init_frames  = 0
	streams      = 1
	sug_bsize    = 1048576
	width        = 384
	height       = 320
	scale        = 0
	rate         = 0
	start        = 0
	length       = 0
(0x00000058)               ID:<LIST>   Size: 0x00001094
                           List Type = <strl>
(0x00000064)                   ID:<strh>   Size: 0x00000038
	fcc_type     = vids
	fcc_handler  = X264 (0x34363258)
	flags        = 0x0
	priority     = 0
	init_frames  = 0
	scale        = 1
	rate         = 24
	start        = 0
	length       = 950003
	sug_bsize    = 16833
	quality      = -1
	samp_size    = 0
(0x000000a4)                   ID:<strf>   Size: 0x00000028
	size         = 40
	width        = 384
	height       = 320
	planes       = 1
	bit_cnt      = 24
	compression  = X264 (0x34363258)
	image_size   = 368640
	xpels_meter  = 0
	ypels_meter  = 0
	num_colors   = 0
	imp_colors   = 0
(0x000000d4)                   ID:<indx>   Size: 0x00001018
(0x000010f4)               ID:<LIST>   Size: 0x00000104
                           List Type = <odml>
(0x00001100)                   ID:<dmlh>   Size: 0x000000f8
	frames       = 950003
(0x00001200)           ID:<LIST>   Size: 0x0000001a
                       List Type = <INFO>
(0x0000120c)               ID:<ISFT>   Size: 0x0000000e
(0x00001222)           ID:<JUNK>   Size: 0x000003f8
(0x00001622)           ID:<LIST>   Size: 0x4010fb7c
                       List Type = <movi>
(0x0000162e)               ID:<00dc>   Size: 0x0000331f
(0x00004956)               ID:<00dc>   Size: 0x00000e79
(0x000057d8)               ID:<00dc>   Size: 0x000024ab
(0x00007c8c)               ID:<00dc>   Size: 0x00003f68
(0x0000bbfc)               ID:<00dc>   Size: 0x00001081
(0x0000cc86)               ID:<00dc>   Size: 0x000016a4
(0x0000e332)               ID:<00dc>   Size: 0x00001004
(0x0000f33e)               ID:<00dc>   Size: 0x000010fa
(0x00010440)               ID:<00dc>   Size: 0x00002d0c
(0x00013154)               ID:<00dc>   Size: 0x00002bd8
(0x00015d34)               ID:<00dc>   Size: 0x000017f3
(0x00017530)               ID:<00dc>   Size: 0x0000167b
(0x00018bb4)               ID:<00dc>   Size: 0x00002131
(0x0001acee)               ID:<00dc>   Size: 0x00001634
(0x0001c32a)               ID:<00dc>   Size: 0x00001732
(0x0001da64)               ID:<00dc>   Size: 0x0000216c
(0x0001fbd8)               ID:<00dc>   Size: 0x00002356
(0x00021f36)               ID:<00dc>   Size: 0x00002265
(0x000241a4)               ID:<00dc>   Size: 0x00001861
(0x00025a0e)               ID:<00dc>   Size: 0x000021b6
(0x00027bcc)               ID:<00dc>   Size: 0x0000174f
(0x00029324)               ID:<00dc>   Size: 0x00002135
(0x0002b462)               ID:<00dc>   Size: 0x000017fd
(0x0002cc68)               ID:<00dc>   Size: 0x00002411
(0x0002f082)               ID:<00dc>   Size: 0x00001aa7
(0x00030b32)               ID:<00dc>   Size: 0x000017d0
(0x0003230a)               ID:<00dc>   Size: 0x00002a26
(0x00034d38)               ID:<00dc>   Size: 0x000017f1
(0x00036532)               ID:<00dc>   Size: 0x000023d8
(0x00038912)               ID:<00dc>   Size: 0x0000189c
(0x0003a1b6)               ID:<00dc>   Size: 0x000023fd
(0x0003c5bc)               ID:<00dc>   Size: 0x00001759
(0x0003dd1e)               ID:<00dc>   Size: 0x000021d0
(0x0003fef6)               ID:<00dc>   Size: 0x00002208
(0x00042106)               ID:<00dc>   Size: 0x000018f3
(0x00043a02)               ID:<00dc>   Size: 0x00002150
(0x00045b5a)               ID:<00dc>   Size: 0x00001785
(0x000472e8)               ID:<00dc>   Size: 0x000021a3
(0x00049494)               ID:<00dc>   Size: 0x0000219a
```
Now let's extract the first h264 encoded frame.  
First we need to convert to decimal

(0x0000162e)               ID:<00dc>   Size: 0x0000331f  
0x0000162e = 5678  
0x0000331f = 13087  

We need to add an offset of 8 bytes to the start to remove the chunk ID ('00dc') and size : 5678 + 8 = 5686.
Now we can extract the h264 data with the dd command reading each byte (bs=1).
I think this works only for I-frames.

```
dd skip=5686 count=13087 bs=1 if=~/Desktop/forEmi/2021_07_26_B.avi of=test.h264
ffplay -i test.h264
```
You can check that there is the NAL unit (https://www.itu.int/rec/T-REC-H.264) at the beginning

```
$ hexdump -C test.h264 -n 48  
00000000  00 00 00 01 67 42 c0 29  8c 8d 40 c0 52 40 3c 22  |....gB.)..@.R@<"|  
00000010  11 a8 00 00 00 01 68 ce  3c 80 00 00 00 01 65 b8  |......h.<.....e.|  
00000020  00 04 00 00 7e ff cb 6f  71 20 07 1c 18 0d 13 00  |....~..oq ......|  
```
