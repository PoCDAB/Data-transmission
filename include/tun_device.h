/*
 * Copyright (C) 2017 Opendigitalradio (http://www.opendigitalradio.org/)
 * Copyright (C) 2017 Felix Morgner <felix.morgner@hsr.ch>
 * Copyright (C) 2017 Tobias Stauber <tobias.stauber@hsr.ch>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __DOD_TUN_DEVICE

#include <linux/if_tun.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#include <asio/io_service.hpp>
#include <asio/posix/stream_descriptor.hpp>

#include <array>
#include <functional>
#include <string>
#include <system_error>
#include <vector>

// A simple wrapper to handle tun device I/O using ASIO
struct tunnel_for_device
  {
  //Create a tun device wrapper with a user provided name
  tunnel_for_device(asio::io_service & eventLoop, std::string const & name);

  ~tunnel_for_device();

  // Enqueue data to be sent to the operating system
  void enqueue(std::vector<unsigned char> && data);
  
  // Put the interface up
  std::error_code up();

  // Retrieve the actual name of the tun device
  std::string const & name() const;

  // Get the device address
  std::string address();

  // Set the IPv4 address of the device.
  std::error_code address(std::string const & address);

  private:
    void do_write(std::vector<unsigned char> && data);

    std::error_code ioctl(int const type, ifreq & request);

    asio::posix::stream_descriptor m_device;

    std::string m_name{};
    int m_ioctlDummy{-1};
  };

#endif

