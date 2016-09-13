PETITIONS RESPONSIVE
--------------------

Petitions Responsive, a mobile-first responsive theme for We The People. This
theme was built using the starter theme 
[Basic](https://www.drupal.org/project/basic). The theme uses SASS stylesheets,
and a SASS-based responsive grid provided by
[Bourbon Neat](http://neat.bourbon.io/).


ABOUT SASS AND BOURBON
----------------------

Sass is An extension of CSS that adds power and elegance to the basic language.
It allows to use variables, nested rules, mixins, inline imports, and more.

To learn more about Sass, visit: http://sass-lang.com

Bourbon is a library of pure Sass mixins that are designed to be simple and easy
to use. No configuration required. The mixins aim to be as vanilla as possible,
meaning they should be as close to the original CSS syntax as possible. Neat is
a lightweight semantic grid framework.

To learn more about Bourbon and Neat, visit: http://bourbon.io


DEVELOPING WITH SASS AND GRUNT
---------------------------------------

Petitions Responsive includes support for [grunt](http://gruntjs.com/) to 
automate compiling the theme's stylesheets and minifying the theme's javascript 
and images. To use grunt for the first time, you'll need to install 
[NodeJs](http://nodejs.org/) and [Ruby](https://www.ruby-lang.org/en/downloads/)
if they aren't already installed.

Dev Dependencies:
- [Ruby](https://www.ruby-lang.org/en/downloads/)
- Bundler (run `$ gem install bundler` after installing Ruby)
- [NodeJS](http://nodejs.org), available via [homebrew](http://brew.sh/) for Mac
- Grunt (run `$ npm install -g grunt-cli --save-dev` after installing Grunt) 

Getting started:

1. Navigate to the theme root and install ruby dependencies: 

  `$ bundle install`

2. cd into the grunt folder and install npm dependencies: 

  `$ npm install`

3. While still in the grunt folder, run grunt: 

  `$ grunt`

By default, grunt will watch all of your asset directories and process any sass,
js, and images included in the theme. However, you can also generate CSS from 
your sass stylesheets directly, by using `sass --watch`. From the theme root:

  `sass -r sass-globbing --watch scss:css`
