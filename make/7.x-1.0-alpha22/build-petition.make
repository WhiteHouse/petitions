api = 2
core = 7.x

; Drupal
; -------
; 
;  Patch to handle menu rebuild error
;    @see http://drupal.org/node/972536
; 
; --------------------------------------
projects[drupal][version] = 7.15
projects[drupal][patch][] = http://drupal.org/files/drupal-menu-int-972536-83-D7.patch

; Petition installation profile
; -------------------------------
projects[petition][type] = profile
projects[petition][download][type] = git
projects[petition][download][url] = https://github.com/WhiteHouse/petition.git
projects[petition][download][branch] = 7.x-1.x
projects[petition][download][revision] = 7.x-1.0-alpha22
