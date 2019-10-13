#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS). Under the terms of Contract DE-NA0003525 with NTESS, the U.S. Government retains certain rights in this software.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr
from gnuradio import blocks
from gnuradio import filter
import timing_utils
import pmt
import numpy


DECIMATE_IN_FREQ_XLATING_FILTER = 1

class uhd_timed_cordic_emulator(gr.hier_block2):
    """
    This block allows for a broadband file to be dehopped at specific times for downstream processing.
    """
    def __init__(self, filename, loop, decimation, samp_rate, f0, short, start_time):
        gr.hier_block2.__init__(self,
            "uhd_timed_cordic_emulator",
            gr.io_signature(0, 0, 0),
            gr.io_signature(1, 1, gr.sizeof_gr_complex))

        # variables
        taps = filter.firdes.low_pass_2(1, samp_rate, (samp_rate*0.4)/decimation, (samp_rate*0.4)/decimation, 60)
        print("freq xlating filter created with " + repr(len(taps)) + " taps")
        fc_start = f0

        # message inputs / outputs
        self.message_port_register_hier_in("command")

        # blocks
        if short:
            self.file = blocks.file_source(gr.sizeof_short*1, filename, loop)
        else:
            self.file = blocks.file_source(gr.sizeof_gr_complex*1, filename, loop)
        self.scale = blocks.multiply_const_vcc((1.0/pow(2,15),))
        self.deint = blocks.interleaved_short_to_complex(False, False)
        self.throttle = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.tagger = timing_utils.add_usrp_tags_c(fc_start, samp_rate, int(start_time), (start_time - int(start_time)))
        self.tuner = timing_utils.retune_uhd_to_timed_tag(int(samp_rate), pmt.intern('dsp_freq'), int(start_time), (start_time - int(start_time)))
        if DECIMATE_IN_FREQ_XLATING_FILTER:
            self.filt = timing_utils.timed_freq_xlating_fir_ccf(decimation, (taps), fc_start, samp_rate)
        else:
            self.filt = timing_utils.timed_freq_xlating_fir_ccf(1, [1], fc_start, samp_rate)
            self.fir = filter.fir_filter_ccf(decimation, (taps))
            self.fir.declare_sample_delay(0)


        # connections
        if short:
            self.connect(self.file, self.deint)
            self.connect(self.deint, self.throttle)
            self.connect(self.throttle, self.scale)
            self.connect(self.scale, self.tagger)
        else:
            self.connect(self.file, self.throttle)
            self.connect(self.throttle, self.tagger)

        self.connect(self.tagger, self.tuner)
        self.connect(self.tuner, self.filt)

        if DECIMATE_IN_FREQ_XLATING_FILTER:
            self.connect(self.filt, self)
        else:
            self.connect(self.filt, self.fir)
            self.connect(self.fir, self)

        self.msg_connect(self, "command", self.tuner, "command")
