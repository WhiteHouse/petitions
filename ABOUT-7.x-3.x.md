The 7.x-3.x branch of petitions aims to remove the application's dependency on
Mongo DB and run on MySQL. Between now and whenever 7.x-3.0 is released this
branch will be building out and refining a proof-of-concept for what will likely
turn out to be mysql-based petition nodes and signature entities.

For a stable Mongo-based petitions application, please use the 7.x-2.x branch.


Technical notes on phasing out Mongo dependencies
-------------------------------------------------

  Modules to be removed by the code base by 7.x-3.0 (more modules may get added
  to this list):
  - wh_petitions
  - petitions_data

  Any new code added to the 7.x-3.x branch is being written in anticipation of
  mongo storage being turned off, and petition nodes and signature_mail entities
  based on a mysql back end. Here are some examples of how we're keeping things
  organized in anticipation of mongo going away:

  ### Example #1: Keeping things organized in mymodule.mongo2mysql.inc

    Store functions that are intended to be deleted when mongo gets shut off in
    mymodule.mongo2mysql.inc.

    Name any new functions created in *.mongo2mysql.inc with mongo2mysql in the
    function name. This way, when it's time to shut mongo off, all we have to do
    is (1) find any files with mongo2mysql in the name and delete them, then (2)
    grep through the code for mongo2mysql, and remove those function calls.


  ### Example #2: Calling mongo-dependent functions and checking related shunt trips

    While we're in transition phasing mongo out, application functionality
    should be decoupled from a specific storage backend as much as possible.
    If myslq-based data is available, it's authoritiative. But don't assume it's
    available. If mysql-based data is NOT available, try getting mongo-based
    data. But don't assume that's available either. Eventually it will get shut
    off. For example:

    ```php

      function mymodule_does_something_with_petitions() {

        // Load a petition.
        
        // First try mysql (petition nodes).
        $petition_is_enabled = module_exists('petition');
        $mysql_shunt_is_tripped = shunt_is_enabled('petition_mysql_save');
        if ($petition_is_enabled && !$mysql_shunt_is_tripped) {
          $petition = node_load($nid);
        }

        //    *******************************************************************
        //    *   Note: This entire block of code can simply be deleted after   *
        //    *         mysql is "turned on" and mongo is turned off.           *
        //    *******************************************************************
        //
        // If mysql data isn't available or we need to do something with mongo
        // data for some reason, get it from mongo like this.
        $mongo_shunt_is_tripped = shunt_is_enabled('wh_petitions_petition_create');
        if (!$mongo_shunt_is_tripped) {
          $petition_from_mongo = petitions_data_mongo2mysql_get_petition($mongo_petition_id);
        }
        // Format petition data like a petition node, reconcile the two
        // different petition objects, or throw watchdog errors.
        $petition = mymodule_mongo2mysql_transitional_reconciling_and_formatting_happens_here($petition, $petition2);

        // Now proceed to do stuff with your $petition node (or node-like
        // object)...

      }

    ```

    For a more complex real-world example of ^^ this, see signatures_queue/includes/process_signatures.inc.


  ### Example #3: Phasing out mongo-dependent functions.

    To phase out mongo-dependent functions or anything else that's meant to be removed:
    - Move functions intended to be removed into example.mongo2myql.inc files
    - Rename functions like this: example_get_example() -> example_mongo2mysql_get_example()
    - Replace calls to example_get_example() throughout the codebase with example_mongo2mysql_get_example()
    - In example.mongo2mysql.inc provide the legacy function, but throw an error explaining what's going on

    See petitions_data_get_petition and petitions_data_mongo2mysql_get_petition
    as an example here:
    https://github.com/bryanhirsch/_petitions/commit/453e2f96793e9ce8a95edbc325497bfebd298942

    Note: Lines like this [here](https://github.com/bryanhirsch/_petitions/commit/453e2f96793e9ce8a95edbc325497bfebd298942#diff-325f9f5d1e9e226e082ddc1c205b536eR47)
    are NOT safe and future proofed as described in example #2.
    Developers/Contributors, wherever  possible, please code defensively and
    write things like #2 along the way so there's less housekeeping to do when it's time to grep for things like
    [this](https://github.com/bryanhirsch/_petitions/commit/453e2f96793e9ce8a95edbc325497bfebd298942#diff-325f9f5d1e9e226e082ddc1c205b536eR47)
    and replace it with its mysql equivalents.

