api = 2
core = 7.x

; Drupal
; -------
;
;  Patch to handle menu rebuild error
;    @see http://drupal.org/node/972536
;
; --------------------------------------
projects[drupal][version] = 7.27
projects[drupal][patch][] = http://drupal.org/files/drupal-menu-int-972536-83-D7.patch

;  Patch correcting IE8 js issue introduced in 7.27. Patch not needed in 7.28 and up.
;    @see https://drupal.org/node/2245331
; -------------------------------
projects[drupal][patch][] = https://www.drupal.org/files/issues/use-of-reserved-word-2245331-1.patch

;  Patch correcting database switching within a Simpletest run.
;    @see https://drupal.org/node/2155023
; -------------------------------
projects[drupal][patch][] = https://drupal.org/files/issues/drupal-insertassert_exception-2155023-3.patch

; Petitions installation profile
; -------------------------------
projects[petitions][type] = profile
projects[petitions][download][tag] = 7.x-2.0-rc26
projects[petitions][download][type] = git
