; Petition
; ---------

; Core version
; -------------
core = 7.x

; API version
; ------------
api = 2

; Contrib projects
; -----------------
projects[advanced_help][subdir] = contrib

projects[captcha][subdir] = contrib

projects[coder][subdir] = contrib

projects[context][subdir] = contrib

projects[ctools][subdir] = contrib

projects[date][subdir] = contrib

projects[devel][subdir] = contrib

projects[diff][subdir] = contrib

projects[email_confirm][subdir] = contrib

projects[entity][subdir] = contrib

projects[entitycache][subdir] = contrib

projects[features][subdir] = contrib

projects[feeds][subdir] = contrib

projects[forward][subdir] = contrib

projects[google_analytics][subdir] = contrib

projects[govdelivery][subdir] = contrib

projects[imce][subdir] = contrib

projects[job_scheduler][subdir] = contrib

projects[maintenance200][subdir] = contrib

projects[memcache][subdir] = contrib

projects[metatag][subdir] = contrib

; TODO: Remove this?
; -------------------
projects[migrate][subdir] = contrib

; TODO: Remove this?
; -------------------
projects[migrate_extras][subdir] = contrib

projects[mongodb][subdir] = contrib

projects[openidadmin][subdir] = contrib

projects[pathauto][subdir] = contrib

projects[profile2][subdir] = contrib

projects[recaptcha][subdir] = contrib

projects[rules][subdir] = contrib

projects[strongarm][subdir] = contrib

projects[token][subdir] = contrib

projects[views][subdir] = contrib

projects[wysiwyg][subdir] = contrib

; Patched contrib
;-----------------

; logintoboggan-lower-password.patch
;   - Provide password token for emailing users their password upon registration
;   - Performance improvement, get rid of mysql LOWER(), it creates a temp table for the whole table
;   - @see http://drupal.org/node/1747878
;
;------------------------------------
projects[logintoboggan][subdir] = contrib
projects[logintoboggan][patch][] = http://drupal.org/files/logintoboggan-lower-password.patch

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
projects[nagios][subdir] = contrib
projects[nagios][patch][] = http://drupal.org/files/check_type.patch
projects[nagios][patch][] = http://drupal.org/files/check_checks.patch

; Contrib themes
-----------------
projects[tao][type] = theme

projects[rubik][type] = theme


; Custom
; TODO: FIGURE OUT WHAT TO DO WITH THESE
; ---------
; wh_core/
; wh_feedback/
; wh_profile_migrate/
; wh_user_migrate/
; wh_user_migrate_alt/
; wh_zipcodelookup/


; features
; TODO: FIGURE OUT WHAT TO DO WITH THESE
; ---------
; petitions_user_registration/
; wh_user_profile/
; wh_user_ss_data/
