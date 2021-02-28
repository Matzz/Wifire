#!/usr/bin/env bash

basedir=$(dirname "$0")

${basedir}/../node_modules/mocha/bin/mocha --config ${basedir}/.mocharc.js "$@" ${basedir}
