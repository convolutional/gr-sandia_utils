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

#ifndef INCLUDED_SANDIA_UTILS_FILE_WRITER_RAW_HEADER_H
#define INCLUDED_SANDIA_UTILS_FILE_WRITER_RAW_HEADER_H

#include <fstream>
#include <sandia_utils/api.h>
#include "file_writer_base.h"

namespace gr {
  namespace sandia_utils {
    class SANDIA_UTILS_API file_writer_raw_header: public file_writer_base
    {
    private:
      std::ofstream         d_outfile;

    public:
      file_writer_raw_header(std::string data_type, std::string file_type,
                    size_t itemsize, uint64_t nsamples, int rate,
                    std::string out_dir, std::string name_spec);
      ~file_writer_raw_header();

      /*!
       * Open a new file
       */
      void open(std::string fname);

      /*!
       * Close the current file
       */
      void close();

      /*!
       * Write data
       */
      int write_impl(const void *in, int nitems);
    };

  } // namespace sandia_utils
} // namespace gr

#endif /* INCLUDED_SANDIA_UTILS_FILE_WRITER_RAW_HEADER_H */
