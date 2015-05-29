api = 2
core = 7.x

; Drupal
; -------
;
;  Patch to handle menu rebuild error
;    @see http://drupal.org/node/972536
;
; --------------------------------------
projects[drupal][version] = 7.35
projects[drupal][patch][] = http://drupal.org/files/drupal-menu-int-972536-83-D7.patch

; Petitions installation profile
; -------------------------------
projects[petitions][type] = profile
projects[petitions][download][tag] = 7.x-3.0-alpha2
projects[petitions][download][type] = git
