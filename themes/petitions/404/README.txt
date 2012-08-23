To refresh the static 404 page we need to:
 * curl http://www.whitehouse.gov/page-not-found > page-not-found.php
 * wget/curl the aggregated css(2) and js file
 * edit page-not-found.php to change css and js links to use local files
	'/sites/default/themes/whitehouse/404/'
 * test with <test-server>/page-not_found
