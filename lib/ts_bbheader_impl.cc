/* -*- c++ -*- */
/*
 * Copyright 2021 Ron Economos.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "ts_bbheader_impl.h"

namespace gr {
  namespace ddgse {

    using output_type = unsigned char;
    ts_bbheader::sptr
    ts_bbheader::make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, const char* filename)
    {
      return gnuradio::make_block_sptr<ts_bbheader_impl>(
        standard, framesize, rate, filename);
    }

    /*
     * The private constructor
     */
    ts_bbheader_impl::ts_bbheader_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, const char* filename)
      : gr::sync_block("ts_bbheader",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(output_type)))
    {
      if (framesize == FECFRAME_NORMAL) {
        switch (rate) {
          case C1_4:
            kbch = 16008;
            break;
          case C1_3:
            kbch = 21408;
            break;
          case C2_5:
            kbch = 25728;
            break;
          case C1_2:
            kbch = 32208;
            break;
          case C3_5:
            kbch = 38688;
            break;
          case C2_3:
            kbch = 43040;
            break;
          case C3_4:
            kbch = 48408;
            break;
          case C4_5:
            kbch = 51648;
            break;
          case C5_6:
            kbch = 53840;
            break;
          case C8_9:
            kbch = 57472;
            break;
          case C9_10:
            kbch = 58192;
            break;
          case C2_9_VLSNR:
            kbch = 14208;
            break;
          case C13_45:
            kbch = 18528;
            break;
          case C9_20:
            kbch = 28968;
            break;
          case C90_180:
            kbch = 32208;
            break;
          case C96_180:
            kbch = 34368;
            break;
          case C11_20:
            kbch = 35448;
            break;
          case C100_180:
            kbch = 35808;
            break;
          case C104_180:
            kbch = 37248;
            break;
          case C26_45:
            kbch = 37248;
            break;
          case C18_30:
            kbch = 38688;
            break;
          case C28_45:
            kbch = 40128;
            break;
          case C23_36:
            kbch = 41208;
            break;
          case C116_180:
            kbch = 41568;
            break;
          case C20_30:
            kbch = 43008;
            break;
          case C124_180:
            kbch = 44448;
            break;
          case C25_36:
            kbch = 44808;
            break;
          case C128_180:
            kbch = 45888;
            break;
          case C13_18:
            kbch = 46608;
            break;
          case C132_180:
            kbch = 47328;
            break;
          case C22_30:
            kbch = 47328;
            break;
          case C135_180:
            kbch = 48408;
            break;
          case C140_180:
            kbch = 50208;
            break;
          case C7_9:
            kbch = 50208;
            break;
          case C154_180:
            kbch = 55248;
            break;
          default:
            kbch = 0;
            break;
        }
      }
      else if (framesize == FECFRAME_SHORT) {
        switch (rate) {
          case C1_4:
            kbch = 3072;
            break;
          case C1_3:
            kbch = 5232;
            break;
          case C2_5:
            kbch = 6312;
            break;
          case C1_2:
            kbch = 7032;
            break;
          case C3_5:
            kbch = 9552;
            break;
          case C2_3:
            kbch = 10632;
            break;
          case C3_4:
            kbch = 11712;
            break;
          case C4_5:
            kbch = 12432;
            break;
          case C5_6:
            kbch = 13152;
            break;
          case C8_9:
            kbch = 14232;
            break;
          case C11_45:
            kbch = 3792;
            break;
          case C4_15:
            kbch = 4152;
            break;
          case C14_45:
            kbch = 4872;
            break;
          case C7_15:
            kbch = 7392;
            break;
          case C8_15:
            kbch = 8472;
            break;
          case C26_45:
            kbch = 9192;
            break;
          case C32_45:
            kbch = 11352;
            break;
          case C1_5_VLSNR_SF2:
            kbch = 2512;
            break;
          case C11_45_VLSNR_SF2:
            kbch = 3792;
            break;
          case C1_5_VLSNR:
            kbch = 3072;
            break;
          case C4_15_VLSNR:
            kbch = 4152;
            break;
          case C1_3_VLSNR:
            kbch = 5232;
            break;
          default:
            kbch = 0;
            break;
        }
      }
      else {
        switch (rate) {
          case C1_5_MEDIUM:
            kbch = 5660;
            break;
          case C11_45_MEDIUM:
            kbch = 7740;
            break;
          case C1_3_MEDIUM:
            kbch = 10620;
            break;
          default:
            kbch = 0;
            break;
        }
      }
      if ((d_fp = fopen(filename, "rb")) == NULL) {
        throw std::runtime_error("can't open file");
      }
      sync = FALSE;
      in_packet = 0;
      out_packet = 0;
      set_output_multiple(kbch);
    }

