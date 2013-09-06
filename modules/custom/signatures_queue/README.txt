SIGNATURE SCHEMA
name                                    data store                 time stored
signatures_submitted_queue              queue backend              upon submission (less than a day)
signatures_pending_validation_queue     queue backend              upon validation (less than a day)
signatures_pending_validation  table    signatures_processing db   while petition is open
signatures_processed   table            signatures_processing db   while petition is open
signatures_processed_archive table      signatures_archive db      forever
signatures_not_validated_archive table  signatures_archive db      forever

VALIDATION SCHEMA
name                                    data store                 time stored
validations_queue                       queue backend              upon validation (less than a day)
validations  table                      signatures_processing db   while petition is open
validations_processed  table            signatures_processing db   while petition is open
validations_processed_archive  table    signatures_archive db  forever
validations_orphaned_archive   table    signatures_archive db  forever
