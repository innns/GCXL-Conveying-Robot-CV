/*
 *  Copyright (C) @innns 2021.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 * @Descripttion: 
 * @version: 1.0
 * @Author: innns
 * @Email: ureinsecure@outlook.com
 * @Date: 2021-05-06 17:55:18
 * @LastEditors: innns
 * @LastEditTime: 2021-05-06 18:07:11
 * @FilePath: /GCXL-Conveying-Robot-CV/include/TCP/TCP_IP.hpp
*/
#include "TCP_Client.hpp"
#include "TCP_Server.hpp"

#include <iostream>    /* cout */
#include <unistd.h>    /* gethostname */
#include <netdb.h>     /* struct hostent */
#include <arpa/inet.h> /* inet_ntop */

std::string getHostIP()
{
    char name[256];
    gethostname(name, sizeof(name));
    struct hostent *host = gethostbyname(name);
    char ipStr[32];
    const char *ret = inet_ntop(host->h_addrtype, host->h_addr_list[0], ipStr, sizeof(ipStr));
    if (NULL == ret)
    {
        std::cout << "hostname transform to ip failed";
        return "0.0.0.0";
    }
    return ipStr;
}

