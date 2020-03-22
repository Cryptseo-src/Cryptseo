// Copyright (c) 2012-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The CSEO developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Functionality for communicating with CryptseoGate.
 */
#ifndef BITCOIN_CSEOCONTROL_H
#define BITCOIN_CSEOCONTROL_H

#include <string>

#include <boost/function.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/thread.hpp>

extern const std::string DEFAULT_CSEO_CONTROL;
static const bool DEFAULT_LISTEN_ONION = true;

void StartCryptseoControl(boost::thread_group& threadGroup);
void InterruptCryptseoControl();
void StopCryptseoControl();

#endif /* BITCOIN_CSEOCONTROL_H */


