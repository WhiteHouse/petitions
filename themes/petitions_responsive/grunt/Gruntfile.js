module.exports = function(grunt) {

    // This is where we configure each task that we'd like to run.
    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        delta: {
            // This is where we set up all the tasks we'd like grunt to watch for changes.
            scripts: {
                files: ['js-source/*.js'],
                tasks: ['uglify'],
                options: {
                    spawn: false,
                },
            },
            images: {
                files: ['images-source/**/*.{png,jpg,gif}', 'images-source/*.{png,jpg,gif}'],
                tasks: ['imagemin'],
                options: {
                    spawn: false,
                }
            },
            css: {
                files: ['scss/*.scss', 'scss/**/*.scss'],
                tasks: ['sass'],
                options: {
                    spawn: false,
                }
            }
        },
        uglify: {
            // This is for minifying all of our scripts.
            options: {
                mangle: false
            },
            my_target: {
                files: [{
                    expand: true,
                    cwd: 'js-source',
                    src: '*.js',
                    dest: 'js'
                }]
            }
        },
        imagemin: {
            // This will optimize all of our images for the web.
            dynamic: {
                files: [{
                    expand: true,
                    cwd: 'images-source/',
                    src: ['**/*.{png,jpg,gif}','*.{png,jpg,gif}' ],
                    dest: 'images/'
                }]
            }
        },
        sass: {
            // This will compile all of our sass files
            // Additional configuration options can be found at https://github.com/gruntjs/grunt-contrib-sass
            dist: {
                options: {
                    style: 'expanded', // This controls the compiled css and can be changed to nested, compact or compressed
                    require: 'sass-globbing',
                    bundleExec: true,
                },
                files: [{
                    expand: true,
                    cwd: 'scss/',
                    src: ['*.scss'],
                    dest: 'css/',
                    ext: '.css'
                }]
            }
        },
        clean: {
            css: ["css/*.css", "!css/normalize.css"],
            js: ["js/*.js"]
        }
    });
    // This is where we tell Grunt we plan to use this plug-in.
    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-contrib-imagemin');
    grunt.loadNpmTasks('grunt-contrib-sass');
    grunt.loadNpmTasks('grunt-contrib-clean');
    grunt.loadNpmTasks('grunt-contrib-watch');
    // Now that we've loaded the package.json and the node_modules we set the base path
    // for the actual execution of the tasks
    grunt.file.setBase('../')
    // This is where we tell Grunt what to do when we type "grunt" into the terminal.
    // Note. if you'd like to run and of the tasks individually you can do so by typing 'grunt mytaskname' alternatively
    // you can type 'grunt watch' to automatically track your files for changes.
    grunt.renameTask( 'watch', 'delta' );
    grunt.registerTask( 'watch', [ 'clean', 'sass', 'uglify', 'delta' ] );
    grunt.registerTask('default', ['watch']);
};
