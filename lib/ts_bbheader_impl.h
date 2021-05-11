/* -*- c++ -*- */
/*
 * Copyright 2021 Ron Economos.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DDGSE_TS_BBHEADER_IMPL_H
#define INCLUDED_DDGSE_TS_BBHEADER_IMPL_H

#include <ddgse/ts_bbheader.h>

#define TS_PACKET_LENGTH 188
#define KBCH_MAX 58192
#define TS_BUFFERS 64  /* must be power of 2 */

namespace gr {
  namespace ddgse {

    class ts_bbheader_impl : public ts_bbheader
    {
     private:
      FILE* d_fp;
      unsigned int kbch;
      unsigned int sync;
      unsigned int count;
      unsigned int dfl;
      unsigned int in_packet;
      unsigned int out_packet;
      unsigned char in[TS_PACKET_LENGTH * TS_BUFFERS];
      unsigned int buffer_offset;
      unsigned char buffer[KBCH_MAX];
      unsigned int check_crc8_bits(const unsigned char *, int);
      void xfer_bits(unsigned char *, const unsigned char *, int);

     public:
      ts_bbheader_impl(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, const char* filename);
      ~ts_bbheader_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace ddgse
} // namespace gr

#endif /* INCLUDED_DDGSE_TS_BBHEADER_IMPL_H */

