/*
 
ESP8266 file system builder with PlatformIO support
 
Copyright (C) 2016 by Xose Pérez <xose dot perez at gmail dot com>
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/
 
// -----------------------------------------------------------------------------
// File system builder
// -----------------------------------------------------------------------------
 
const gulp = require('gulp');
const { series } = require('gulp');
const plumber = require('gulp-plumber');
const htmlmin = require('gulp-htmlmin');
const cleancss = require('gulp-clean-css');
const uglify = require('gulp-uglify-es').default;
const gzip = require('gulp-gzip');
const del = require('del');
const useref = require('gulp-useref');
const gulpif = require('gulp-if');
const inline = require('gulp-inline');
const concat = require('gulp-concat');
const compile_utemplates = require('./underscore-support');

const inputPath = 'web/dev/';
const templatesPath = 'web/dev/templates/'
const outputPath = 'web/build/';


/* Clean destination folder */
gulp.task('clean', function() {
    return del([outputPath + '*']);
})
 
/* Copy static files */
gulp.task('copy_static', function() {
    return gulp.src([
            'web/dev/*.{jpg,jpeg,png,ico,gif}',
        ])
        .pipe(gulp.dest('web/build'));
});

/* Compile and merge underscore templates */
gulp.task('build_templates', function() {
    return gulp.src(templatesPath + '*.html')
            .pipe(htmlmin({
                collapseWhitespace: true,
                conservativeCollapse: true
            }))
            .pipe(compile_utemplates())
            .pipe(concat('templates.js'))
            .pipe(gulp.dest(outputPath));
});

/* Minify HTML, CSS, JS */
gulp.task('minify', function() {
    return gulp.src(inputPath + '*.html')
        .pipe(useref())
        .pipe(gulpif('*.css', cleancss()))
        // .pipe(gulpif('*.js', uglify()))
        // .pipe(gulpif('*.html', htmlmin({
        //     collapseWhitespace: true,
        //     removeComments: true
        // })))
        .pipe(gulp.dest(outputPath));
});

/* Inline javascript and css into html files (index.html) */
gulp.task('inline', function() {
    return gulp.src(outputPath + '*.html')
        .pipe(inline({
            base: outputPath,
            js: uglify,
            css: cleancss,
            disabledTypes: ['svg', 'img']
        }))
        .pipe(gzip())
        .pipe(gulp.dest(outputPath));
});

gulp.task('cleanup_tmp_files', function() {
    return del([
        outputPath + 'index.html',
        outputPath + 'core.js',
        outputPath + 'templates.js',
        outputPath + 'core.css',
    ]);
});
 
gulp.task('buildfs', series('clean', 'copy_static', 'minify', 'build_templates', 'inline', 'cleanup_tmp_files'));
gulp.task('default', series('buildfs'));

