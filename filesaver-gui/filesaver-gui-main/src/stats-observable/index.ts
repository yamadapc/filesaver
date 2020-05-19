import { Observable } from "rxjs";
import fileSaver from "../filesaver-wrapper";

export interface Stats {
  totalFiles: number;
  totalKnownFiles: number;
  filesPerSecond: number;
  elapsed: number;
  numWorkers: number;
  storageQueueSize: number;
  inMemoryEntryCount: number;
}

export function getStats(): Stats {
  return {
    totalFiles: fileSaver.getTotalFiles(),
    elapsed: fileSaver.getElapsed(),
    filesPerSecond: fileSaver.getFilesPerSecond(),
    inMemoryEntryCount: fileSaver.getInMemoryEntryCount(),
    numWorkers: fileSaver.getNumWorkers(),
    storageQueueSize: fileSaver.getStorageQueueSize(),
    totalKnownFiles: fileSaver.getTotalKnownFiles(),
  };
}

export function getObservable(period = 500): Observable<Stats> {
  return new Observable((subscriber) => {
    fileSaver.scan("/");
    fileSaver.start();

    subscriber.next(getStats());

    const interval = setInterval(() => {
      const stats = getStats();
      subscriber.next(stats);
    }, period);

    return (): void => {
      clearInterval(interval);
      fileSaver.stop();
    };
  });
}
