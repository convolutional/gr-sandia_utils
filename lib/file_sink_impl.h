/* -*- c++ -*- */
/*
 * Copyright 2017 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_SANDIA_UTILS_FILE_SINK_IMPL_H
#define INCLUDED_SANDIA_UTILS_FILE_SINK_IMPL_H

#include <stdio.h>
#include <iostream>
#include <sys/time.h>         /* struct timeval, gettimeofday */
#include <boost/thread/mutex.hpp>
#include <pmt/pmt.h>
#include <sandia_utils/file_sink.h>
#include <sandia_utils/constants.h>
#include "epoch_time.h"
#include "file_writer_base.h"


namespace gr {
  namespace sandia_utils {

    class file_sink_impl : public file_sink
    {
     private:
      size_t            d_itemsize;
      trigger_type_t    d_mode;
      int               d_freq;
      uint64_t          d_nsamples;
      std::string       d_out_dir;
      std::string       d_name_spec;

      // sampling period
      double                        d_T;

      // base class
      file_writer_base::sptr        d_file_writer;

      // state of recording
      bool                          d_recording;
      bool                          d_recording_changed;
      bool                          d_issue_start;

      // mode state
      // 0 - waiting for start of burst
      // 1 - waiting for end of burst
      int                           d_burst_state;

      // number of samples to discard
      int                           d_ndiscard;

      // align to secondary boundary
      bool                          d_align;

      // sample timestamp (epoch integer and fractional second)
      epoch_time                    d_samp_time;

      // protection
      boost::recursive_mutex        d_mutex;

     public:
      file_sink_impl(std::string type, size_t itemsize, std::string file_type,
                     trigger_type_t mode, uint64_t nsamples, int rate, std::string out_dir, std::string name_spec);
      ~file_sink_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      // must overload
      bool stop() {
        // ensure file writer stops
        d_file_writer->stop();
        // send_update(d_file_writer->get_filename());
        return true;
      }

      // publish updates
      void send_update(std::string fname) {
        // publish update
        pmt::pmt_t msg = pmt::dict_add(pmt::make_dict(), FNAME_KEY,
          pmt::intern(fname));
        message_port_pub(FILE_MSG_KEY, msg);
      }

      // setup rpc
      void setup_rpc();

      // set/get recording state
      void set_recording(bool state);
      bool get_recording();

      // set second  alignment
      void set_second_align(bool align) {d_align = align;}
      bool get_second_align() { return d_align; }

      // set/get mode
      void set_mode(trigger_type_t mode);
      trigger_type_t get_mode();

      // set/get number of samples per file
      void set_nsamples(uint64_t nsamples) {d_file_writer->set_nsamples(nsamples); }
      uint64_t get_nsamples() { return d_file_writer->get_nsamples(); }

      // set/get new folder
      void set_gen_new_folder(bool mode);
      bool get_gen_new_folder() { return d_file_writer->get_gen_new_folder(); }

      // set center freq
      void set_freq(int freq) {d_file_writer->set_freq(freq); }
      int get_freq() { return d_file_writer->get_freq(); }

      // set sample rate
      void set_rate(int rate) { d_file_writer->set_rate(rate); }
      int get_rate() { return d_file_writer->get_rate(); }

    private:
      void do_set_recording(bool state);
      void do_set_mode(trigger_type_t mode);
      int do_handle_tags(std::vector<tag_t> &tags,
        uint64_t starting_offset, bool& do_stop,
        int noutput_items);

    };

  } // namespace sandia_utils
} // namespace gr

#endif /* INCLUDED_SANDIA_UTILS_FILE_SINK_IMPL_H */
