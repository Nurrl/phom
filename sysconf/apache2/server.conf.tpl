<Directory (~serverrealpath)>
  Options -Indexes +FollowSymLinks +SymLinksIfOwnerMatch +MultiViews
  AddType application/x-httpd-php .php
  AllowOverride All
  Require all granted
</Directory>
<VirtualHost *:80>
  ServerAdmin (~email)
  DocumentRoot (~serverrealpath)

  ServerName (~servername)
  ServerAlias (~serveralias)

  ErrorLog (~logrealpath)/(~servername)-apache.log

  <IfModule mod_rewrite.c>
    RewriteEngine on
    RewriteCond %{HTTPS} off
    RewriteRule (.*) https://%{HTTP_HOST}%{REQUEST_URI}
  </IfModule>
</VirtualHost>
<IfModule mod_ssl.c>
  <VirtualHost *:443>
    ServerAdmin (~email)
    DocumentRoot (~serverrealpath)

    ServerName (~servername)
    ServerAlias (~serveralias)

    ErrorLog (~logrealpath)/(~servername)-apache.log

    <IfModule mod_rewrite.c>
      RewriteEngine on
      RewriteCond %{HTTP_HOST} !(~servername)
      RewriteRule ^/(.*) http://(~servername)/$1 [R,L]
    </IfModule>

    ProxyErrorOverride on
    <FilesMatch "\.php$">
      SetHandler "proxy:unix:/run/php/php-fpm.(~servername).sock|fcgi://localhost/"
    </FilesMatch>

    Header always set Strict-Transport-Security "max-age=31536000; includeSubDomains"

    SSLEngine on
    SSLProtocol all -SSLv2 -SSLv3
    SSLHonorCipherOrder on
    SSLCipherSuite 'EECDH+ECDSA+AESGCM EECDH+aRSA+AESGCM EECDH+ECDSA+SHA384 EECDH+ECDSA+SHA256 EECDH+aRSA+SHA384 EECDH+aRSA+SHA256 EECDH+aRSA+RC4 EECDH EDH+aRSA RC4 !aNULL !eNULL !LOW !3DES !MD5 !EXP !PSK !SRP !DSS !RC4'
    SSLCertificateFile $letsEncryptDir/live/(~servername)/cert.pem
    SSLCertificateKeyFile $letsEncryptDir/live/(~servername)/privkey.pem
    SSLCertificateChainFile $letsEncryptDir/live/(~servername)/chain.pem
    SSLOptions +StdEnvVars +ExportCertData
  </VirtualHost>
</IfModule>
<IfModule mod_rewrite.c>
    RewriteEngine on
    RewriteCond %{HTTP_HOST} !^(~servername)$
    RewriteRule ^/(.*) http://(~servername)/$1 [R=301,L]
  </IfModule>

  ProxyErrorOverride on
  <FilesMatch "\.php$">
    SetHandler "proxy:unix:/run/php/php(~phpver).(~servername).sock|fcgi://localhost/"
  </FilesMatch>
</VirtualHost>
