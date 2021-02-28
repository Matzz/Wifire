
const rewire = require('rewire');
const defaults = rewire('react-scripts/scripts/build.js');
const config = defaults.__get__('config');

// Consolidate chunk files instead
config.optimization.splitChunks = {
          chunks(chunk) {
            return false
          },
          cacheGroups: {
			default: false
			}
        };
// Move runtime into bundle instead of separate file
config.optimization.runtimeChunk = false;
// JS
config.output.filename = 'static/js/[name].js';
config.output.chunkFilename = 'static/js/[name].chunk.js';

// console.log("pligns", config.plugins);
// CSS. "5" is MiniCssPlugin
//config.plugins[5].options.filename = 'static/css/[name].css';
