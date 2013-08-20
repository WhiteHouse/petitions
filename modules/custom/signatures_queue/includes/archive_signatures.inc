<?php

/**
 * @file
 * The "archive signatures" workflow.
 *
 * Retrieve and delete batches of signatures from signatures_submitted_queue,
 * send a validation email, then store signature info in
 * signatures_pending_validation_queue.
 */
