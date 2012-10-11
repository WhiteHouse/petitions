Petitions Homepage(7.x-1.x)
---------------------------

  This module sets the content variable of the homepage per configuration
  settings.

  This module does two simple things:

    1. Provides default content for the homepage.

    2. Provides configuration settings to alter the /homepage content.

Usage
-----

  1. Go to admin/config/petitions/homepage.

  2. Alter output and/or disable content areas in the Right Sidebar (Aside)
     area of the homepage.

  3. Flush caches

NOTES
-----

  1. Don't enable blocks or regions on the homepage, other than the content
     region. Doing so will break the output defined by the module and/or
     configuration settings.

  2. The homepage uses a grid system (defined in css/petitions-grid.css)

TODO
----

  1. Determine which resources may be removed from the Petitions and Petitions44
     themes' img directories.

  2. Turn the Right Sidebar into a region and convert the content to blocks.

  3. Disable resizable textareas on the configuration form.

  4. Add configuration settings to select the account from which the Recent
     Tweet is retrieved.

  5. Flush caches on form submit automatically.

  6. Allow users to revert to defaults via configuration settings.

  7. Wrap input in check_markup()
