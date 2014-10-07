# Signatures Queue

- [Introduction](#introduction)
- [Design](#design)
- [Installation](#installation)
- [Modification](#modification)

## Introduction

The Signatures Queue module provides a queue-based backend for processing
petition signatures submitted via the API. It is composed of a series of
"workflows" through which each signature moves, from receipt, through email
address verification, to counting. The process is designed to 1) prevent spam
and abuse by verifying signatory-supplied email addresses and 2) and to improve
performance and scalability by moving expensive processing to an asynchronous
system.

## Design

The system design is documented in the workflow diagrams in ./diagrams. The data
flow diagram shows the path a signature takes through the system. The workflow
diagrams depict the logical flow within each workflow. These diagrams should
always be updated *before* making changes to the system.

## Installation

The automated Signatures Queue workflows--those that are not user-initiated--are
invoked via Drush commands. "Installing" the queue system involves scheduling
these commands. Following are the commands with recommended frequencies to run
them at:

| Command | Frequency |
|---|---|
| `drush sqiw initiate_signature_validation` | Every minute |
| `drush sqiw preprocess_signatures` | Every minute |
| `drush sqiw process_signatures` | Every minute |
| `drush sqiw archive_signatures` | Every hour |

## Modification

To add a new workflow:
* Update the workflow diagrams. See [diagrams/README.md](./diagrams/README.md).
* Create `./includes/{workflow_machine_name}.inc` containing a function
  `_signatures_queue_{workflow_machine_name}()` with a signature that conforms
  to the invocation in `signatures_queue_invoke_workflow()`. See the other
  includes in the same directory for examples.
* Add the new workflow machine name to the array in
  `signatures_queue_get_workflow_names()`.
