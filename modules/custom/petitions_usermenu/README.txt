Petitions User Module (7.x-1.x)
-------------------------------

This module does not implement any custom menu links. It programatically creates a new menu called Petitions User Menu (petitions-user-menu) with menu_save(), leveraging pages provided by other modules (e.g. user/logout, dashboard) and adding some functionality to control the behaviour of the menu.

All these items are rendered by default on every page, then links are hidden/displayed by JavaScript. This enables us to serve cached pages to authenticated users from our CDN.
