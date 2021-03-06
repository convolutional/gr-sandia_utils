/* -*- c++ -*- */
/* 
 * Copyright 2018 gr-sandia_utils author.
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


#ifndef INCLUDED_SANDIA_UTILS_MESSAGE_VECTOR_RASTER_FILE_SINK_H
#define INCLUDED_SANDIA_UTILS_MESSAGE_VECTOR_RASTER_FILE_SINK_H

#include <sandia_utils/api.h>
#include <gnuradio/sync_block.h>
#include<string>

namespace gr {
  namespace sandia_utils {

    /*!
     * \brief <+description of block+>
     * \ingroup sandia_utils
     *
     */
    class SANDIA_UTILS_API message_vector_raster_file_sink : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<message_vector_raster_file_sink> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of sandia_utils::message_vector_raster_file_sink.
       *
       * To avoid accidental use of raw pointers, sandia_utils::message_vector_raster_file_sink's
       * constructor is in a private implementation
       * class. sandia_utils::message_vector_raster_file_sink::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string filename, int n_rows);

      // reset raster
      virtual void reset() = 0;

      // get the latest file produced
      virtual std::string get_filename() = 0;
    };

  } // namespace sandia_utils
} // namespace gr

#endif /* INCLUDED_SANDIA_UTILS_MESSAGE_VECTOR_RASTER_FILE_SINK_H */

