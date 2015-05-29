# Petitions Log

- [Introduction](#introduction)
- [Logging Strategy](#logging-strategy)
- [Events](#events)

## Introduction

The Petitions Log module provides logging facilities for the Petitions
distribution. System events are logged with `petitionslog_event()` using
[Graphite](http://graphite.readthedocs.org/)-compatible event names--i.e., paths
delimited by dots (.). (See [Getting Your Data Into Graphite: Step 1]
(http://graphite.readthedocs.org/en/latest/feeding-carbon.html#step-1-plan-a-naming-hierarchy)
.) The events currently logged by the application are listed [below](#events).

## Logging Strategy

1. Application events are given a log event name beginning with the machine name
   of the module responsible for the associated functionality and further 

1. Exceptional events are given a log event name in the form
   `exceptions.{module_name}.{id}`, where `module_name` is the name of the
   module containing the exception-handling code and `id` is a random, unique
   identifier for the event, generated as follows:
   
   ```bash
   php -r 'print substr(sha1(time()), 0, 7) . "\n";'
   ```

   What constitutes an "exceptional event"? Any instance of application or
   infrastructure misbehavior that merits investigation when it occurs. This
   does not include bad user input or harmless anomalies. It includes such
   things as unexpected PDO exceptions, PHP exceptions, and the like. Exception
   `catch` clauses are a natural use, but not all such clauses should log and
   certainly they are not the only places to log. In general, whether or not an
   an event as an exception can be determined by asking the question, "Would I
   want to be notified if this was happening in production?" If yes, then log it
   as an exception. If no, then don't.

## Events

The following events are being recorded.

| Event | Type |
| --- | --- |
| `exception.{module_name}.{id}` | count |
| `petition.created` | count |
| `petitionssignatureform.form.displayed` | count |
| `petitionssignatureform.form.display_status.configuration_error` | count |
| `petitionssignatureform.form.display_status.invalid_petition_id` | count |
| `petitionssignatureform.form.display_status.ok` | count |
| `petitionssignatureform.form.submission_status.ok` | count |
| `petitionssignatureform.form.submission_status.server_error` | count |
| `petitionssignatureform.form.submission_status.validation_error` | count |
| `petitionssignatureform.form.submitted` | count |
| `signatures_queue.data_flow.time_elapsed.initiated_signature_validation__to__preprocessed_signature` | time |
| `signatures_queue.data_flow.time_elapsed.preprocessed_signature__to__processed_signature` | time |
| `signatures_queue.data_flow.time_elapsed.preprocessed_validation__to__processed_signature` | time |
| `signatures_queue.data_flow.time_elapsed.received_new_signature__to__initiated_signature_validation` | time |
| `signatures_queue.data_flow.time_elapsed.received_signature_validation__to__preprocessed_validation` | time |
| `signatures_queue.data_flow.time_elapsed.received_signature_validation__to__processed_signature` | time |
| `signatures_queue.data_store.{data_store**}.emptied` | count |
| `signatures_queue.data_store.{data_store**}.item_added` | count |
| `signatures_queue.data_store.{data_store**}.item_removed` | count |
| `signatures_queue.data_store.{data_store**}.size` | gauge |
| `signatures_queue.workflow.{workflow*}.caught_up` | count |
| `signatures_queue.workflow.{workflow*}.completed` | count |
| `signatures_queue.workflow.{workflow*}.invoked` | count |
| `signatures_queue.workflow.{workflow*}.processing_time` | time |
| `signatures_queue.workflow.{workflow*}.status.ok` | count |
| `signatures_queue.workflow.{workflow*}.status.bad_request` | count |
| `signatures_queue.workflow.{workflow*}.status.forbidden` | count |
| `signatures_queue.workflow.{workflow*}.status.not_found` | count |
| `signatures_queue.workflow.{workflow*}.status.server_error` | count |
| `signatures_queue.workflow.initiate_signature_validation.time_elapsed.drupal_mail` | time |
| `signatures_queue.workflow.receive_new_signatures.email_address.is_disposable.{true|false}` | count |
| `signatures_queue.workflow.receive_new_signatures.email_address.is_subaddressed.{true|false}` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.email_address.is_disposable.{true|false}` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.email_address.is_subaddressed.{true|false}` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.invoked` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.completed` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.status.ok` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.status.bad_request` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.status.forbidden` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.status.not_found` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.invoked` | count |
| `signatures_queue.workflow.receive_new_signatures.api_key.{api_key}.completed` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.email_address.is_disposable.{true|false}` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.email_address.is_subaddressed.{true|false}` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.invoked` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.completed` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.status.ok` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.status.bad_request` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.status.forbidden` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.status.not_found` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.invoked` | count |
| `signatures_queue.workflow.receive_new_signatures.petition_id.{petition_id}.completed` | count |

\* Workflows:

- `receive_new_signatures`
- `initiate_signature_validation`
- `receive_signature_validation`
- `preprocess_signatures`
- `preprocess_validations` (faux workflow)
- `process_signatures`
- `archive_signatures`

\*\* Data Stores:

- `signatures_submitted_queue`
- `signatures_pending_validation_queue`
- `signatures_pending_validation`
- `signatures_processed`
- `signatures_processed_archive`
- `signatures_not_validated_archive`
- `validations_queue`
- `validations`
- `validations_processed`
- `validations_processed_archive`
- `validations_orphaned_archive`
