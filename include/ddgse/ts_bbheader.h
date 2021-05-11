/* -*- c++ -*- */
/*
 * Copyright 2021 Ron Economos.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DDGSE_TS_BBHEADER_H
#define INCLUDED_DDGSE_TS_BBHEADER_H

#include <ddgse/api.h>
#include <ddgse/dvb_config.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ddgse {

    /*!
     * \brief <+description of block+>
     * \ingroup ddgse
     *
     */
    class DDGSE_API ts_bbheader : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<ts_bbheader> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ddgse::ts_bbheader.
       *
       * To avoid accidental use of raw pointers, ddgse::ts_bbheader's
       * constructor is in a private implementation
       * class. ddgse::ts_bbheader::make is the public interface for
       * creating new instances.
       */
      static sptr make(dvb_standard_t standard, dvb_framesize_t framesize, dvb_code_rate_t rate, const char* filename);
    };

  } // namespace ddgse
} // namespace gr

#endif /* INCLUDED_DDGSE_TS_BBHEADER_H */

