Overview:
The signatures queue module implements and provides the infrastructure for moving signatures 
through workflows. It provides a SignaturesQueue class, configuration for signatures queues, 
and a queue monitoring dashboard.

Note:
Workflows are invoked using signatures_queue_invoke_workflow((string){workflow_name}, array $options = array())
This function is normally invoked via Drush (i.e. `drush signatures-queue-invoke-workflow`), except for user-initiated workflows.


To add a new workflow:
* Create ./includes/{workflow_name}.inc
* Within the same file, create a function which handles the specific workflow's logic. Ie:
  function _signatures_queue_{workflow_name}($job_id, $server_name, $worker_name, $options) {
    $queue = SignaturesQueue::get('{workflow_name}');
    $queue->createQueue();
  }
* Reference the other files in ./includes/*.inc for more detail through implementation examples.
* Add the new workflow to the array in signatures_queue_get_workflow_names()
* Update the workflow diagram **				
* Update the SIGNATURE SCHEMA section, found below


Data used during the signature workflows reside in two types of data stores: tables and queues.
The name, data store, and time stored of each workflow can be found below.
The following diagrams are also useful in understanding the signature queue workflows.

Data flow diagram:
./diagrams/data-flow-diagram.png

Workflow diagram:
./diagrams/workflow-diagrams.png

**  See ./diagrams/README.md for information on updating png diagrams

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
