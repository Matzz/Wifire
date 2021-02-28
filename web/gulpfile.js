const gulp = require('gulp')
const inlinesource = require('gulp-inline-source')
const replace = require('gulp-replace')
const gzip = require('gulp-gzip');

gulp.task('default', () => {
    return gulp.src('./build/*.html')
        .pipe(replace('.js"></script>', '.js" inline></script>'))
        .pipe(replace('rel="stylesheet">', 'rel="stylesheet" inline>'))
        .pipe(inlinesource({
            compress: false
        }))
        .pipe(gzip())
        .pipe(gulp.dest('./combined_build'))
});