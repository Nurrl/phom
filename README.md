# phom [![Build Status](https://travis-ci.org/Nurrl/phom.svg?branch=develop)](https://travis-ci.org/Nurrl/phom)
Command-Line Utily to setup and manage virtual hosts for nginx and apache.

This follows: http://google.github.io/styleguide/cppguide.html

## User tree prevision
```
/**
 * User space tree:
 * /
 * |-- /usr
 *     |-- share
 *         |-- php (duplicate)
 *         |-- zoneinfo (duplicate)
 * |-- /etc
 *     |-- localtime (duplicate)
 *     |-- hosts (from template)
 *     |-- nsswitch.conf (duplicate)
 *     |-- resolv.conf (duplicate)
 *     |-- services (duplicate)
 *     |-- ssl
 *         |-- certs (duplicate)
 * |-- /var
 *     |-- lib
 *         |-- php
 *             |-- session
 * |-- /lib
 *     |-- *
 *         |-- libnss_dns.so.2 (duplicate)
 * |-- /tmp
 * |-- /dev
 *     |-- urandom (duplicate)
 *     |-- zero (duplicate)
 *     |-- null (duplicate)
 * |-- /data (User data)
 *     |-- cred.json (Json for ftp credentials)
 *     |-- (***.tld)
 *         |-- (pool.cfg)
 *         |-- (pool<phpversion>.cfg -> pool.cfg)
 *     |-- ...
 * |-- /space (User accessible space from ftp)
 *     |-- (***.tld)
 *     |-- (...)
 *     |-- logs
 *         |-- (***.tld)
 *             |-- (error.log)
 *             |-- (phperror.log)
 *         |-- (...)
 *     |-- readme
 * |-- /<path to />
 *     |-- space --> /space
 *
 */

 /**
  * Web Server tree:
  * /
  * |-- /data (User data)
  *     |-- ***.tld
  *         |-- pool.cfg
  *         |-- pool<phpversion>.cfg -> pool.cfg
  *     |-- ...
  * |-- /space (User accessible space from ftp)
  *     |-- ***.tld
  *     |-- logs
  *         |-- ***.tld
  *             |-- error.log
  *             |-- phperror.log
  *
  */
```
