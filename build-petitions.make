api = 2
core = 7.x

; Drupal
; -------
;
;  Patch to handle menu rebuild error
;    @see http://drupal.org/node/972536
;
; --------------------------------------
projects[drupal][version] = 7.24
projects[drupal][patch][] = http://drupal.org/files/drupal-menu-int-972536-83-D7.patch

;  Patch correcting database switching within a Simpletest run.
;    @see https://drupal.org/node/2155023
; -------------------------------
projects[drupal][patch][] = https://drupal.org/files/issues/drupal-insertassert_exception-2155023-3.patch

; Petitions installation profile
; -------------------------------
projects[petitions][version] = 2.0-rc11
