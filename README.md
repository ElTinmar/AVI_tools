# AVI_tools

## View motion vectors

ffplay -flags2 +export_mvs input.avi -vf codecview=mv=pf+bf+bb

## Demuxing an AVI frame by frame by hand

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
dd skip=5686 count=13087 bs=1 if=input.avi of=test.h264
ffplay -i test.h264
```
You can check that there are NALUs (https://www.itu.int/rec/T-REC-H.264) at the beginning

```
$ hexdump -C test.h264 -n 48  
00000000  00 00 00 01 67 42 c0 29  8c 8d 40 c0 52 40 3c 22  |....gB.)..@.R@<"|  
00000010  11 a8 00 00 00 01 68 ce  3c 80 00 00 00 01 65 b8  |......h.<.....e.|  
00000020  00 04 00 00 7e ff cb 6f  71 20 07 1c 18 0d 13 00  |....~..oq ......|  
```

## Parse the h264 stream

```
$ ffmpeg -i test.h264 -c copy -bsf:v trace_headers -f null -  

[trace_headers @ 0x5582573dbb40] Packet: 13087 bytes, key frame, no pts, dts 0, duration 48000.
[trace_headers @ 0x5582573dbb40] Sequence Parameter Set
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00111 = 7
[trace_headers @ 0x5582573dbb40] 8           profile_idc                                          01000010 = 66
[trace_headers @ 0x5582573dbb40] 16          constraint_set0_flag                                        1 = 1
[trace_headers @ 0x5582573dbb40] 17          constraint_set1_flag                                        1 = 1
[trace_headers @ 0x5582573dbb40] 18          constraint_set2_flag                                        0 = 0
[trace_headers @ 0x5582573dbb40] 19          constraint_set3_flag                                        0 = 0
[trace_headers @ 0x5582573dbb40] 20          constraint_set4_flag                                        0 = 0
[trace_headers @ 0x5582573dbb40] 21          constraint_set5_flag                                        0 = 0
[trace_headers @ 0x5582573dbb40] 22          reserved_zero_2bits                                        00 = 0
[trace_headers @ 0x5582573dbb40] 24          level_idc                                            00101001 = 41
[trace_headers @ 0x5582573dbb40] 32          seq_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 33          log2_max_frame_num_minus4                             0001100 = 11
[trace_headers @ 0x5582573dbb40] 40          pic_order_cnt_type                                          1 = 0
[trace_headers @ 0x5582573dbb40] 41          log2_max_pic_order_cnt_lsb_minus4                     0001101 = 12
[trace_headers @ 0x5582573dbb40] 48          max_num_ref_frames                                        010 = 1
[trace_headers @ 0x5582573dbb40] 51          gaps_in_frame_num_allowed_flag                              0 = 0
[trace_headers @ 0x5582573dbb40] 52          pic_width_in_mbs_minus1                             000011000 = 23
[trace_headers @ 0x5582573dbb40] 61          pic_height_in_map_units_minus1                      000010100 = 19
[trace_headers @ 0x5582573dbb40] 70          frame_mbs_only_flag                                         1 = 1
[trace_headers @ 0x5582573dbb40] 71          direct_8x8_inference_flag                                   0 = 0
[trace_headers @ 0x5582573dbb40] 72          frame_cropping_flag                                         0 = 0
[trace_headers @ 0x5582573dbb40] 73          vui_parameters_present_flag                                 1 = 1
[trace_headers @ 0x5582573dbb40] 74          aspect_ratio_info_present_flag                              0 = 0
[trace_headers @ 0x5582573dbb40] 75          overscan_info_present_flag                                  0 = 0
[trace_headers @ 0x5582573dbb40] 76          video_signal_type_present_flag                              0 = 0
[trace_headers @ 0x5582573dbb40] 77          chroma_loc_info_present_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 78          timing_info_present_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 79          nal_hrd_parameters_present_flag                             0 = 0
[trace_headers @ 0x5582573dbb40] 80          vcl_hrd_parameters_present_flag                             0 = 0
[trace_headers @ 0x5582573dbb40] 81          pic_struct_present_flag                                     0 = 0
[trace_headers @ 0x5582573dbb40] 82          bitstream_restriction_flag                                  1 = 1
[trace_headers @ 0x5582573dbb40] 83          motion_vectors_over_pic_boundaries_flag                     1 = 1
[trace_headers @ 0x5582573dbb40] 84          max_bytes_per_pic_denom                                     1 = 0
[trace_headers @ 0x5582573dbb40] 85          max_bits_per_mb_denom                                       1 = 0
[trace_headers @ 0x5582573dbb40] 86          log2_max_mv_length_horizontal                       000010001 = 16
[trace_headers @ 0x5582573dbb40] 95          log2_max_mv_length_vertical                         000010001 = 16
[trace_headers @ 0x5582573dbb40] 104         max_num_reorder_frames                                      1 = 0
[trace_headers @ 0x5582573dbb40] 105         max_dec_frame_buffering                                   010 = 1
[trace_headers @ 0x5582573dbb40] 108         rbsp_stop_one_bit                                           1 = 1
[trace_headers @ 0x5582573dbb40] 109         rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 110         rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 111         rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] Picture Parameter Set
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           01000 = 8
[trace_headers @ 0x5582573dbb40] 8           pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 9           seq_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 10          entropy_coding_mode_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 11          bottom_field_pic_order_in_frame_present_flag                0 = 0
[trace_headers @ 0x5582573dbb40] 12          num_slice_groups_minus1                                     1 = 0
[trace_headers @ 0x5582573dbb40] 13          num_ref_idx_l0_default_active_minus1                        1 = 0
[trace_headers @ 0x5582573dbb40] 14          num_ref_idx_l1_default_active_minus1                        1 = 0
[trace_headers @ 0x5582573dbb40] 15          weighted_pred_flag                                          0 = 0
[trace_headers @ 0x5582573dbb40] 16          weighted_bipred_idc                                        00 = 0
[trace_headers @ 0x5582573dbb40] 18          pic_init_qp_minus26                                         1 = 0
[trace_headers @ 0x5582573dbb40] 19          pic_init_qs_minus26                                         1 = 0
[trace_headers @ 0x5582573dbb40] 20          chroma_qp_index_offset                                      1 = 0
[trace_headers @ 0x5582573dbb40] 21          deblocking_filter_control_present_flag                      1 = 1
[trace_headers @ 0x5582573dbb40] 22          constrained_intra_pred_flag                                 0 = 0
[trace_headers @ 0x5582573dbb40] 23          redundant_pic_cnt_present_flag                              0 = 0
[trace_headers @ 0x5582573dbb40] 24          rbsp_stop_one_bit                                           1 = 1
[trace_headers @ 0x5582573dbb40] 25          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 26          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 27          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 28          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 29          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 30          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] 31          rbsp_alignment_zero_bit                                     0 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                                           1 = 0
[trace_headers @ 0x5582573dbb40] 9           slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 12          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 13          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 28          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 31          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 47          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 48          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 49          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 50          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 51          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 52          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                                 00000110001 = 48
[trace_headers @ 0x5582573dbb40] 19          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 22          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 23          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 38          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 41          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 57          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 58          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 59          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 60          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 61          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 62          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                               0000001100001 = 96
[trace_headers @ 0x5582573dbb40] 21          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 24          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 25          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 40          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 43          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 59          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 60          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 61          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 62          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 63          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 64          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                             000000010010001 = 144
[trace_headers @ 0x5582573dbb40] 23          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 26          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 27          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 42          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 45          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 61          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 62          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 63          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 64          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 66          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                             000000011000001 = 192
[trace_headers @ 0x5582573dbb40] 23          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 26          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 27          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 42          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 45          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 61          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 62          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 63          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 64          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 66          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                             000000011110001 = 240
[trace_headers @ 0x5582573dbb40] 23          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 26          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 27          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 42          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 45          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 61          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 62          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 63          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 64          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 66          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                           00000000100100001 = 288
[trace_headers @ 0x5582573dbb40] 25          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 28          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 29          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 44          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 47          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 63          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 64          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 66          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 67          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 68          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                           00000000101010001 = 336
[trace_headers @ 0x5582573dbb40] 25          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 28          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 29          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 44          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 47          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 63          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 64          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 66          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 67          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 68          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                           00000000110000001 = 384
[trace_headers @ 0x5582573dbb40] 25          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 28          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 29          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 44          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 47          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 63          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 64          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 66          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 67          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 68          slice_beta_offset_div2                                      1 = 0
[trace_headers @ 0x5582573dbb40] Slice Header
[trace_headers @ 0x5582573dbb40] 0           forbidden_zero_bit                                          0 = 0
[trace_headers @ 0x5582573dbb40] 1           nal_ref_idc                                                11 = 3
[trace_headers @ 0x5582573dbb40] 3           nal_unit_type                                           00101 = 5
[trace_headers @ 0x5582573dbb40] 8           first_mb_in_slice                           00000000110110001 = 432
[trace_headers @ 0x5582573dbb40] 25          slice_type                                                011 = 2
[trace_headers @ 0x5582573dbb40] 28          pic_parameter_set_id                                        1 = 0
[trace_headers @ 0x5582573dbb40] 29          frame_num                                     000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 44          idr_pic_id                                                010 = 1
[trace_headers @ 0x5582573dbb40] 47          pic_order_cnt_lsb                            0000000000000000 = 0
[trace_headers @ 0x5582573dbb40] 63          no_output_of_prior_pics_flag                                0 = 0
[trace_headers @ 0x5582573dbb40] 64          long_term_reference_flag                                    0 = 0
[trace_headers @ 0x5582573dbb40] 65          slice_qp_delta                                              1 = 0
[trace_headers @ 0x5582573dbb40] 66          disable_deblocking_filter_idc                               1 = 0
[trace_headers @ 0x5582573dbb40] 67          slice_alpha_c0_offset_div2                                  1 = 0
[trace_headers @ 0x5582573dbb40] 68          slice_beta_offset_div2                                      1 = 0
```

You can also use another nice tool

```
pip3 install h26x-extractor  
h26x-extractor -v test.h264

