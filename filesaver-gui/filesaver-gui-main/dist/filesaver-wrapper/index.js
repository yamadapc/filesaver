"use strict";
exports.__esModule = true;
exports.getInMemoryEntryCount = exports.getStorageQueueSize = exports.getNumWorkers = exports.getFilesPerSecond = exports.getElapsed = exports.getTotalKnownFiles = exports.getTotalFiles = exports.getSize = exports.scan = exports.stop = exports.start = void 0;
// eslint-disable-next-line @typescript-eslint/ban-ts-ignore
// @ts-ignore
var node_filesaver_1 = require("node_filesaver");
var fileSaver = new node_filesaver_1.FileSaver();
function start() {
    fileSaver.start();
}
exports.start = start;
function stop() {
    fileSaver.stop();
}
exports.stop = stop;
function scan(target) {
    fileSaver.scan(target);
}
exports.scan = scan;
function getSize(target) {
    return fileSaver.getCurrentSizeAt(target);
}
exports.getSize = getSize;
function getTotalFiles() {
    return fileSaver.getTotalFiles();
}
exports.getTotalFiles = getTotalFiles;
function getTotalKnownFiles() {
    return fileSaver.getTotalKnownFiles();
}
exports.getTotalKnownFiles = getTotalKnownFiles;
function getElapsed() {
    return fileSaver.getElapsed();
}
exports.getElapsed = getElapsed;
function getFilesPerSecond() {
    return fileSaver.getFilesPerSecond();
}
exports.getFilesPerSecond = getFilesPerSecond;
function getNumWorkers() {
    return fileSaver.getNumWorkers();
}
exports.getNumWorkers = getNumWorkers;
function getStorageQueueSize() {
    return fileSaver.getStorageQueueSize();
}
exports.getStorageQueueSize = getStorageQueueSize;
function getInMemoryEntryCount() {
    return fileSaver.getInMemoryEntryCount();
}
exports.getInMemoryEntryCount = getInMemoryEntryCount;
//# sourceMappingURL=index.js.map