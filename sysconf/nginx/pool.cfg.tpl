[(~servername)]
; Define socket
listen = /run/php/php(~phpver).(~servername).sock

; Define env var for this instance
env[DOCUMENT_ROOT] = (~serverpath)

{~if serverjailed}
; Default pool settings
chroot = (~userrealroot)
chdir = (~serverpath)
{~else}
; Default pool settings
chdir = /
{~endif}

user = (~username)
group = www-data
listen.owner = (~username)
listen.group = www-data
listen.mode = 0666

pm = ondemand
pm.max_children = 16
pm.process_idle_timeout = 8s
pm.max_requests = 64

; Redirect worker stdout and stderr into main error log. If not set, stdout and
; stderr will be redirected to /dev/null according to FastCGI specs.
catch_workers_output = yes

; Pool mix fix
php_admin_value[opcache.enable] = 0

; Fix strange url
php_admin_value[cgi.fix_pathinfo] = 1

; Log file
php_admin_flag[log_errors] = on

{~if serverjailed}
php_admin_value[open_basedir] = (~serverpath):/tmp:/usr/share/php
php_admin_value[error_log] = (~logpath)/(~servername)-php.log
{~else}
php_admin_value[open_basedir] = (~serverrealpath):/tmp:/usr/share/php
php_admin_value[error_log] = (~logrealpath)/(~servername)-php.log
{~endif}
