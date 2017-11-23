# :droplet: Vapors [![Build Status](https://travis-ci.org/Nurrl/vapors.svg?branch=develop)](https://travis-ci.org/Nurrl/vapors)

Lightweight *nginx* & *apache2* configuration generator and manager for secure and easy web service deployment.

## Config

The config directory, */etc/vapors* is divided in 2 parts :
  * The *default/* directory,
anything in this directory WILL be copied to the client's web root at creation.
  * The *apache2/* & *nginx/* directories,
which are same but for different http servers, these contain configuration templates.

### Template help:
```
(~ ) is the base substitution marker.
  
(~servername) -> The main domain for the web server.
(~serveralias) -> The alias(es) for the domain, separated by spaces.
(~serverpath) -> The server web directory's relative path.
(~serverrealpath) -> The server web directory's absolute path.
  
(~username) -> The *nix user name.
(~userroot) -> The user root directory's relative path.
(~userrealroot) -> The user root directory's absolute path.
  
(~logpath) -> The log directory's relative path.
(~logrealpath) -> The log directory's absolute path.

(~email) -> Web master email.
(~phpver) -> PHP Version.
  
  
{~ } is the base logical marker.
  
{~if (not) var}..!{~else}..!{~endif} -> Basic conditionals
```

## License

This project is licensed under the GNU GPL-3.0 License - see the [LICENSE](LICENSE) file for details
