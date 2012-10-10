UNIT TESTS FOR ZEN GRIDS
------------------------

To run the unit tests for Zen Grids:

1. Create a "tests" Compass project using the unit-tests pattern:

   compass create tests -r zen-grids --using=zen-grids/unit-tests

2. From inside the "tests" project, compare the compiled stylesheets to the
   previous unit test results in the test-results directory:

   diff -r test-results/ stylesheets/

   If the unit tests were successful, the above command should report no
   differences.