I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[1029, 1964]
NALU offset:	1029 Bytes
NALU length:	936 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x0000000165062e000100001ffd74cd7b880002143fda307a87721a0012d325b207d8d603c04c3889d7a860b98f1cc6602431bb765a080690b8678375b063f6d36c84787d5eef85f28405ad1045c0038ac9900a2a0180181b4186c49acb8b1f91152b336344d8c8e8aabbedd6e9274c3be39dff770d4603dc9a1014511f...
NALU RBSP:	0x062e000100001ffd74cd7b880002143fda307a87721a0012d325b207d8d603c04c3889d7a860b98f1cc6602431bb765a080690b8678375b063f6d36c84787d5eef85f28405ad1045c0038ac9900a2a0180181b4186c49acb8b1f91152b336344d8c8e8aabbedd6e9274c3be39dff770d4603dc9a1014511f859c3d2274...

CodedSliceIDR (payload size: 931.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 48      |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[1965, 2811]
NALU offset:	1965 Bytes
NALU length:	847 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x0000000165030b8000400007ffbfefb8e214d7a01a5dd00ef71f846e9f4b6d2981c8e82e9c7e1b3d9c387336036130904f1d389b040ed7fcff1eec4507567b6f49ef1f80fe5048d4ec0a3ce114e3ccc93984f8d31f9dc3a658674118315c963c9d7d8090905ffe2fcf7c84460060c58bce080313949eb5326bfe34f7bc...
NALU RBSP:	0x030b8000400007ffbfefb8e214d7a01a5dd00ef71f846e9f4b6d2981c8e82e9c7e1b3d9c387336036130904f1d389b040ed7fcff1eec4507567b6f49ef1f80fe5048d4ec0a3ce114e3ccc93984f8d31f9dc3a658674118315c963c9d7d8090905ffe2fcf7c84460060c58bce080313949eb5326bfe34f7bcc1005805ea...

CodedSliceIDR (payload size: 842.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 96      |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[2812, 3875]
NALU offset:	2812 Bytes
NALU length:	1064 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x00000001650122e000100001e10c26133c7a01fffb18fc087b86a998106c5ff402fbf12700fbed29b37303c0812d461a06ccfe3f00719793331736b7a9a458fca5f15ba344ed3b53c5fe40d0107bb486450ec6880870fea43f0a5fe6e759720a134158bc6e39190a6efcd1caf156d01588155096ca0e03d43ce16450e7...
NALU RBSP:	0x0122e000100001e10c26133c7a01fffb18fc087b86a998106c5ff402fbf12700fbed29b37303c0812d461a06ccfe3f00719793331736b7a9a458fca5f15ba344ed3b53c5fe40d0107bb486450ec6880870fea43f0a5fe6e759720a134158bc6e39190a6efcd1caf156d01588155096ca0e03d43ce16450e714627940e2...

CodedSliceIDR (payload size: 1059.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 144     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[3876, 5013]
NALU offset:	3876 Bytes
NALU length:	1138 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x00000001650182e000100001ffe91e83b8a03574044c9f627b1d00d7d3f5fdc4fbc3017dcc1c4200b013a0d876101f5c08ca2bfff97bcafdbeab37afa751244e170e9227359b8c24f40d053d00d4796a25f2fe5e7785c08808664b1c9bb0babe422293461c82999abe252a2186a93560a15f8fc6fafbe3c98451a16340...
NALU RBSP:	0x0182e000100001ffe91e83b8a03574044c9f627b1d00d7d3f5fdc4fbc3017dcc1c4200b013a0d876101f5c08ca2bfff97bcafdbeab37afa751244e170e9227359b8c24f40d053d00d4796a25f2fe5e7785c08808664b1c9bb0babe422293461c82999abe252a2186a93560a15f8fc6fafbe3c98451a163406c080aa892...

CodedSliceIDR (payload size: 1133.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 192     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[5014, 6149]
NALU offset:	5014 Bytes
NALU length:	1136 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x000000016501e2e000100001fffff8a8a0004f7a9bd8afffffefabd4696ccef267fe1a7a45711c200005dcbcdfbbaf8afd910ee97773fe876cb2c203c6a392d442c25aa2f4cb6d35bb983012a05c645e27f180417ca4250c0b0d80001241aabfe5dfdefa485000f84cc73246a745f0741071e000dafb78087c48518430...
NALU RBSP:	0x01e2e000100001fffff8a8a0004f7a9bd8afffffefabd4696ccef267fe1a7a45711c200005dcbcdfbbaf8afd910ee97773fe876cb2c203c6a392d442c25aa2f4cb6d35bb983012a05c645e27f180417ca4250c0b0d80001241aabfe5dfdefa485000f84cc73246a745f0741071e000dafb78087c4851843018b4e9fd66...

CodedSliceIDR (payload size: 1131.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 240     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[6150, 7782]
NALU offset:	6150 Bytes
NALU length:	1633 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x00000001650090b8000400007843059000103d96fffedfff712dee87ebf031d60a6cc371ff6e2307deac337acca28f2d4d4251589a0521f081defee2e3330b85e81b9543d2771457cf489d53e8b4aa195e1cf12a4a4cc1a1f8376ff4fb0c8c4973a654c9d7eedc506eda538402a4cd87d7373f8c531bbdc448af011be9...
NALU RBSP:	0x0090b8000400007843059000103d96fffedfff712dee87ebf031d60a6cc371ff6e2307deac337acca28f2d4d4251589a0521f081defee2e3330b85e81b9543d2771457cf489d53e8b4aa195e1cf12a4a4cc1a1f8376ff4fb0c8c4973a654c9d7eedc506eda538402a4cd87d7373f8c531bbdc448af011be906c8d74b48...

CodedSliceIDR (payload size: 1628.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 288     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[7783, 9376]
NALU offset:	7783 Bytes
NALU length:	1594 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x000000016500a8b8000400007ff8433c2e17abd84a0f36270f0ef2b927ab019d924b212eb87aa44bc226650776a2d781fa38c13ba65b1c642ef5b2f72e1ff681000667db8df38d9b06c671b31f8d24c5b05cb61ce0ff454c45ac947514e0ea177b2cd0ea9b70d998956b005862f18baa31abac2e2bfe4823bb170b3979...
NALU RBSP:	0x00a8b8000400007ff8433c2e17abd84a0f36270f0ef2b927ab019d924b212eb87aa44bc226650776a2d781fa38c13ba65b1c642ef5b2f72e1ff681000667db8df38d9b06c671b31f8d24c5b05cb61ce0ff454c45ac947514e0ea177b2cd0ea9b70d998956b005862f18baa31abac2e2bfe4823bb170b3979740b593dd1...

CodedSliceIDR (payload size: 1589.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 336     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[9377, 11145]
NALU offset:	9377 Bytes
NALU length:	1769 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x000000016500c0b8000400007f0bf0201856e2ac28a9248a90dc7f55ac933971655b4dc32917cc72450a21105cc8402a880c7df664c0a0c16eeaa660d4d9ca3dc712ed054ab7d81bb1aa5ac3da9d0a2f51c0389406fc85152be31bcd8111ca6e8fc88234a6a8b007c1ea5f38a5563aeb2479149a785e47d0411eee0001...
NALU RBSP:	0x00c0b8000400007f0bf0201856e2ac28a9248a90dc7f55ac933971655b4dc32917cc72450a21105cc8402a880c7df664c0a0c16eeaa660d4d9ca3dc712ed054ab7d81bb1aa5ac3da9d0a2f51c0389406fc85152be31bcd8111ca6e8fc88234a6a8b007c1ea5f38a5563aeb2479149a785e47d0411eee0001a15458d12d...

CodedSliceIDR (payload size: 1764.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 384     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

========================================================================================================

NALU bytepos:	[11146, 13086]
NALU offset:	11146 Bytes
NALU length:	1941 Bytes (including start code)
NALU type:	5 (Coded slice of an IDR picture)
NALU bytes:	0x000000016500d8b8000400007f6777690c00c42e0e87c8ad4b09a21782b4cc4205dee69a486cba40c017909d4b540d02092b0e77a6d06b83b3dc96693dc559303135149614c10e4abcd192a5c6d2aa557642cc8965b56e8d77e4102e0970171776367b8b225ab227a941c24e2f537b21460792aa6b4658e3607a92dcb8...
NALU RBSP:	0x00d8b8000400007f6777690c00c42e0e87c8ad4b09a21782b4cc4205dee69a486cba40c017909d4b540d02092b0e77a6d06b83b3dc96693dc559303135149614c10e4abcd192a5c6d2aa557642cc8965b56e8d77e4102e0970171776367b8b225ab227a941c24e2f537b21460792aa6b4658e3607a92dcb80480798ad2...

CodedSliceIDR (payload size: 1936.0 Bytes)
+----------------------+---------+
| field                | value   |
+======================+=========+
| first_mb_in_slice    | 432     |
+----------------------+---------+
| slice_type           | 2       |
+----------------------+---------+
| slice_type_clear     | I       |
+----------------------+---------+
| pic_parameter_set_id | 0       |
+----------------------+---------+
| frame_num            | 0       |
+----------------------+---------+
| idr_pic_id           | 1       |
+----------------------+---------+

```

## Demuxing the whole AVI

```
ffmpeg -i input.avi -c:v copy output.h264
```