#define CRC_POLY 0xAB

    /*
     * MSB is sent first
     *
     * The polynomial has been reversed
     */
    unsigned int
    ts_bbheader_impl::check_crc8_bits(const unsigned char *in, int length)
    {
      int crc = 0;
      int b;
      int offset, index = 0;
      unsigned char packed;
      unsigned char bits[8];

      for (int i = 0; i < length; i++) {
        packed = in[index++];
        offset = 0;
        for (int n = 7; n >= 0; n--) {
          bits[offset++] = packed & (1 << n) ? 1 : 0;
        }
        for (int j = 0; j < 8; j++) {
          b = bits[j] ^ (crc & 0x01);
          crc >>= 1;
          if (b) {
            crc ^= CRC_POLY;
          }
        }
      }

      return (crc);
    }

    void
    ts_bbheader_impl::xfer_bits(unsigned char *out, const unsigned char *in, int length)
    {
      unsigned char bits;

      for (int j = 0; j < length; j++) {
        bits = in[j + 9];
        for (int n = 7; n >= 0; n--) {
          *out++ = bits & (1 << n) ? 1 : 0;
        }
      }
    }

    /*
     * Our virtual destructor.
     */
    ts_bbheader_impl::~ts_bbheader_impl()
    {
      if (d_fp)
        fclose((FILE*)d_fp);
    }

    int
    ts_bbheader_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      output_type *out = reinterpret_cast<output_type*>(output_items[0]);
      unsigned int produced = 0;
      unsigned int offset = 0;
      unsigned int start_offset;
      unsigned int length, check;
      unsigned int packets_max, packets_1st, packets_2nd;
      size_t bytes_read;
      unsigned int reuse = FALSE;

      if (in_packet >= out_packet) {
        packets_max = (TS_BUFFERS - 1) - (in_packet - out_packet);
        packets_1st = TS_BUFFERS - in_packet;
        if (packets_max < packets_1st) {
          packets_1st = packets_max;
        }
        packets_2nd = packets_max - packets_1st;
        bytes_read = fread(&in[in_packet * TS_PACKET_LENGTH], 1, packets_1st * TS_PACKET_LENGTH, (FILE*)d_fp);
        if (bytes_read % TS_PACKET_LENGTH) {
          throw std::runtime_error("bytes read not multiple of 188");
        }
        in_packet += bytes_read / TS_PACKET_LENGTH;
        in_packet &= (TS_BUFFERS - 1);
        if (packets_2nd) {
          bytes_read = fread(&in[in_packet * TS_PACKET_LENGTH], 1, packets_2nd * TS_PACKET_LENGTH, (FILE*)d_fp);
          if (bytes_read % TS_PACKET_LENGTH) {
            throw std::runtime_error("bytes read not multiple of 188");
          }
          in_packet += bytes_read / TS_PACKET_LENGTH;
          in_packet &= (TS_BUFFERS - 1);
        }
      }
      else {
        packets_max = (out_packet - in_packet) - 1;
        bytes_read = fread(&in[in_packet * TS_PACKET_LENGTH], 1, packets_max * TS_PACKET_LENGTH, (FILE*)d_fp);
        if (bytes_read % TS_PACKET_LENGTH) {
          throw std::runtime_error("bytes read not multiple of 188");
        }
        in_packet += bytes_read / TS_PACKET_LENGTH;
        in_packet &= (TS_BUFFERS - 1);
      }

      while (((int)produced < noutput_items) && (in_packet != out_packet)) {
        if (in[(out_packet * TS_PACKET_LENGTH) + 0] != 0x47) {
          printf("Transport Stream sync error!\n");
        }
        if (sync == FALSE) {
          if (in[(out_packet * TS_PACKET_LENGTH) + 8] == 0xb8) {
            check = check_crc8_bits(&in[(out_packet * TS_PACKET_LENGTH) + 9], (BB_HEADER_LENGTH_BITS + 8) / 8);
            if (check != 0) {
              sync = FALSE;
              printf("TS Baseband header crc failed.\n");
            }
            else {
              dfl = 0;
              dfl |= in[(out_packet * TS_PACKET_LENGTH) + 13] << 8;
              dfl |= in[(out_packet * TS_PACKET_LENGTH) + 14];
              if (in[(out_packet * TS_PACKET_LENGTH) + 7] != 0xb4) {
                length = in[(out_packet * TS_PACKET_LENGTH) + 7] - 1;
                xfer_bits(&out[offset], &in[(out_packet * TS_PACKET_LENGTH)], length);
                offset += length * 8;
                produced += length * 8;
                if (kbch - produced != 0) {
                  length = kbch - produced;
                  memset(&out[offset], 0, length);
                  offset += length;
                  produced += length;
                }
                sync = FALSE;
              }
              else {
                sync = TRUE;
                count = 1;
                start_offset = offset;
                length = in[(out_packet * TS_PACKET_LENGTH) + 7] - 1;
                buffer_offset = 0;
                xfer_bits(&buffer[0], &in[(out_packet * TS_PACKET_LENGTH)], length);
                buffer_offset += length * 8;
              }
            }
          }
          else {
            sync = FALSE;
          }
        }
        else {
          if (in[(out_packet * TS_PACKET_LENGTH) + 8] == count) {
            count++;
            if (in[(out_packet * TS_PACKET_LENGTH) + 7] == 0xb4) {
              length = in[(out_packet * TS_PACKET_LENGTH) + 7] - 1;
              xfer_bits(&buffer[buffer_offset], &in[(out_packet * TS_PACKET_LENGTH)], length);
              buffer_offset += length * 8;
            }
            else {
              length = in[(out_packet * TS_PACKET_LENGTH) + 7] - 1;
              xfer_bits(&buffer[buffer_offset], &in[(out_packet * TS_PACKET_LENGTH)], length);
              buffer_offset += length * 8;
              if (kbch - buffer_offset != 0) {
                length = kbch - buffer_offset;
                memset(&buffer[buffer_offset], 0, length);
              }
              memcpy(&out[offset], &buffer[0], kbch);
              offset += kbch;
              produced += kbch;
              sync = FALSE;
            }
          }
          else {
            printf("sequence error! count = %d, seq = %d\n", count, in[(out_packet * TS_PACKET_LENGTH) + 8]);
            offset = start_offset;
            produced = 0;
            if (in[(out_packet * TS_PACKET_LENGTH) + 8] == 0xb8) {
              reuse = TRUE;
            }
            sync = FALSE;
          }
        }
        if (reuse == FALSE) {
          out_packet++;
          out_packet &= (TS_BUFFERS - 1);
        }
        else {
          reuse = FALSE;
        }
      }

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace ddgse */
} /* namespace gr */

