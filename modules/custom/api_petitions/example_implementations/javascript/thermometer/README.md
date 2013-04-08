
## Contents

 - [About](#about)
 - [Usage](#usage)
   - [Embed the example widget in your website](#embed-the-example-widget-in-your-website)
   - [Customize the thermometer to track your own petition](#customize-the-thermometer-to-track-your-own-petition)
   - [Download and customize the example source code](#download-and-customize-the-example-source-code)
 - [Technical details](#technical-details)
 - [Developers](#developers)
 - [Authors / Maintainers](#authors-maintainers)



## About

This is an example thermometer widget that can be embedded in any website. The widget tracks the signature count for a petition on petitions.whitehouse.gov.

To see an example of what the thermometer looks like, download or clone this repository, then open [index.html](index.html) in your browser.



## Usage

### Embed the example widget in your website

  To add the thermometer to your website, copy and paste the snippet of HTML below into any page.

  ``` HTML
    <link rel="stylesheet" href="https://petitions.whitehouse.gov/sites/default/files/petitions-api-examples/thermometer/css/thermometer.css" type="text/css" />
    <script type="text/javascript" src="http://code.jquery.com/jquery-1.7.1.min.js"></script>
    <script type="text/javascript" src="https://petitions.whitehouse.gov/sites/default/files/petitions-api-examples/thermometer/js/date.format.js"></script>
    <script type="text/javascript" src="https://petitions.whitehouse.gov/sites/default/files/petitions-api-examples/thermometer/js/thermometer.js"></script>
    <div id="thermometer-widget">
      <div id="top-container">
        <div id="petition-title"></div>
        <div id="petition-info">
          <span class="label">Created:</span> <span id="created"></span> |
          <span class="label">Issues:</span> <span id="issues"></span>
        </div>
      </div>
      <div id="main-container" class="clearfix">
        <div id="data-container">
          <div id="item-one" class="clearfix">
            <div class="item-label">
              Total signatures on this petition
            </div>
            <div class="data">
              <span class="display-total-signatures"></span>
            </div>
          </div>
          <div id="item-two" class="clearfix">
            <div class="item-label">
              Signatures needed by <span id="response-deadline"></span> to reach goal of <span id="response-threshold"></span>
            </div>
            <div class="data">
              <span id="remaining-signatures"></span>
            </div>
          </div>
          <div id="item-three" class="clearfix">
            <div id="response-link"></div>
          </div>
        </div>
        <div id="display" class="clearfix">
          <div id="top"></div>
          <div id="middle">
            <div class="value"></div>
          </div>
          <div id="base">
            <span class="display-total-signatures"></span>
          </div>
        </div>
      </div>
    </div>
    <script type="text/javascript">
      $(function() {
        $.ajax({
          url: 'https://api.whitehouse.gov/v1/petitions/50cb6d2ba9a0b1c52e000017.jsonp',
          dataType: 'jsonp',
          success: function(data) {
            petitionData(data);
          }
        });
      });
    </script>
  ```


### Customize the thermometer to track your own petition

  To change which petition the thermometer visualizes, just change the petition ID in the script. Find the ID `50cb6d2ba9a0b1c52e000017` here:

        https://api.whitehouse.gov/v1/petitions/50cb6d2ba9a0b1c52e000017.jsonp

  then replace it with whatever petition ID you want. Look up the ID via the API. For example:

        https://api.whitehouse.gov/v1/petitions

  See additional documentation here: https://petitions.whitehouse.gov/developers.



### Download and customize the example source code

  All the source code for this example is available on GitHub here:

    https://github.com/whitehouse/petitions-api-examples

  Download or clone this repository. The HTML, JS, and CSS files are all included here:

    petitions-api-examples/thermometer



## Technical details

  - Languages: HTML, JavaScript, and CSS
  - Libraries / Frameworks: jQuery
  - Data formats used: JSONP



## Developers

  This widget requires you to know a specific petition ID.  At the time of this release, the only way to look up a specific petition ID is by using the API. To view all
  petitions and their IDs, you can visit:

    https://api.whitehouse.gov/v1/petitions.json

  Detailed documentation about the We the People API is available here:

    https://petitions.whitehouse.gov/developers



## Authors / Maintainers

  This example was developed by the White House "We the People" API team.
