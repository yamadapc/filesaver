"use strict";
exports.__esModule = true;
exports.getObservable = exports.getStats = void 0;
var rxjs_1 = require("rxjs");
var filesaver_wrapper_1 = require("../filesaver-wrapper");
var initialStats = {
    totalFiles: 0,
    totalKnownFiles: 0,
    filesPerSecond: 0,
    elapsed: 0,
    numWorkers: 0,
    storageQueueSize: 0,
    inMemoryEntryCount: 0
};
function getStats() {
    return {
        totalFiles: filesaver_wrapper_1.getTotalFiles(),
        elapsed: filesaver_wrapper_1.getElapsed(),
        filesPerSecond: filesaver_wrapper_1.getFilesPerSecond(),
        inMemoryEntryCount: filesaver_wrapper_1.getInMemoryEntryCount(),
        numWorkers: filesaver_wrapper_1.getNumWorkers(),
        storageQueueSize: filesaver_wrapper_1.getStorageQueueSize(),
        totalKnownFiles: filesaver_wrapper_1.getTotalKnownFiles()
    };
}
exports.getStats = getStats;
function getObservable(period) {
    if (period === void 0) { period = 500; }
    return new rxjs_1.Observable(function (subscriber) {
        subscriber.next(getStats());
        var interval = setInterval(function () {
            var stats = getStats();
            subscriber.next(stats);
        }, period);
        return function () {
            clearInterval(interval);
        };
    });
}
exports.getObservable = getObservable;
//# sourceMappingURL=index.js.map