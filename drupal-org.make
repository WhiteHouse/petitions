; Petitions
; ---------

; Core version
; -------------
core = 7.x

; API version
; ------------
api = 2

; Defaults
; ---------
defaults[projects][subdir] = contrib

; Contrib projects
; -----------------
projects[advanced_help][version] = 1.0

projects[captcha][version] = 1.0-beta2

projects[colorbox][version] = 2.2

projects[conditional_styles][version] = 2.1

projects[context][version] = 3.0-beta4

projects[ctools][version] = 1.2

projects[date][version] = 2.6

projects[diff][version] = 2.0-beta2

projects[email_confirm][version] = 1.0

projects[entity][version] = 1.0-rc3

projects[entitycache][version] = 1.1

projects[examples][version] = 1.x-dev

projects[features][version] = 1.0

projects[feeds][version] = 2.0-alpha5

projects[forward][version] = 1.3

projects[globalredirect][version] = 1.5

projects[google_analytics][version] = 1.2

projects[govdelivery][version] = 1.0-alpha1

projects[imce][version] = 1.5

projects[job_scheduler][version] = 2.0-alpha3

projects[libraries][version] = 2.0

projects[maintenance200][version] = 1.0

projects[memcache][version] = 1.0

projects[metatag][version] = 1.0-alpha8

projects[migrate][version] = 2.4

projects[migrate_extras][version] = 2.4

projects[mongodb][version] = 1.0-rc2

projects[openidadmin][version] = 1.0

projects[pathauto][version] = 1.2

projects[profile2][version] = 1.2

projects[recaptcha][version] = 1.7

projects[rules][version] = 2.2

projects[services_documentation][version] = 1.2

projects[shunt][version] = 1.1

projects[textcaptcha][version] = 1.3

projects[simplehtmldom][version] = 1.12

projects[strongarm][version] = 2.0

projects[token][version] = 1.2

projects[views][version] = 3.4

projects[views_bulk_operations][version] = 3.1

projects[wysiwyg][version] = 2.1

; Patched contrib
;-----------------

; 1747878-2-remove-lower.patch
;   - Performance improvement, get rid of mysql LOWER(), it creates a temp table for the whole table
;   - @see http://drupal.org/node/1747878
;
; password_token-1165126-6.patch
;   - Make password available as a token
;   - @see http://drupal.org/node/1165126#comment-5492890
;
;------------------------------------
projects[logintoboggan][version] = 1.3
projects[logintoboggan][patch][] = http://drupal.org/files/1747878-2-remove-lower.patch
projects[logintoboggan][patch][] = http://drupal.org/files/password_token-1165126-6.patch

; check_type.patch
;  - Data types were not checked before getting 'status' key from them
;  - @see http://drupal.org/node/1572368
;
; check_checks.patch
;   - Improved error handling.
;     Before we run checks, lets see if the checks for this module are enabled otherwise, break out of the loop.
;   - @see http://drupal.org/node/1747870
;
;------------------------------------
projects[nagios][version] = 1.1
projects[nagios][patch][] = http://drupal.org/files/check_type.patch
projects[nagios][patch][] = http://drupal.org/files/check_checks.patch

; 1917432-3.patch
;   - Fixes issue where a relationship's source argument (path) only accepts numeric values.
;   - Fix has been applied to the dev branch of the Services module. This patch can be removed when the next release is made available.
;   - @see http://drupal.org/node/1917432
;
; services-1937312-format-arguments-errors.patch
;   - Fixes Services module WSOD on requests missing required arguments.
;
;------------------------------------
projects[services][version] = 3.3
projects[services][patch][] = http://drupal.org/files/services-relationship-source-arg-path-1917432-3.patch
projects[services][patch][] = http://drupal.org/files/exception-data-1935472-D7-9.patch
projects[services][patch][] = https://drupal.org/files/services-1937312-format-arguments-errors.patch

; Contrib themes
; -----------------
projects[tao][type] = theme
projects[tao][subdir] = ""
projects[tao][version] = 3.0-beta3

projects[rubik][type] = theme
projects[rubik][subdir] = ""
projects[rubik][version] = 4.0-beta5

projects[zen][type] = theme
projects[zen][subdir] = ""
projects[zen][version] = 5.1

projects[fortyfour][type] = theme
projects[fortyfour][subdir] = ""
projects[fortyfour][download][type] = git
projects[fortyfour][download][branch] = 7.x-1.x
projects[fortyfour][download][tag] = 7.x-1.0-alpha7

; External libraries
; -----------------

libraries[colorbox][download][type] = "file"
libraries[colorbox][download][url] = "https://github.com/jackmoore/colorbox/archive/1.4.27.zip"
