/*
 * Copyright 2021 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(ts_bbheader.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(a9d459bcb58c0d280393411b3b384faa)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <ddgse/ts_bbheader.h>
// pydoc.h is automatically generated in the build directory
#include <ts_bbheader_pydoc.h>

void bind_ts_bbheader(py::module& m)
{

    using ts_bbheader    = ::gr::ddgse::ts_bbheader;


    py::class_<ts_bbheader, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<ts_bbheader>>(m, "ts_bbheader", D(ts_bbheader))

        .def(py::init(&ts_bbheader::make),
           py::arg("standard"),
           py::arg("framesize"),
           py::arg("rate"),
           py::arg("filename"),
           D(ts_bbheader,make)
        )
        



        ;




}








